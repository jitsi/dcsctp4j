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
#include <exception>
#include "all_classes.h"
#include "wrapped_objects.h"
#include "dcsctp4j.h"

struct NativeSctpSocket {
    NativeSctpSocket(jDcSctpSocketCallbacks jCallbacks);

    /* Declared before the callbacks, which hold a reference to it. */
    CallbackExceptionSink exceptions;
    WrappedSocketCallbacks callbacks;
    std::unique_ptr<dcsctp::DcSctpSocketInterface> socket;
};

/*
 * Rethrows, on leaving the scope, an exception a callback raised while dcsctp was
 * running. Declared inside the NATIVE_PROLOG try block, so the exception is caught
 * by the NATIVE_EPILOG handlers even when the entry point returns from the middle of
 * its body: locals of a try block are destroyed before the block is exited.
 */
class PendingExceptionGuard {
public:
    explicit PendingExceptionGuard(NativeSctpSocket* socket) :
        socket_(socket), uncaughtAtEntry_(std::uncaught_exceptions()) {}

    ~PendingExceptionGuard() noexcept(false)
    {
        /* If the entry point is already unwinding (SimpleJNI threw a java_exception
           directly), that exception wins; throwing here would terminate. */
        if (std::uncaught_exceptions() == uncaughtAtEntry_) {
            socket_->exceptions.rethrow();
        }
    }

    PendingExceptionGuard(const PendingExceptionGuard&) = delete;
    PendingExceptionGuard& operator=(const PendingExceptionGuard&) = delete;

private:
    NativeSctpSocket* socket_;
    int uncaughtAtEntry_;
};

/*
 * Prolog and epilog for native entry points that operate on a NativeSctpSocket.
 * SOCKET_PROLOG(ptr) opens the NATIVE_PROLOG try block, declares `nativeSocket`
 * (the NativeSctpSocket* that `ptr` refers to) for use by the body, and arms a
 * PendingExceptionGuard for it. SOCKET_EPILOG is NATIVE_EPILOG.
 */
#define SOCKET_PROLOG(ptr) NATIVE_PROLOG \
    auto nativeSocket = (NativeSctpSocket*)(intptr_t)(ptr); \
    PendingExceptionGuard pendingExceptionGuard(nativeSocket);
#define SOCKET_EPILOG    NATIVE_EPILOG
#define SOCKET_EPILOG_Z  NATIVE_EPILOG_Z

dcsctp::DcSctpMessage convertDcSctpMessage(JNIEnv* env, jDcSctpMessage jMessage);
dcsctp::SendOptions convertSendOptions(JNIEnv* env, jSendOptions jOptions);

smjni::local_java_ref<jDcSctpMessage> convertDcSctpMessage(JNIEnv* env, const dcsctp::DcSctpMessage& message);

#endif
