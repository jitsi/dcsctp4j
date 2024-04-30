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

// The socket/association state
@ExposeToNative
public enum SocketState {
    // The socket is closed.
    kClosed,
    // The socket has initiated a connection, which is not yet established. Note
    // that for incoming connections and for reconnections when the socket is
    // already connected, the socket will not transition to this state.
    kConnecting,
    // The socket is connected, and the connection is established.
    kConnected,
    // The socket is shutting down, and the connection is not yet closed.
    kShuttingDown,
}
