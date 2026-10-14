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

#include <smjni/smjni.h>
#include <span>
#include "all_classes.h"
#include <net/dcsctp/public/dcsctp_socket.h>
#include "native_sctp_socket.h"
#include "dcsctp4j.h"

using namespace smjni;
using namespace dcsctp;
using namespace std;

NativeSctpSocket::NativeSctpSocket(jDcSctpSocketCallbacks jCallbacks) :
    callbacks(jCallbacks, exceptions)
{
}

void JNICALL DcSctpSocketFactory_NativeSctpSocket_class::destruct(JNIEnv *env, jclass, jlong ptr)
{
NATIVE_PROLOG
    std::unique_ptr<NativeSctpSocket> nativeSocket((NativeSctpSocket*)(intptr_t)ptr);
    /* Destroying the dcsctp socket stops its timers, which calls back into Java. Tear
       it down first so that an exception from those callbacks is reported rather than
       destroyed along with the sink; the unique_ptr frees the rest either way. */
    nativeSocket->socket.reset();
    nativeSocket->exceptions.rethrow();
NATIVE_EPILOG
}

void JNICALL DcSctpSocketFactory_NativeSctpSocket_class::receivePacket_(JNIEnv* env, jDcSctpSocketFactory_NativeSctpSocket, jlong ptr, jbyteArray data, jint offset, jint length)
{
SOCKET_PROLOG(ptr)
    java_array_access aData(env, data);
    /* Validate without addition so nothing can overflow: once offset and length are
       both non-negative, size - offset cannot overflow, and length > size - offset
       also covers offset > size. */
    const jsize size = aData.size();
    if (offset < 0 || length < 0 || length > size - offset) {
        auto ex = java_runtime::throwable().ctor(env, java_string_create(env, "Invalid values for array length"));
        throw java_exception(ex);
    }
    /* It's safe to expose the java array data directly for the duration of this call,
       because dcsctp will make a copy of anything it needs. */
    auto dataView = std::span<const uint8_t>(reinterpret_cast<const uint8_t*>(aData.data()) + offset,
                                             static_cast<size_t>(length));
    nativeSocket->socket->ReceivePacket(dataView);
NATIVE_EPILOG
}

jlong JNICALL DcSctpSocketFactory_NativeSctpSocket_class::messagesReady_(JNIEnv* env, jDcSctpSocketFactory_NativeSctpSocket, jlong ptr)
{
SOCKET_PROLOG(ptr)
    return nativeSocket->socket->MessagesReady();
NATIVE_EPILOG_Z
}

jDcSctpMessage JNICALL DcSctpSocketFactory_NativeSctpSocket_class::getNextMessage_(JNIEnv* env, jDcSctpSocketFactory_NativeSctpSocket, jlong ptr)
{
SOCKET_PROLOG(ptr)
    auto msg = nativeSocket->socket->GetNextMessage();
    if (!msg) {
        return nullptr;
    }

    return convertDcSctpMessage(env, *msg).release();
NATIVE_EPILOG_Z
}

void JNICALL DcSctpSocketFactory_NativeSctpSocket_class::handleTimeout_(JNIEnv* env, jDcSctpSocketFactory_NativeSctpSocket, jlong ptr, jlong timeoutId)
{
SOCKET_PROLOG(ptr)
    nativeSocket->socket->HandleTimeout(TimeoutID(timeoutId));
NATIVE_EPILOG
}

void JNICALL DcSctpSocketFactory_NativeSctpSocket_class::connect_(JNIEnv* env, jDcSctpSocketFactory_NativeSctpSocket, jlong ptr)
{
SOCKET_PROLOG(ptr)
    nativeSocket->socket->Connect();
NATIVE_EPILOG
}

jboolean JNICALL DcSctpSocketFactory_NativeSctpSocket_class::connectWithConnectionToken_(JNIEnv* env, jDcSctpSocketFactory_NativeSctpSocket, jlong ptr, jbyteArray myData, jbyteArray peerData)
{
SOCKET_PROLOG(ptr)
    java_array_access aMyData(env, myData);
    java_array_access aPeerData(env, peerData);
    /* As with receivePacket_, dcsctp copies what it needs, so the Java arrays can be exposed directly. */
    auto mySpan = std::span<const uint8_t>(reinterpret_cast<const uint8_t*>(aMyData.data()), aMyData.size());
    auto peerSpan = std::span<const uint8_t>(reinterpret_cast<const uint8_t*>(aPeerData.data()), aPeerData.size());
    return nativeSocket->socket->ConnectWithConnectionToken(mySpan, peerSpan) ? java_true : java_false;
NATIVE_EPILOG_Z
}

void JNICALL DcSctpSocketFactory_NativeSctpSocket_class::shutdown_(JNIEnv* env, jDcSctpSocketFactory_NativeSctpSocket, jlong ptr)
{
SOCKET_PROLOG(ptr)
    nativeSocket->socket->Shutdown();
NATIVE_EPILOG
}

void JNICALL DcSctpSocketFactory_NativeSctpSocket_class::close_(JNIEnv* env, jDcSctpSocketFactory_NativeSctpSocket, jlong ptr)
{
SOCKET_PROLOG(ptr)
    nativeSocket->socket->Close();
NATIVE_EPILOG
}

jSocketState JNICALL DcSctpSocketFactory_NativeSctpSocket_class::state_(JNIEnv* env, jDcSctpSocketFactory_NativeSctpSocket, jlong ptr)
{
SOCKET_PROLOG(ptr)
    auto status = nativeSocket->socket->state();
    return enum_members::get<SocketState_members>().map(env, status);
NATIVE_EPILOG_Z
}

jDcSctpOptions JNICALL DcSctpSocketFactory_NativeSctpSocket_class::options_(JNIEnv* env, jDcSctpSocketFactory_NativeSctpSocket, jlong ptr)
{
SOCKET_PROLOG(ptr)
    auto options = make_unique<DcSctpOptions>(nativeSocket->socket->options());
    auto jOptions = java_classes::get<DcSctpOptions_class>().ctor(env, (jlong)(intptr_t)options.release());
    return jOptions.release();
NATIVE_EPILOG_Z
}

void JNICALL DcSctpSocketFactory_NativeSctpSocket_class::setStreamPriority_(JNIEnv* env, jDcSctpSocketFactory_NativeSctpSocket, jlong ptr, jshort streamId, jshort streamPriority)
{
SOCKET_PROLOG(ptr)
    nativeSocket->socket->SetStreamPriority(StreamID(streamId), StreamPriority(streamPriority));
NATIVE_EPILOG
}

jshort JNICALL DcSctpSocketFactory_NativeSctpSocket_class::getStreamPriority_(JNIEnv* env, jDcSctpSocketFactory_NativeSctpSocket, jlong ptr, jshort streamId)
{
SOCKET_PROLOG(ptr)
    return *nativeSocket->socket->GetStreamPriority(StreamID(streamId));
NATIVE_EPILOG_Z
}

jint JNICALL DcSctpSocketFactory_NativeSctpSocket_class::send_(JNIEnv* env, jDcSctpSocketFactory_NativeSctpSocket, jlong ptr, jbyteArray payload, jint ppid, jshort streamID, jboolean isUnordered, jLong lifetime, jLong maxRetransmissions, jlong lifecycleId)
{
SOCKET_PROLOG(ptr)

    java_array_access aPayload(env, payload);
    vector<uint8_t> vPayload(aPayload.cbegin(), aPayload.cend());

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

jlong JNICALL DcSctpSocketFactory_NativeSctpSocket_class::bufferedAmount_(JNIEnv* env, jDcSctpSocketFactory_NativeSctpSocket, jlong ptr, jshort streamId)
{
SOCKET_PROLOG(ptr)

    return nativeSocket->socket->buffered_amount(StreamID(streamId));
NATIVE_EPILOG_Z
}

jlong JNICALL DcSctpSocketFactory_NativeSctpSocket_class::bufferedAmountLowThreshold_(JNIEnv* env, jDcSctpSocketFactory_NativeSctpSocket, jlong ptr, jshort streamId)
{
SOCKET_PROLOG(ptr)

    return nativeSocket->socket->buffered_amount_low_threshold(StreamID(streamId));
NATIVE_EPILOG_Z
}

void JNICALL DcSctpSocketFactory_NativeSctpSocket_class::setBufferedAmountLowThreshold_(JNIEnv* env, jDcSctpSocketFactory_NativeSctpSocket, jlong ptr, jshort streamId, jlong bytes)
{
SOCKET_PROLOG(ptr)

    nativeSocket->socket->SetBufferedAmountLowThreshold(StreamID(streamId), bytes);
NATIVE_EPILOG
}

jMetrics JNICALL DcSctpSocketFactory_NativeSctpSocket_class::getMetrics_(JNIEnv* env, jDcSctpSocketFactory_NativeSctpSocket, jlong ptr)
{
SOCKET_PROLOG(ptr)

    auto metrics = nativeSocket->socket->GetMetrics();
    if (!metrics) {
        return nullptr;
    }
    auto metricsP = make_unique<Metrics>(*metrics);
    return java_classes::get<Metrics_class>().ctor(env, (jlong)(intptr_t)metricsP.release()).release();
NATIVE_EPILOG_Z
}

DcSctpMessage convertDcSctpMessage(JNIEnv* env, jDcSctpMessage jmessage)
{
    auto & messageClass = java_classes::get<DcSctpMessage_class>();

    auto payload = messageClass.getPayload(env, jmessage);
    java_array_access aPayload(env, payload);
    vector<uint8_t> vPayload(aPayload.cbegin(), aPayload.cend());

    auto streamID = messageClass.getStreamID(env, jmessage);
    auto ppid = messageClass.getPpid(env, jmessage);

    auto message = DcSctpMessage(StreamID(streamID), PPID(ppid), vPayload);
    return message;
}

local_java_ref<jDcSctpMessage> convertDcSctpMessage(JNIEnv* env, const DcSctpMessage& message)
{
    auto payload = message.payload();
    auto jPayload = java_array_create<jbyte>(env, payload.begin(), payload.end());

    auto & messageClass = java_classes::get<DcSctpMessage_class>();
    auto jmessage = messageClass.ctor(env, *message.stream_id(), *message.ppid(), jPayload);

    return jmessage;
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
