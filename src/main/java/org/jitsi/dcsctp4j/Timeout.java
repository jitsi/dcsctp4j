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

/**
 * A very simple timeout that can be started and stopped. When started,
 * it will be given a unique `timeout_id` which should be provided to
 * {@link DcSctpSocketInterface#handleTimeout} when it expires.
 */
@ExposeToNative
public interface Timeout
{
    /**
     * Called to start time timeout, with the duration in milliseconds as
     * `duration` and with the timeout identifier as `timeout_id`, which - if
     * the timeout expires - shall be provided to `DcSctpSocket::HandleTimeout`.
     *
     * `Start` and `Stop` will always be called in pairs. In other words will
     * ´Start` never be called twice, without a call to `Stop` in between.
     *
     * Note(Java): the SctpSocket holds a JNI global reference to a Timeout object.  To avoid
     * reference cycles, the Timeout subclass must only have weak references to objects that reference
     * the SctpSocket.
     */
    @CalledByNative
    void start(long duration, long timeoutId);

    /**
     * Called to stop the running timeout.
     *
     * `Start` and `Stop` will always be called in pairs. In other words will
     * ´Start` never be called twice, without a call to `Stop` in between.
     *
     * `Stop` will always be called prior to releasing this object.
     */
    @CalledByNative
    void stop();

    /**
     * Called to restart an already running timeout, with the `duration` and
     * `timeout_id` parameters as described in `Start`. This can be overridden by
     * the implementation to restart it more efficiently.
     */
    @CalledByNative
    default void restart(long durationMs, long timeoutId)
    {
        stop();
        start(durationMs, timeoutId);
    }
}
