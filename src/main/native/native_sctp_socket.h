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

#ifndef NATIVE_SCTP_SOCKET_H
#define NATIVE_SCTP_SOCKET_H

#include <smjni/smjni.h>
#include "all_classes.h"
#include "wrapped_objects.h"

struct NativeSctpSocket {
    NativeSctpSocket(jDcSctpSocketCallbacks jCallbacks);

    WrappedSocketCallbacks callbacks;
    std::unique_ptr<dcsctp::DcSctpSocketInterface> socket;
};

dcsctp::DcSctpMessage convertDcSctpMessage(JNIEnv* env, jDcSctpMessage jMessage);
dcsctp::SendOptions convertSendOptions(JNIEnv* env, jSendOptions jOptions);

#endif
