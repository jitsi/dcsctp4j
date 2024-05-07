#include "wrapped_objects.h"
#include "dcsctp4j.h"

using namespace smjni;
using namespace dcsctp;
using namespace std;

WrappedPacketObserver::WrappedPacketObserver(jPacketObserver observer) :
    packetObserverClass(java_classes::get<PacketObserver_class>()),
    javaObserver(jglobal_ref(observer))
{
}

void WrappedPacketObserver::OnSentPacket(TimeMs now, rtc::ArrayView<const uint8_t> payload)
{
    JNIEnv* env = jni_provider::get_jni();
    
    auto jPayload = java_array_create<jbyte>(env, payload.size());
    auto aPayload = java_array_access(env, jPayload);
    copy(payload.cbegin(), payload.cend(), aPayload.begin());
    aPayload.commit();

    packetObserverClass.OnSentPacket(env, javaObserver, (jlong)*now, jPayload.c_ptr());
}

void WrappedPacketObserver::OnReceivedPacket(TimeMs now, rtc::ArrayView<const uint8_t> payload)
{
    JNIEnv* env = jni_provider::get_jni();
    
    auto jPayload = java_array_create<jbyte>(env, payload.size());
    auto aPayload = java_array_access(env, jPayload);
    copy(payload.cbegin(), payload.cend(), aPayload.begin());
    aPayload.commit();

    packetObserverClass.OnReceivedPacket(env, javaObserver, (jlong)*now, jPayload.c_ptr());
}


WrappedTimeout::WrappedTimeout(jTimeout timeout) :
    timeoutClass(java_classes::get<Timeout_class>()),
    timeout(jglobal_ref(timeout))
{
}

void WrappedTimeout::Start(DurationMs duration, TimeoutID timeout_id)
{
    JNIEnv* env = jni_provider::get_jni();

    timeoutClass.start(env, timeout, *duration, *timeout_id);
}


void WrappedTimeout::Stop()
{
    JNIEnv* env = jni_provider::get_jni();

    timeoutClass.stop(env, timeout);
}


void WrappedTimeout::Restart(DurationMs duration, TimeoutID timeout_id)
{
    JNIEnv* env = jni_provider::get_jni();

    timeoutClass.restart(env, timeout, *duration, *timeout_id);
}



WrappedSocketCallbacks::WrappedSocketCallbacks(jDcSctpSocketCallbacks callbacks) :
    socketCallbacksClass(java_classes::get<DcSctpSocketCallbacks_class>()),
    socketCallbacks(jglobal_ref(callbacks))
{
}


void WrappedSocketCallbacks::SendPacket(rtc::ArrayView<const uint8_t> data)
{
    JNIEnv* env = jni_provider::get_jni();

    auto jData = java_array_create<jbyte>(env, data.size());
    auto aData = java_array_access(env, jData);
    copy(data.cbegin(), data.cend(), aData.begin());
    aData.commit();

    socketCallbacksClass.sendPacket(env, socketCallbacks, jData.c_ptr());
}

SendPacketStatus WrappedSocketCallbacks::SendPacketWithStatus(
        rtc::ArrayView<const uint8_t> data)
{
    JNIEnv* env = jni_provider::get_jni();

    auto jData = java_array_create<jbyte>(env, data.size());
    auto aData = java_array_access(env, jData);
    copy(data.cbegin(), data.cend(), aData.begin());
    aData.commit();

    jint status = socketCallbacksClass.sendPacketWithStatus_(env, socketCallbacks, jData.c_ptr());

    return SendPacketStatus(status);
}

std::unique_ptr<Timeout> WrappedSocketCallbacks::CreateTimeout(
        webrtc::TaskQueueBase::DelayPrecision precision)
{
    JNIEnv* env = jni_provider::get_jni();

    auto timeout = socketCallbacksClass.createTimeout(env, socketCallbacks,
                                                      enum_members::get<DelayPrecision_members>().map(env, precision));

    return make_unique<WrappedTimeout>(timeout.c_ptr());
}

TimeMs WrappedSocketCallbacks::TimeMillis()
{
    JNIEnv* env = jni_provider::get_jni();

    return TimeMs(socketCallbacksClass.timeMillis(env, socketCallbacks));
}

webrtc::Timestamp WrappedSocketCallbacks::Now()
{
    JNIEnv* env = jni_provider::get_jni();

    auto instant = socketCallbacksClass.Now(env, socketCallbacks);

    return java_classes::get<Instant_class>().toTimestamp(env, instant);
}

uint32_t WrappedSocketCallbacks::GetRandomInt(uint32_t low, uint32_t high)
{
    JNIEnv* env = jni_provider::get_jni();

    return socketCallbacksClass.getRandomInt(env, socketCallbacks, low, high);
}

void WrappedSocketCallbacks::OnMessageReceived(DcSctpMessage message)
{
    JNIEnv* env = jni_provider::get_jni();

    auto payload = message.payload();
    auto jPayload = java_array_create<jbyte>(env, payload.size());
    auto aPayload = java_array_access(env, jPayload);
    copy(payload.cbegin(), payload.cend(), aPayload.begin());
    aPayload.commit();

    socketCallbacksClass.OnMessageReceived_(env, socketCallbacks, jPayload.c_ptr(), *message.ppid(), *message.stream_id());
}

void WrappedSocketCallbacks::OnError(ErrorKind error, absl::string_view message)
{
    JNIEnv* env = jni_provider::get_jni();

    auto jmessage = java_string_create(env, message.data(), message.size());

    socketCallbacksClass.OnError(env, socketCallbacks, enum_members::get<ErrorKind_members>().map(env, error), jmessage);
}

void WrappedSocketCallbacks::OnAborted(ErrorKind error, absl::string_view message)
{
    JNIEnv* env = jni_provider::get_jni();

    auto jmessage = java_string_create(env, message.data(), message.size());

    socketCallbacksClass.OnAborted(env, socketCallbacks, enum_members::get<ErrorKind_members>().map(env, error), jmessage);
}

void WrappedSocketCallbacks::OnConnected()
{
    JNIEnv* env = jni_provider::get_jni();

    socketCallbacksClass.OnConnected(env, socketCallbacks);
}

void WrappedSocketCallbacks::OnClosed()
{
    JNIEnv* env = jni_provider::get_jni();

    socketCallbacksClass.OnClosed(env, socketCallbacks);
}

void WrappedSocketCallbacks::OnConnectionRestarted()
{
    JNIEnv* env = jni_provider::get_jni();

    socketCallbacksClass.OnConnectionRestarted(env, socketCallbacks);
}

void WrappedSocketCallbacks::OnStreamsResetFailed(
        rtc::ArrayView<const StreamID> outgoing_streams,
        absl::string_view reason)
{
    JNIEnv* env = jni_provider::get_jni();

    auto jOutgoingStreams = java_array_create<jshort>(env, outgoing_streams.size());
    java_array_access streamsAccess(env, jOutgoingStreams.c_ptr());
    transform(outgoing_streams.cbegin(), outgoing_streams.cend(), streamsAccess.begin(), [](auto s) { return *s; });
    streamsAccess.commit();

    auto jreason = java_string_create(env, reason.data(), reason.size());

    socketCallbacksClass.OnStreamsResetFailed(env, socketCallbacks, jOutgoingStreams, jreason);
}

void WrappedSocketCallbacks::OnStreamsResetPerformed(
        rtc::ArrayView<const StreamID> outgoing_streams)
{
    JNIEnv* env = jni_provider::get_jni();

    auto jOutgoingStreams = java_array_create<jshort>(env, outgoing_streams.size());
    java_array_access streamsAccess(env, jOutgoingStreams.c_ptr());
    transform(outgoing_streams.cbegin(), outgoing_streams.cend(), streamsAccess.begin(), [](auto s) { return *s; });
    streamsAccess.commit();

    socketCallbacksClass.OnStreamsResetPerformed(env, socketCallbacks, jOutgoingStreams);
}

void WrappedSocketCallbacks::OnIncomingStreamsReset(
        rtc::ArrayView<const StreamID> incoming_streams)
{
    JNIEnv* env = jni_provider::get_jni();

    auto jIncomingStreams = java_array_create<jshort>(env, incoming_streams.size());
    java_array_access streamsAccess(env, jIncomingStreams.c_ptr());
    transform(incoming_streams.cbegin(), incoming_streams.cend(), streamsAccess.begin(), [](auto s) { return *s; });
    streamsAccess.commit();

    socketCallbacksClass.OnStreamsResetPerformed(env, socketCallbacks, jIncomingStreams);

}

void WrappedSocketCallbacks::OnBufferedAmountLow(StreamID stream_id)
{
    JNIEnv* env = jni_provider::get_jni();

    socketCallbacksClass.OnBufferedAmountLow(env, socketCallbacks, *stream_id);
}

void WrappedSocketCallbacks::OnTotalBufferedAmountLow()
{
    JNIEnv* env = jni_provider::get_jni();

    socketCallbacksClass.OnTotalBufferedAmountLow(env, socketCallbacks);
}
