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
#include "all_classes.h"
#include <net/dcsctp/public/dcsctp_socket_factory.h>
#include "wrapped_objects.h"
#include "native_sctp_socket.h"
#include "dcsctp4j.h"

using namespace smjni;
using namespace dcsctp;
using namespace std;

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    try
    {
        jni_provider::init(vm);
        JNIEnv * env = jni_provider::get_jni();
        java_runtime::init(env);

        NATIVE_PROLOG
            java_classes::init(env);
            enum_members::init(env);
            return JNI_VERSION_1_6;
        NATIVE_EPILOG
    }
    catch(std::exception & ex)
    {
        //If we are here there is no way to communicate with
        //Java - something really bad happened.
        //Let's just log and report failure
        fprintf(stderr, "%s\n", ex.what());

    }
    return 0;
}

jlong JNICALL DcSctpSocketFactory_class::construct(JNIEnv *env, jclass)
{
NATIVE_PROLOG
    auto factory = new DcSctpSocketFactory();
    return (jlong)(intptr_t)factory;
NATIVE_EPILOG_Z
}

void JNICALL DcSctpSocketFactory_class::destruct(JNIEnv *env, jclass, jlong ptr)
{
NATIVE_PROLOG
    auto factory = (DcSctpSocketFactory*)(intptr_t)ptr;
    delete factory;
NATIVE_EPILOG
}
   
jlong JNICALL DcSctpSocketFactory_class::create_(JNIEnv *env, jDcSctpSocketFactory, jlong ptr, jstring jLogPrefix, jDcSctpSocketCallbacks jCallbacks, jPacketObserver jPacketObserver, jDcSctpOptions jOptions)
{
NATIVE_PROLOG
    auto factory = (DcSctpSocketFactory*)(intptr_t)ptr;

    string logPrefix = java_string_to_cpp(env, jLogPrefix);

    auto nativeSocket = make_unique<NativeSctpSocket>(jCallbacks);

    unique_ptr<PacketObserver> packetObserver;
    if (jPacketObserver != nullptr) {
        packetObserver = make_unique<WrappedPacketObserver>(jPacketObserver);
    }
    else {
        packetObserver = nullptr;
    }
    
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    nativeSocket->socket = factory->Create(logPrefix, nativeSocket->callbacks, std::move(packetObserver), *options);

    return (jlong)(intptr_t)nativeSocket.release();
NATIVE_EPILOG_Z
}
