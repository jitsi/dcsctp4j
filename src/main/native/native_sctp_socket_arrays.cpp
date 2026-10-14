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

/*
 * The natives that marshal Java arrays into dcsctp calls live in this file rather
 * than in native_sctp_socket.cpp because GCC 10's aarch64 cross compiler crashes
 * with an internal compiler error when they are compiled in the same translation
 * unit as the rest of that file. The Linux build no longer uses GCC, but the split
 * is harmless and keeps the sources buildable with it.
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
