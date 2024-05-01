#include "wrapped_objects.h"

using namespace smjni;
using namespace dcsctp;

using java_classes = smjni::java_class_table<JNIGEN_ALL_GENERATED_CLASSES>;

WrappedPacketObserver::WrappedPacketObserver(jPacketObserver observer) :
    packetObserverClass(java_classes::get<PacketObserver_class>()),
    javaObserver(jglobal_ref(observer))
{
}

void WrappedPacketObserver::OnSentPacket(TimeMs now, rtc::ArrayView<const uint8_t> payload)
{
    JNIEnv* env = jni_provider::get_jni();
    
    java_direct_buffer<uint8_t> byteBufferPayload(const_cast<uint8_t*>(payload.data()), payload.size());
    packetObserverClass.OnSentPacket(env, javaObserver, (jlong)*now, byteBufferPayload.to_java(env));
}

void WrappedPacketObserver::OnReceivedPacket(TimeMs now, rtc::ArrayView<const uint8_t> payload)
{
    JNIEnv* env = jni_provider::get_jni();
    
    java_direct_buffer<uint8_t> byteBufferPayload(const_cast<uint8_t*>(payload.data()), payload.size());
    packetObserverClass.OnReceivedPacket(env, javaObserver, (jlong)*now, byteBufferPayload.to_java(env));
}
