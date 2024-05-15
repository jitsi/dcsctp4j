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
package org.jitsi.dcsctp4j;

import smjni.jnigen.CalledByNative;
import smjni.jnigen.ExposeToNative;

import java.time.Instant;
import java.util.logging.Level;
import java.util.logging.LogRecord;

@ExposeToNative
public class LogProxy {
    private final long nativePtr;
    private final java.util.logging.Logger loggerDelegate = java.util.logging.Logger.getLogger(DcSctp4j.class.getName());

    LogProxy()
    {
        nativePtr = registerNative(getEffectiveLoggingSeverity(loggerDelegate));
        long ptrCopy = nativePtr;
        DcSctp4j.CLEANER.register(this, () -> destruct(ptrCopy));
    }

    @CalledByNative
    void onLogMessage(String message, String filename, int line, Integer threadId, long timestampUs, LoggingSeverity severity) {

        /* WebRTC log messages have filename and line, whereas Java log records have class and method.
         * So, encode filename and line, if present, into the message.
         */
        String formattedMessage;
        if (filename == null && line == 0)
        {
            formattedMessage = message;
        }
        else
        {
            StringBuilder sb = new StringBuilder();
            if (filename != null) {
                sb.append(filename);
            }
            if (line != 0)
            {
                sb.append("#").append(line);
            }
            sb.append(": ");
            sb.append(message);
            formattedMessage = sb.toString();
        }


        LogRecord record = new LogRecord(severity.level, formattedMessage);
        record.setLoggerName(loggerDelegate.getName());

        if (timestampUs != Long.MIN_VALUE)
        {
            long secs = Math.floorDiv(timestampUs, 1000_000L);
            long micros = Math.floorMod(timestampUs, 1000_000L);
            Instant instant = Instant.ofEpochSecond(secs, micros * 1000L);
            record.setInstant(instant);
        }

        if (threadId != null)
        {
            record.setThreadID(threadId);
        }

        loggerDelegate.log(record);
    }
    private native long registerNative(LoggingSeverity minSeverity);

    private static native void destruct(long nativePtr);

    private static LogProxy instance;
    static void register()
    {
        if (instance == null) {
            instance = new LogProxy();
        }
    }

    /** Unfortunately there's no API to get a logger's effective log level directly, so
     *  experimentally find the broadest LoggingSeverity whose level is logged by the logger.
     */
    private static LoggingSeverity getEffectiveLoggingSeverity(java.util.logging.Logger logger)
    {
        LoggingSeverity ret = LoggingSeverity.LS_NONE;
        for (LoggingSeverity l : LoggingSeverity.values()) {
            if (logger.isLoggable(l.level) &&
                    l.level.intValue() < ret.level.intValue()) {
                ret = l;
            }
        }
        return ret;
    }

    @ExposeToNative
    public enum LoggingSeverity {
        @CalledByNative
        LS_VERBOSE(Level.FINE),
        @CalledByNative
        LS_INFO(Level.INFO),
        @CalledByNative
        LS_WARNING(Level.WARNING),
        @CalledByNative
        LS_ERROR(Level.SEVERE),
        @CalledByNative
        LS_NONE(Level.OFF);

        public final Level level;

        LoggingSeverity(Level level) {
            this.level = level;
        }

        public static LoggingSeverity fromLevel(Level level) {
            LoggingSeverity ret = LS_NONE;
            for (LoggingSeverity l : LoggingSeverity.values()) {
                if (l.level.intValue() >= level.intValue() &&
                        l.level.intValue() < ret.level.intValue()) {
                    ret = l;
                }
            }
            return ret;
        }
    }
}
