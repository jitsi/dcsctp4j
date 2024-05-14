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

import org.jetbrains.annotations.NotNull;
import smjni.jnigen.CalledByNative;
import smjni.jnigen.ExposeToNative;

// An SCTP message is a group of bytes sent and received as a whole on a
// specified stream identifier (`stream_id`), and with a payload protocol
// identifier (`ppid`).
@ExposeToNative
public class DcSctpMessage {
    public DcSctpMessage(short s, int p, @NotNull byte[] pay)
    {
        streamID = s;
        ppid = p;
        payload = pay;
    }

    // The stream identifier to which the message is sent.
    @CalledByNative
    public short getStreamID() {
        return streamID;
    }

    // The payload protocol identifier (ppid) associated with the message.
    @CalledByNative
    public int getPpid() {
        return ppid;
    }

    // The payload of the message.
    @CalledByNative @NotNull
    public byte[] getPayload() {
        return payload;
    }

    private final short streamID;

    private final int ppid;

    private final byte[] payload;
}
