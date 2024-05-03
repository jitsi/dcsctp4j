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

import smjni.jnigen.ExposeToNative;

// Return value of DcSctpSocketCallbacks::SendPacketWithStatus.
@ExposeToNative
public enum SendPacketStatus {
    // Indicates that the packet was successfully sent. As sending is unreliable,
    // there are no guarantees that the packet was actually delivered.
    kSuccess(0),
    // The packet was not sent due to a temporary failure, such as the local send
    // buffer becoming exhausted. This return value indicates that the socket will
    // recover and sending that packet can be retried at a later time.
    kTemporaryFailure(1),
    // The packet was not sent due to other reasons.
    kError(2);

    public final int nativeStatus;

    SendPacketStatus(int n) {
        nativeStatus = n;
    }
}
