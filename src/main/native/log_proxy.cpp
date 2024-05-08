#include "log_proxy.h"

using namespace smjni;
using namespace rtc;
using namespace std;

LogProxy::LogProxy(JNIEnv * env, jLogProxy jProxy, LoggingSeverity minSeverity):
    logProxy(jglobal_ref(jProxy))
{
    if (instance != nullptr) {
        auto ex = java_runtime::throwable().ctor(env, java_string_create(env, "LogProxy registered multiple times"));
        throw java_exception(ex);
    }

    instance = this;

    LogMessage::AddLogToStream(this, minSeverity);
    LogMessage::SetLogToStderr(false);
}

LogProxy::~LogProxy()
{
    LogMessage::RemoveLogToStream(this);
    if (instance == this) {
        instance = nullptr;
    }
}

void LogProxy::OnLogMessage(const string& message)
{
    /* Shouldn't be called, because we're wrapping OnLogMessage(const LogLineRef&) */
}

static local_java_ref<jstring> jstring_from(JNIEnv * env, absl::string_view str)
{
    if (str.data() == nullptr) {
        return nullptr;
    }
    return java_string_create(env, str.data(), str.size());
}

void LogProxy::OnLogMessage(const LogLineRef& line)
{
    JNIEnv* env = jni_provider::get_jni();

    auto message = jstring_from(env, line.message());
    auto filename = jstring_from(env, line.filename());
    jint lineNo = line.line();
    jInteger threadId = java_classes::get<Integer_class>().fromOptionalInt(env, line.thread_id());
    jlong timestampUs = line.timestamp().IsFinite() ? line.timestamp().us() : numeric_limits<jlong>::min();
    auto severity = enum_members::get<LoggingSeverity_members>().map(env, line.severity());

    java_classes::get<LogProxy_class>().onLogMessage(env, logProxy, message, filename, lineNo, threadId, timestampUs, severity);
}



jlong JNICALL LogProxy_class::registerNative(JNIEnv * env, jLogProxy thiz, jLogProxy_LoggingSeverity minSeverity)
{
NATIVE_PROLOG
    LogProxy* logProxy = new LogProxy(env, thiz, enum_members::get<LoggingSeverity_members>().map(env, minSeverity));
    return (jlong)(intptr_t)logProxy;
NATIVE_EPILOG_Z
}

void JNICALL LogProxy_class::destruct(JNIEnv * env, jclass, jlong nativePtr)
{
NATIVE_PROLOG
    LogProxy* logProxy = (LogProxy*)(intptr_t)nativePtr;

    delete logProxy;
NATIVE_EPILOG
}

