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

import java.nio.ByteBuffer;

// A PacketObserver can be attached to a socket and will be called for
// all sent and received packets.
public interface PacketObserver {
    // Called when a packet is sent, with the current time (in milliseconds) as
    // `now`, and the packet payload as `payload`.
    void OnSentPacket(long now, ByteBuffer payload);

    // Called when a packet is received, with the current time (in milliseconds)
    // as `now`, and the packet payload as `payload`.
    void OnReceivedPacket(long now, ByteBuffer payload);
}
