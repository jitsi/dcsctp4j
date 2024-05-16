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
    static LogProxy * instance;

    smjni::global_java_ref<jLogProxy> logProxy;
};
