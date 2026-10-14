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
#include <exception>
#include <string>
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

jbyteArray JNICALL DcSctpSocketFactory_class::generateConnectionToken_(JNIEnv *env, jDcSctpSocketFactory, jlong ptr, jDcSctpOptions jOptions, jLongBinaryOperator jGetRandomInt)
{
NATIVE_PROLOG
    auto factory = (DcSctpSocketFactory*)(intptr_t)ptr;

    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    auto & operatorClass = java_classes::get<LongBinaryOperator_class>();
    /* GenerateConnectionToken calls this synchronously, so env stays valid for the duration.
       dcsctp is compiled without exception support, so an exception from the Java callback
       must not unwind through it: hold it and rethrow once dcsctp has returned. */
    std::exception_ptr pending;
    auto getRandomInt = [env, &operatorClass, jGetRandomInt, &pending](uint32_t low, uint32_t high) -> uint32_t {
        if (pending) {
            return low;
        }
        try {
            jlong value = operatorClass.applyAsLong(env, jGetRandomInt, low, high);
            if (value < low || value >= high) {
                auto ex = java_runtime::throwable().ctor(env, java_string_create(env,
                        "getRandomInt returned " + std::to_string(value) + ", outside [" +
                        std::to_string(low) + ", " + std::to_string(high) + ")"));
                throw java_exception(ex);
            }
            return static_cast<uint32_t>(value);
        }
        catch (...) {
            pending = std::current_exception();
            return low;
        }
    };

    vector<uint8_t> token = factory->GenerateConnectionToken(*options, getRandomInt);
    if (pending) {
        std::rethrow_exception(pending);
    }

    return java_array_create<jbyte>(env, token.begin(), token.end()).release();
NATIVE_EPILOG_Z
}
