#include <rtc_base/logging.h>
#include "dcsctp4j.h"

class LogProxy : public rtc::LogSink
{
public:
    LogProxy(JNIEnv * env, jLogProxy jProxy, rtc::LoggingSeverity minSeverity);

    virtual ~LogProxy();

public:
    virtual void OnLogMessage(const std::string& message) override;
    virtual void OnLogMessage(const rtc::LogLineRef& line) override;

private:
    static void initialize_and_register(JNIEnv * env);
    LogProxy * instance;

    smjni::global_java_ref<jLogProxy> logProxy;
};
