#include <smjni/smjni.h>
#include "all_classes.h"
#include <net/dcsctp/public/dcsctp_socket_factory.h>
#include "wrapped_objects.h"

using namespace smjni;
using namespace dcsctp;
using namespace std;

#define NATIVE_PROLOG  try {
#define NATIVE_EPILOG  } \
                       catch(java_exception & ex) \
                       { \
                           ex.raise(env);\
                       }\
                       catch(std::exception & ex)\
                       {\
                           java_exception::translate(env, ex);\
                       }

using java_classes = smjni::java_class_table<JNIGEN_ALL_GENERATED_CLASSES>;

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    try
    {
        jni_provider::init(vm);
        JNIEnv * env = jni_provider::get_jni();
        java_runtime::init(env);

        NATIVE_PROLOG
            java_classes::init(env);

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


jlong JNICALL DcSctpSocketFactory_class::construct(JNIEnv *, jclass)
{
    auto factory = new DcSctpSocketFactory();
    return (jlong)(intptr_t)factory;
}

void JNICALL DcSctpSocketFactory_class::destruct(JNIEnv *, jclass, jlong ptr)
{
    auto factory = (DcSctpSocketFactory*)(intptr_t)ptr;
    delete factory;
}
   
jlong JNICALL create_(JNIEnv *env, jDcSctpSocketFactory, jlong ptr, jstring jLogPrefix, jDcSctpSocketCallbacks jCallbacks, jPacketObserver jPacketObserver, jDcSctpOptions options)
{
    auto factory = (DcSctpSocketFactory*)(intptr_t)ptr;

    string logPrefix = java_string_to_cpp(env, jLogPrefix);
    WrappedSocketCallbacks socketCallbacks(jCallbacks);
    unique_ptr<PacketObserver> packetObserver;
    if (jPacketObserver != nullptr) {
        packetObserver = make_unique<WrappedPacketObserver>(jPacketObserver);
    }
    else {
        packetObserver = nullptr;
    }
    
    auto socket = factory->Create(logPrefix, socketCallbacks, std::move(packetObserver), nullptr);
    return 0;
}
