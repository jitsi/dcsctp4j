#ifndef WRAPPED_OBJECTS_H_
#define WRAPPED_OBJECTS_H_

#include <smjni/smjni.h>
#include "all_classes.h"
#include <net/dcsctp/public/dcsctp_socket.h>

class WrappedSocketCallbacks: public dcsctp::DcSctpSocketCallbacks {
  public:
    WrappedSocketCallbacks(jDcSctpSocketCallbacks);
    virtual ~WrappedSocketCallbacks() = default;

    virtual void SendPacket(rtc::ArrayView<const uint8_t> data) override;

    virtual dcsctp::SendPacketStatus SendPacketWithStatus(
        rtc::ArrayView<const uint8_t> data) override;

    virtual std::unique_ptr<dcsctp::Timeout> CreateTimeout(
        webrtc::TaskQueueBase::DelayPrecision precision) override;

    virtual dcsctp::TimeMs TimeMillis() override;

    virtual webrtc::Timestamp Now() override;

    virtual uint32_t GetRandomInt(uint32_t low, uint32_t high) override;

    virtual void OnMessageReceived(dcsctp::DcSctpMessage message) override;

    virtual void OnError(dcsctp::ErrorKind error, absl::string_view message) override;

    virtual void OnAborted(dcsctp::ErrorKind error, absl::string_view message) override;

    virtual void OnConnected() override;

    virtual void OnClosed() override;
  
    virtual void OnConnectionRestarted() override;

    virtual void OnStreamsResetFailed(
        rtc::ArrayView<const dcsctp::StreamID> outgoing_streams,
        absl::string_view reason) override;

    virtual void OnStreamsResetPerformed(
        rtc::ArrayView<const dcsctp::StreamID> outgoing_streams) override;

    virtual void OnIncomingStreamsReset(
        rtc::ArrayView<const dcsctp::StreamID> incoming_streams) override;

    virtual void OnBufferedAmountLow(dcsctp::StreamID stream_id) override;

    virtual void OnTotalBufferedAmountLow() override;

    /* Not currently implementing lifecycle events, take the default no-op implementations. */
    
  private:
    DcSctpSocketCallbacks_class socketCallbacksClass;
    smjni::global_java_ref<jDcSctpSocketCallbacks> socketCallbacks;
};

class WrappedPacketObserver: public dcsctp::PacketObserver {
 public:
    WrappedPacketObserver(jPacketObserver);
    virtual ~WrappedPacketObserver() = default;
    virtual void OnSentPacket(dcsctp::TimeMs now, rtc::ArrayView<const uint8_t> payload) override;
    virtual void OnReceivedPacket(dcsctp::TimeMs now, rtc::ArrayView<const uint8_t> payload) override;

  private:
    PacketObserver_class packetObserverClass;
    smjni::global_java_ref<jPacketObserver> javaObserver;
};

#endif
