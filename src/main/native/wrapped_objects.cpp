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

#include "wrapped_objects.h"
#include "dcsctp4j.h"

using namespace smjni;
using namespace dcsctp;
using namespace std;

WrappedPacketObserver::WrappedPacketObserver(jPacketObserver observer, CallbackExceptionSink& exceptions_) :
    packetObserverClass(java_classes::get<PacketObserver_class>()),
    wJavaObserver(jweak_ref(observer)),
    exceptions(exceptions_)
{
}

local_java_ref<jPacketObserver> WrappedPacketObserver::getObj(JNIEnv *env)
{
    local_java_ref<jPacketObserver> obj(wJavaObserver);

    if (!obj) {
        auto ex = java_runtime::throwable().ctor(env, java_string_create(env, "Weak PacketObserver reference garbage collected"));
        throw java_exception(ex);
    }
    return obj;
}

void WrappedPacketObserver::OnSentPacket(TimeMs now, std::span<const uint8_t> payload)
{
    exceptions.call([&] {
        JNIEnv* env = jni_provider::get_jni();

        auto javaObserver = getObj(env);
    
        auto jPayload = java_array_create<jbyte>(env, payload.begin(), payload.end());

        packetObserverClass.OnSentPacket(env, javaObserver, (jlong)*now, jPayload.c_ptr());
    });
}

void WrappedPacketObserver::OnReceivedPacket(TimeMs now, std::span<const uint8_t> payload)
{
    exceptions.call([&] {
        JNIEnv* env = jni_provider::get_jni();

        auto javaObserver = getObj(env);
    
        auto jPayload = java_array_create<jbyte>(env, payload.begin(), payload.end());

        packetObserverClass.OnReceivedPacket(env, javaObserver, (jlong)*now, jPayload.c_ptr());
    });
}


WrappedTimeout::WrappedTimeout(jTimeout timeout, CallbackExceptionSink& exceptions_) :
    timeoutClass(java_classes::get<Timeout_class>()),
    timeout(jglobal_ref(timeout)),
    exceptions(exceptions_)
{
}

void WrappedTimeout::Start(DurationMs duration, TimeoutID timeout_id)
{
    exceptions.call([&] {
        JNIEnv* env = jni_provider::get_jni();

        timeoutClass.start(env, timeout, *duration, *timeout_id);
        started = true;
    });
}


void WrappedTimeout::Stop()
{
    /* If start() never reached Java (suppressed after an earlier callback threw),
       don't deliver an unpaired stop(). */
    if (!started) {
        return;
    }
    exceptions.call([&] {
        JNIEnv* env = jni_provider::get_jni();

        timeoutClass.stop(env, timeout);
        started = false;
    });
}


void WrappedTimeout::Restart(DurationMs duration, TimeoutID timeout_id)
{
    if (!started) {
        Start(duration, timeout_id);
        return;
    }
    exceptions.call([&] {
        JNIEnv* env = jni_provider::get_jni();

        timeoutClass.restart(env, timeout, *duration, *timeout_id);
    });
}



WrappedSocketCallbacks::WrappedSocketCallbacks(jDcSctpSocketCallbacks callbacks, CallbackExceptionSink& exceptions_) :
    socketCallbacksClass(java_classes::get<DcSctpSocketCallbacks_class>()),
    wSocketCallbacks(jweak_ref(callbacks)),
    exceptions(exceptions_)
{
    /* Seed the fallback for Now() with a real time, so that an exception from the
       very first callback does not leave the library seeing the epoch. */
    Now();
}

local_java_ref<jDcSctpSocketCallbacks> WrappedSocketCallbacks::getObj(JNIEnv *env)
{
    local_java_ref<jDcSctpSocketCallbacks> obj(wSocketCallbacks);

    if (!obj) {
        auto ex = java_runtime::throwable().ctor(env, java_string_create(env, "Weak DcSctpSocketCallbacks reference garbage collected"));
        throw java_exception(ex);
    }
    return obj;
}


void WrappedSocketCallbacks::SendPacket(std::span<const uint8_t> data)
{
    SendPacketWithStatus(data);
}

SendPacketStatus WrappedSocketCallbacks::SendPacketWithStatus(
        std::span<const uint8_t> data)
{
    return exceptions.call([&] {
        JNIEnv* env = jni_provider::get_jni();

        auto socketCallbacks = getObj(env);

        auto jData = java_array_create<jbyte>(env, data.begin(), data.end());

        jint status = socketCallbacksClass.sendPacketWithStatus_(env, socketCallbacks, jData.c_ptr());

        return SendPacketStatus(status);
    }, SendPacketStatus::kError);
}

std::unique_ptr<Timeout> WrappedSocketCallbacks::CreateTimeout(
        webrtc::TaskQueueBase::DelayPrecision precision)
{
    return exceptions.callOr([&]() -> std::unique_ptr<Timeout> {
        JNIEnv* env = jni_provider::get_jni();

        auto socketCallbacks = getObj(env);

        auto timeout = socketCallbacksClass.createTimeout(env, socketCallbacks,
                                                          enum_members::get<DelayPrecision_members>().map(env, precision));

        return make_unique<WrappedTimeout>(timeout.c_ptr(), exceptions);
    }, []() -> std::unique_ptr<Timeout> { return make_unique<NullTimeout>(); });
}

TimeMs WrappedSocketCallbacks::TimeMillis()
{
    return TimeMs(Now().ms());
}

webrtc::Timestamp WrappedSocketCallbacks::Now()
{
    return exceptions.call([&] {
        JNIEnv* env = jni_provider::get_jni();

        auto socketCallbacks = getObj(env);

        auto instant = socketCallbacksClass.Now(env, socketCallbacks);

        lastNow = java_classes::get<Instant_class>().toTimestamp(env, instant);
        return lastNow;
    }, lastNow);
}

uint32_t WrappedSocketCallbacks::GetRandomInt(uint32_t low, uint32_t high)
{
    return exceptions.call([&]() -> uint32_t {
        JNIEnv* env = jni_provider::get_jni();

        auto socketCallbacks = getObj(env);

        return socketCallbacksClass.getRandomInt(env, socketCallbacks, low, high);
    }, low);
}

void WrappedSocketCallbacks::OnMessageReceived(DcSctpMessage message)
{
    exceptions.call([&] {
        JNIEnv* env = jni_provider::get_jni();

        auto socketCallbacks = getObj(env);

        auto payload = message.payload();
        auto jPayload = java_array_create<jbyte>(env, payload.begin(), payload.end());

        socketCallbacksClass.OnMessageReceived_(env, socketCallbacks, jPayload.c_ptr(), *message.ppid(), *message.stream_id());
    });
}

void WrappedSocketCallbacks::OnMessageReady()
{
    exceptions.call([&] {
        JNIEnv* env = jni_provider::get_jni();

        auto socketCallbacks = getObj(env);

        socketCallbacksClass.OnMessageReady(env, socketCallbacks);
    });
}

void WrappedSocketCallbacks::OnError(ErrorKind error, absl::string_view message)
{
    exceptions.call([&] {
        JNIEnv* env = jni_provider::get_jni();

        auto socketCallbacks = getObj(env);

        auto jmessage = java_string_create(env, message.data(), message.size());

        socketCallbacksClass.OnError(env, socketCallbacks, enum_members::get<ErrorKind_members>().map(env, error), jmessage);
    });
}

void WrappedSocketCallbacks::OnAborted(ErrorKind error, absl::string_view message)
{
    exceptions.call([&] {
        JNIEnv* env = jni_provider::get_jni();

        auto socketCallbacks = getObj(env);

        auto jmessage = java_string_create(env, message.data(), message.size());

        socketCallbacksClass.OnAborted(env, socketCallbacks, enum_members::get<ErrorKind_members>().map(env, error), jmessage);
    });
}

void WrappedSocketCallbacks::OnConnected()
{
    exceptions.call([&] {
        JNIEnv* env = jni_provider::get_jni();

        auto socketCallbacks = getObj(env);

        socketCallbacksClass.OnConnected(env, socketCallbacks);
    });
}

void WrappedSocketCallbacks::OnClosed()
{
    exceptions.call([&] {
        JNIEnv* env = jni_provider::get_jni();

        auto socketCallbacks = getObj(env);

        socketCallbacksClass.OnClosed(env, socketCallbacks);
    });
}

void WrappedSocketCallbacks::OnConnectionRestarted()
{
    exceptions.call([&] {
        JNIEnv* env = jni_provider::get_jni();

        auto socketCallbacks = getObj(env);

        socketCallbacksClass.OnConnectionRestarted(env, socketCallbacks);
    });
}

void WrappedSocketCallbacks::OnStreamsResetFailed(
        std::span<const StreamID> outgoing_streams,
        absl::string_view reason)
{
    exceptions.call([&] {
        JNIEnv* env = jni_provider::get_jni();

        auto socketCallbacks = getObj(env);

        auto jOutgoingStreams = java_array_create<jshort>(env, outgoing_streams.size());
        java_array_access streamsAccess(env, jOutgoingStreams.c_ptr());
        transform(outgoing_streams.begin(), outgoing_streams.end(), streamsAccess.begin(), [](auto s) { return *s; });
        streamsAccess.commit();

        auto jreason = java_string_create(env, reason.data(), reason.size());

        socketCallbacksClass.OnStreamsResetFailed(env, socketCallbacks, jOutgoingStreams, jreason);
    });
}

void WrappedSocketCallbacks::OnStreamsResetPerformed(
        std::span<const StreamID> outgoing_streams)
{
    exceptions.call([&] {
        JNIEnv* env = jni_provider::get_jni();

        auto socketCallbacks = getObj(env);

        auto jOutgoingStreams = java_array_create<jshort>(env, outgoing_streams.size());
        java_array_access streamsAccess(env, jOutgoingStreams.c_ptr());
        transform(outgoing_streams.begin(), outgoing_streams.end(), streamsAccess.begin(), [](auto s) { return *s; });
        streamsAccess.commit();

        socketCallbacksClass.OnStreamsResetPerformed(env, socketCallbacks, jOutgoingStreams);
    });
}

void WrappedSocketCallbacks::OnIncomingStreamsReset(
        std::span<const StreamID> incoming_streams)
{
    exceptions.call([&] {
        JNIEnv* env = jni_provider::get_jni();

        auto socketCallbacks = getObj(env);

        auto jIncomingStreams = java_array_create<jshort>(env, incoming_streams.size());
        java_array_access streamsAccess(env, jIncomingStreams.c_ptr());
        transform(incoming_streams.begin(), incoming_streams.end(), streamsAccess.begin(), [](auto s) { return *s; });
        streamsAccess.commit();

        socketCallbacksClass.OnIncomingStreamsReset(env, socketCallbacks, jIncomingStreams);
    });
}

void WrappedSocketCallbacks::OnBufferedAmountLow(StreamID stream_id)
{
    exceptions.call([&] {
        JNIEnv* env = jni_provider::get_jni();

        auto socketCallbacks = getObj(env);

        socketCallbacksClass.OnBufferedAmountLow(env, socketCallbacks, *stream_id);
    });
}

void WrappedSocketCallbacks::OnTotalBufferedAmountLow()
{
    exceptions.call([&] {
        JNIEnv* env = jni_provider::get_jni();

        auto socketCallbacks = getObj(env);

        socketCallbacksClass.OnTotalBufferedAmountLow(env, socketCallbacks);
    });
}
