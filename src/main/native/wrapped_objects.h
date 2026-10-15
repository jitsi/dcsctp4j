/*
 * Copyright @ 2024 - present 8x8, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef WRAPPED_OBJECTS_H_
#define WRAPPED_OBJECTS_H_

#include <smjni/smjni.h>
#include <exception>
#include <span>
#include <utility>
#include "all_classes.h"
#include <net/dcsctp/public/dcsctp_socket.h>
#include <net/dcsctp/public/packet_observer.h>

/*
 * dcsctp is compiled without exception support, so an exception raised by a Java
 * callback (a Java exception surfacing as smjni::java_exception, or anything else)
 * must never unwind through dcsctp's frames. Every wrapper runs its callback body
 * through a CallbackExceptionSink: the first exception is kept here and the
 * callback returns a harmless fallback instead. Once dcsctp has returned to the
 * native entry point that triggered the callback, the entry point rethrows the kept
 * exception, and the usual NATIVE_EPILOG handlers deliver it to the Java caller.
 * While an exception is pending no further callbacks are delivered to Java.
 */
class CallbackExceptionSink {
public:
    template <typename Body, typename Fallback>
    auto callOr(Body&& body, Fallback&& fallback) -> decltype(body())
    {
        if (pending_ && !bestEffort_) {
            return fallback();
        }
        try {
            return body();
        }
        catch (...) {
            if (bestEffort_) {
                report(std::current_exception());
            }
            else {
                pending_ = std::current_exception();
            }
            return fallback();
        }
    }

    template <typename Body, typename R>
    R call(Body&& body, R fallbackValue)
    {
        return callOr(std::forward<Body>(body), [&]() -> R { return fallbackValue; });
    }

    template <typename Body>
    void call(Body&& body)
    {
        callOr(std::forward<Body>(body), [] {});
    }

    /* Drops the pending exception, if any, without rethrowing it. */
    void discard() { pending_ = nullptr; }

    /*
     * Switches to best-effort mode, for tearing the socket down: every callback is
     * still delivered, even after one has thrown, and each exception is reported
     * (ExceptionDescribe) and dropped rather than held. Used when there is no caller
     * to rethrow to, so that every timer gets its stop().
     */
    void beginBestEffort()
    {
        bestEffort_ = true;
        /* An exception still held from before has no caller left to go to: report it now. */
        if (pending_) {
            report(std::exchange(pending_, nullptr));
        }
    }

    /* Rethrows and clears the pending exception, if any. */
    void rethrow()
    {
        if (pending_) {
            std::exception_ptr ex = std::exchange(pending_, nullptr);
            std::rethrow_exception(ex);
        }
    }

private:
    static void report(std::exception_ptr ex) noexcept;

    std::exception_ptr pending_;
    bool bestEffort_ = false;
};

class WrappedSocketCallbacks: public dcsctp::DcSctpSocketCallbacks {
  public:
    WrappedSocketCallbacks(jDcSctpSocketCallbacks, CallbackExceptionSink& exceptions);
    virtual ~WrappedSocketCallbacks() = default;

    virtual void SendPacket(std::span<const uint8_t> data) override;

    virtual dcsctp::SendPacketStatus SendPacketWithStatus(
        std::span<const uint8_t> data) override;

    virtual std::unique_ptr<dcsctp::Timeout> CreateTimeout(
        webrtc::TaskQueueBase::DelayPrecision precision) override;

    virtual dcsctp::TimeMs TimeMillis() override;

    virtual webrtc::Timestamp Now() override;

    virtual uint32_t GetRandomInt(uint32_t low, uint32_t high) override;

    virtual void OnMessageReceived(dcsctp::DcSctpMessage message) override;

    virtual void OnMessageReady() override;

    virtual void OnError(dcsctp::ErrorKind error, absl::string_view message) override;

    virtual void OnAborted(dcsctp::ErrorKind error, absl::string_view message) override;

    virtual void OnConnected() override;

    virtual void OnClosed() override;
  
    virtual void OnConnectionRestarted() override;

    virtual void OnStreamsResetFailed(
        std::span<const dcsctp::StreamID> outgoing_streams,
        absl::string_view reason) override;

    virtual void OnStreamsResetPerformed(
        std::span<const dcsctp::StreamID> outgoing_streams) override;

    virtual void OnIncomingStreamsReset(
        std::span<const dcsctp::StreamID> incoming_streams) override;

    virtual void OnBufferedAmountLow(dcsctp::StreamID stream_id) override;

    virtual void OnTotalBufferedAmountLow() override;

    /* Not currently implementing lifecycle events, take the default no-op implementations. */
    
  private:
    DcSctpSocketCallbacks_class socketCallbacksClass;
    smjni::weak_java_ref<jDcSctpSocketCallbacks> wSocketCallbacks;
    CallbackExceptionSink& exceptions;
    /* Returned by Now() while an exception is pending, so time does not jump. */
    webrtc::Timestamp lastNow = webrtc::Timestamp::Zero();

    smjni::local_java_ref<jDcSctpSocketCallbacks> getObj(JNIEnv* env);
};

class WrappedPacketObserver: public dcsctp::PacketObserver {
 public:
    WrappedPacketObserver(jPacketObserver, CallbackExceptionSink& exceptions);
    virtual ~WrappedPacketObserver() = default;
    virtual void OnSentPacket(dcsctp::TimeMs now, std::span<const uint8_t> payload) override;
    virtual void OnReceivedPacket(dcsctp::TimeMs now, std::span<const uint8_t> payload) override;

  private:
    PacketObserver_class packetObserverClass;
    smjni::weak_java_ref<jPacketObserver> wJavaObserver;
    CallbackExceptionSink& exceptions;

    smjni::local_java_ref<jPacketObserver> getObj(JNIEnv* env);
};

class WrappedTimeout: public dcsctp::Timeout {
public:
    WrappedTimeout(jTimeout, CallbackExceptionSink& exceptions);
    virtual ~WrappedTimeout() = default;
    virtual void Start(dcsctp::DurationMs duration, dcsctp::TimeoutID timeout_id) override;
    virtual void Stop() override;
    virtual void Restart(dcsctp::DurationMs duration, dcsctp::TimeoutID timeout_id) override;

private:
    Timeout_class timeoutClass;
    smjni::global_java_ref<jTimeout> timeout;
    CallbackExceptionSink& exceptions;
    /* Whether a start() has been delivered to Java, so that stop() and restart()
       honour Timeout's promise that they only follow a start(). */
    bool started = false;
};

/* Handed to dcsctp when the Java createTimeout callback failed: it never fires. */
class NullTimeout: public dcsctp::Timeout {
public:
    virtual void Start(dcsctp::DurationMs, dcsctp::TimeoutID) override {}
    virtual void Stop() override {}
    virtual void Restart(dcsctp::DurationMs, dcsctp::TimeoutID) override {}
};

#endif
