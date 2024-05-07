#include <smjni/smjni.h>
#include "all_classes.h"
#include <net/dcsctp/public/dcsctp_socket.h>
#include "native_sctp_socket.h"
#include "dcsctp4j.h"

using namespace smjni;
using namespace dcsctp;
using namespace std;

NativeSctpSocket::NativeSctpSocket(jDcSctpSocketCallbacks jCallbacks) :
    callbacks(jCallbacks)
{
}


void JNICALL DcSctpSocketFactory_NativeSctpSocket_class::receivePacket_(JNIEnv* env, jDcSctpSocketFactory_NativeSctpSocket, jlong ptr, jByteBuffer data)
{
NATIVE_PROLOG
    auto nativeSocket = (NativeSctpSocket*)(intptr_t)ptr;
    auto bData = java_direct_buffer<const uint8_t>(env, data);
    auto vData = rtc::ArrayView<const uint8_t>(bData.data(), bData.size());
    nativeSocket->socket->ReceivePacket(vData);
NATIVE_EPILOG
}


void JNICALL DcSctpSocketFactory_NativeSctpSocket_class::handleTimeout_(JNIEnv* env, jDcSctpSocketFactory_NativeSctpSocket, jlong ptr, jlong timeoutId)
{
NATIVE_PROLOG
    auto nativeSocket = (NativeSctpSocket*)(intptr_t)ptr;
    nativeSocket->socket->HandleTimeout(TimeoutID(timeoutId));
NATIVE_EPILOG
}


void JNICALL DcSctpSocketFactory_NativeSctpSocket_class::connect_(JNIEnv* env, jDcSctpSocketFactory_NativeSctpSocket, jlong ptr)
{
NATIVE_PROLOG
    auto nativeSocket = (NativeSctpSocket*)(intptr_t)ptr;
    nativeSocket->socket->Connect();
NATIVE_EPILOG
}


void JNICALL DcSctpSocketFactory_NativeSctpSocket_class::shutdown_(JNIEnv* env, jDcSctpSocketFactory_NativeSctpSocket, jlong ptr)
{
NATIVE_PROLOG
    auto nativeSocket = (NativeSctpSocket*)(intptr_t)ptr;
    nativeSocket->socket->Shutdown();
NATIVE_EPILOG
}


void JNICALL DcSctpSocketFactory_NativeSctpSocket_class::close_(JNIEnv* env, jDcSctpSocketFactory_NativeSctpSocket, jlong ptr)
{
NATIVE_PROLOG
    auto nativeSocket = (NativeSctpSocket*)(intptr_t)ptr;
    nativeSocket->socket->Close();
NATIVE_EPILOG
}


jSocketState JNICALL DcSctpSocketFactory_NativeSctpSocket_class::state_(JNIEnv* env, jDcSctpSocketFactory_NativeSctpSocket, jlong ptr)
{
NATIVE_PROLOG
    auto nativeSocket = (NativeSctpSocket*)(intptr_t)ptr;
    auto status = nativeSocket->socket->state();
    return enum_members::get<SocketState_members>().map(env, status);
NATIVE_EPILOG_Z
}


jDcSctpOptions JNICALL DcSctpSocketFactory_NativeSctpSocket_class::options_(JNIEnv* env, jDcSctpSocketFactory_NativeSctpSocket, jlong ptr)
{
NATIVE_PROLOG
    auto nativeSocket = (NativeSctpSocket*)(intptr_t)ptr;
    auto options = make_unique<DcSctpOptions>(nativeSocket->socket->options());
    auto jOptions = java_classes::get<DcSctpOptions_class>().ctor(env, (jlong)(intptr_t)options.release());
    return jOptions.release();
NATIVE_EPILOG_Z
}


void JNICALL DcSctpSocketFactory_NativeSctpSocket_class::setStreamPriority_(JNIEnv* env, jDcSctpSocketFactory_NativeSctpSocket, jlong ptr, jshort streamId, jshort streamPriority)
{
NATIVE_PROLOG
    auto nativeSocket = (NativeSctpSocket*)(intptr_t)ptr;
    nativeSocket->socket->SetStreamPriority(StreamID(streamId), StreamPriority(streamPriority));
NATIVE_EPILOG
}


jshort JNICALL DcSctpSocketFactory_NativeSctpSocket_class::getStreamPriority_(JNIEnv* env, jDcSctpSocketFactory_NativeSctpSocket, jlong ptr, jshort streamId)
{
NATIVE_PROLOG
    auto nativeSocket = (NativeSctpSocket*)(intptr_t)ptr;
    return *nativeSocket->socket->GetStreamPriority(StreamID(streamId));
NATIVE_EPILOG_Z
}


jint JNICALL DcSctpSocketFactory_NativeSctpSocket_class::send_(JNIEnv* env, jDcSctpSocketFactory_NativeSctpSocket, jlong ptr, jByteBuffer payload, jint ppid, jshort streamID, jboolean isUnordered, jLong lifetime, jLong maxRetransmissions, jlong lifecycleId)
{
NATIVE_PROLOG
    auto nativeSocket = (NativeSctpSocket*)(intptr_t)ptr;

    auto bPayload = java_direct_buffer<const uint8_t>(env, payload);
    vector<uint8_t> vPayload(bPayload.begin(), bPayload.end());

    auto message = DcSctpMessage(StreamID(streamID), PPID(ppid), vPayload);

    SendOptions options;

    options.unordered = IsUnordered(isUnordered ? true : false);

    auto longClass = java_classes::get<Long_class>();
    if (lifetime != nullptr) {
        options.lifetime = std::optional(DurationMs(longClass.longValue(env, lifetime)));
    }
    if (maxRetransmissions != nullptr) {
        options.max_retransmissions = std::optional<size_t>(longClass.longValue(env, maxRetransmissions));
    }
    options.lifecycle_id = LifecycleId(lifecycleId);

    return (jint)nativeSocket->socket->Send(std::move(message), options);
NATIVE_EPILOG_Z
}


jintArray JNICALL DcSctpSocketFactory_NativeSctpSocket_class::sendMany_(JNIEnv* env, jDcSctpSocketFactory_NativeSctpSocket, jlong ptr, jDcSctpMessageArray messages, jSendOptions joptions)
{
NATIVE_PROLOG
    auto nativeSocket = (NativeSctpSocket*)(intptr_t)ptr;

    java_array_access jmessages(env, messages);
    auto vmessages = vector<DcSctpMessage>();
    vmessages.reserve(jmessages.size());

    transform(jmessages.cbegin(), jmessages.cend(), back_inserter(vmessages), [env](auto m) { return convertDcSctpMessage(env, m.c_ptr()); });

    auto options = convertSendOptions(env, joptions);

    auto statuses = nativeSocket->socket->SendMany(vmessages, options);

    auto statusArray = java_array_create<jint>(env, statuses.size());
    java_array_access statusArrayAccess(env, statusArray.c_ptr());
    transform(statuses.cbegin(), statuses.cend(), statusArrayAccess.begin(), [](auto e) { return (int)e; });
    statusArrayAccess.commit(env);
    return statusArray.release();;
NATIVE_EPILOG_Z
}


jResetStreamsStatus JNICALL DcSctpSocketFactory_NativeSctpSocket_class::resetStreams_(JNIEnv* env, jDcSctpSocketFactory_NativeSctpSocket, jlong ptr, jshortArray outgoingStreams)
{
NATIVE_PROLOG
    auto nativeSocket = (NativeSctpSocket*)(intptr_t)ptr;
    java_array_access jstreams(env, outgoingStreams);
    auto vstreams = vector<StreamID>();
    vstreams.reserve(jstreams.size());

    transform(jstreams.cbegin(), jstreams.cend(), back_inserter(vstreams), [](auto s) { return StreamID(s); });

    auto status = nativeSocket->socket->ResetStreams(vstreams);

    return enum_members::get<ResetStreamsStatus_members>().map(env, status);
NATIVE_EPILOG_Z
}


jlong JNICALL DcSctpSocketFactory_NativeSctpSocket_class::bufferedAmount_(JNIEnv* env, jDcSctpSocketFactory_NativeSctpSocket, jlong ptr, jshort streamId)
{
NATIVE_PROLOG
    auto nativeSocket = (NativeSctpSocket*)(intptr_t)ptr;

    return nativeSocket->socket->buffered_amount(StreamID(streamId));
NATIVE_EPILOG_Z
}


jlong JNICALL DcSctpSocketFactory_NativeSctpSocket_class::bufferedAmountLowThreshold_(JNIEnv* env, jDcSctpSocketFactory_NativeSctpSocket, jlong ptr, jshort streamId)
{
NATIVE_PROLOG
    auto nativeSocket = (NativeSctpSocket*)(intptr_t)ptr;

    return nativeSocket->socket->buffered_amount_low_threshold(StreamID(streamId));
NATIVE_EPILOG_Z
}


void JNICALL DcSctpSocketFactory_NativeSctpSocket_class::setBufferedAmountLowThreshold_(JNIEnv* env, jDcSctpSocketFactory_NativeSctpSocket, jlong ptr, jshort streamId, jlong bytes)
{
NATIVE_PROLOG
    auto nativeSocket = (NativeSctpSocket*)(intptr_t)ptr;

    nativeSocket->socket->SetBufferedAmountLowThreshold(StreamID(streamId), bytes);
NATIVE_EPILOG
}


jMetrics JNICALL DcSctpSocketFactory_NativeSctpSocket_class::getMetrics_(JNIEnv* env, jDcSctpSocketFactory_NativeSctpSocket, jlong ptr)
{
NATIVE_PROLOG
    /* TODO */
NATIVE_EPILOG_Z
}


DcSctpMessage convertDcSctpMessage(JNIEnv* env, jDcSctpMessage jmessage)
{
    auto messageClass = java_classes::get<DcSctpMessage_class>();

    auto payload = messageClass.getPayload(env, jmessage);
    auto bPayload = java_direct_buffer<const uint8_t>(env, payload);
    vector<uint8_t> vPayload(bPayload.begin(), bPayload.end());

    auto streamID = messageClass.getStreamID(env, jmessage);
    auto ppid = messageClass.getPpid(env, jmessage);

    auto message = DcSctpMessage(StreamID(streamID), PPID(ppid), vPayload);
    return message;
}

SendOptions convertSendOptions(JNIEnv* env, jSendOptions jOptions)
{
    auto optionsClass = java_classes::get<SendOptions_class>();
    auto longClass = java_classes::get<Long_class>();

    auto isUnordered = optionsClass.get_isUnordered(env, jOptions);
    auto lifetime = optionsClass.get_lifetime(env, jOptions).c_ptr();
    auto maxRetransmissions = optionsClass.get_maxRetransmissions(env, jOptions).c_ptr();
    auto lifecycleId = optionsClass.get_lifecycleId(env, jOptions);

    SendOptions options;
    options.unordered = IsUnordered(isUnordered ? true : false);

    if (lifetime != nullptr) {
        options.lifetime = std::optional(DurationMs(longClass.longValue(env, lifetime)));
    }
    if (maxRetransmissions != nullptr) {
        options.max_retransmissions = std::optional<size_t>(longClass.longValue(env, maxRetransmissions));
    }
    options.lifecycle_id = LifecycleId(lifecycleId);

    return options;
}
