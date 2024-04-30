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

import org.jitsi.utils.JNIUtils;
import org.jitsi.utils.logging2.Logger;
import org.jitsi.utils.logging2.LoggerImpl;

import java.lang.ref.Cleaner;

public class DcSctp4j {
    private static final Logger logger =
            new LoggerImpl(DcSctp4j.class.getName());

    static final Cleaner CLEANER = Cleaner.create();

    static {
        // Load the native library
        try {
            JNIUtils.loadLibrary("dcsctp4j", DcSctp4j.class.getClassLoader());
            logger.info("DcSctp4j lib loaded");
        } catch (Exception e) {
            logger.error("Error loading native library: ", e);
        }
    }
}
