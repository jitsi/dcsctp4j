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

@ExposeToNative
public enum SendStatus {
    /**
     * The message was enqueued successfully. As sending the message is done
     * asynchronously, this is no guarantee that the message has been actually
     * sent.
     */
    @CalledByNative
    kSuccess(0),
    /**
     * The message was rejected as the payload was empty (which is not allowed in
     * SCTP).
     */
    @CalledByNative
    kErrorMessageEmpty(1),
    /**
     * The message was rejected as the payload was larger than what has been set
     * as `DcSctpOptions.max_message_size`.
     */
    @CalledByNative
    kErrorMessageTooLarge(2),
    /**
     * The message could not be enqueued as the socket is out of resources. This
     * mainly indicates that the send queue is full.
     */
    @CalledByNative
    kErrorResourceExhaustion(3),
    /** The message could not be sent as the socket is shutting down. */
    @CalledByNative
    kErrorShuttingDown(4);

    public final int nativeStatus;

    SendStatus(int n) {
        nativeStatus = n;
    }
    static SendStatus fromNativeStatus(int nativeStatus) {
        SendStatus javaStatus;
        switch(nativeStatus) {
            case 0:
                javaStatus = kSuccess;
                break;
            case 1:
                javaStatus = kErrorMessageEmpty;
                break;
            case 2:
                javaStatus = kErrorMessageTooLarge;
                break;
            case 3:
                javaStatus = kErrorResourceExhaustion;
                break;
            case 4:
                javaStatus = kErrorShuttingDown;
                break;
            default:
                throw new IllegalArgumentException("Bad native status value " + nativeStatus);
        }
        if (javaStatus.nativeStatus != nativeStatus) {
            throw new IllegalStateException("native status " + nativeStatus + " was mapped to java status " +
                    javaStatus + " == " + javaStatus.nativeStatus);
        }
        return javaStatus;
    }
}
