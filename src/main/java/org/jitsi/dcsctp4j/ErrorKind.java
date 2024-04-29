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

public enum ErrorKind {
    // Indicates that no error has occurred. This will never be the case when
    // `OnError` or `OnAborted` is called.
    kNoError,
    // There have been too many retries or timeouts, and the library has given up.
    kTooManyRetries,
    // A command was received that is only possible to execute when the socket is
    // connected, which it is not.
    kNotConnected,
    // Parsing of the command or its parameters failed.
    kParseFailed,
    // Commands are received in the wrong sequence, which indicates a
    // synchronisation mismatch between the peers.
    kWrongSequence,
    // The peer has reported an issue using ERROR or ABORT command.
    kPeerReported,
    // The peer has performed a protocol violation.
    kProtocolViolation,
    // The receive or send buffers have been exhausted.
    kResourceExhaustion,
    // The client has performed an invalid operation.
    kUnsupportedOperation,
}
