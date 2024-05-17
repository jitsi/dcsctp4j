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

import org.jetbrains.annotations.Nullable;
import smjni.jnigen.CalledByNative;
import smjni.jnigen.ExposeToNative;

/** Send options for sending messages */
@ExposeToNative
public class SendOptions {
    /** If the message should be sent with unordered message delivery. */
    @CalledByNative
    public boolean isUnordered = false;

    /**
     * If set, will discard messages that haven't been correctly sent and
     * received before the lifetime has expired. This is only available if the
     * peer supports Partial Reliability Extension (RFC3758).
     */
    @CalledByNative @Nullable
    Long lifetime = null;

    /**
     * If set, limits the number of retransmissions. This is only available
     * if the peer supports Partial Reliability Extension (RFC3758).
     */
    @CalledByNative @Nullable
    Long maxRetransmissions = null;

    /**
     * If set, will generate lifecycle events for this message. See e.g.
     * `DcSctpSocketCallbacks::OnLifecycleMessageFullySent`. This value is decided
     * by the client and the library will provide it to all lifecycle callbacks.
     */
    @CalledByNative
    long lifecycleId = LIFECYCLE_NOT_SET;

    static final long LIFECYCLE_NOT_SET = 0;
}
