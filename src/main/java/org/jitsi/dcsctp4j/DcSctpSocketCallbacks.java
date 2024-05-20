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

import java.time.Instant;

import org.jetbrains.annotations.NotNull;
import smjni.jnigen.*;

/**
 * Callbacks that the DcSctpSocket will call synchronously to the owning
 * client. It is allowed to call back into the library from callbacks that start
 * with "On". It has been explicitly documented when it's not allowed to call
 * back into this library from within a callback.
 *
 * Theses callbacks are only synchronously triggered as a result of the client
 * calling a public method in {@link DcSctpSocketInterface}.
 */
@ExposeToNative
public interface DcSctpSocketCallbacks
{

    /**
     * Called when the library wants the packet serialized as `data` to be sent.
     *
     * Note that it's NOT ALLOWED to call into this library from within this
     * callback.
     */
    SendPacketStatus sendPacketWithStatus(@NotNull byte[] data);

    /** Version of sendPacketWithStatus optimizing JNI */
    @CalledByNative
    default int sendPacketWithStatus_(@NotNull byte[] data)
    {
        return sendPacketWithStatus(data).nativeStatus;
    }

    /**
     * Called when the library wants to create a Timeout. The callback must return
     * an object that implements that interface.
     *
     * Low precision tasks are scheduled more efficiently by using leeway to
     * reduce Idle Wake Ups and is the preferred precision whenever possible. High
     * precision timeouts do not have this leeway, but is still limited by OS
     * timer precision. At the time of writing, kLow's additional leeway may be up
     * to 17 ms, but please see {@link DelayPrecision} for
     * up-to-date information.
     *
     * Note that it's NOT ALLOWED to call into this library from within this
     * callback.
     */
    @CalledByNative
    Timeout createTimeout(@NotNull DelayPrecision precision);

    /**
     * Returns the current time (from any epoch).
     *
     * Note that it's NOT ALLOWED to call into this library from within this
     * callback.
     */
    @CalledByNative @NotNull
    Instant Now();

    /**
     * Called when the library needs a random number uniformly distributed between
     * `low` (inclusive) and `high` (exclusive). The random numbers used by the
     * library are not used for cryptographic purposes. There are no requirements
     * that the random number generator must be secure.
     *
     * Note that it's NOT ALLOWED to call into this library from within this
     * callback.
     */
    @CalledByNative
    long getRandomInt(long low, long high);

    /**
     * Triggered when the outgoing message buffer is empty, meaning that there are
     * no more queued messages, but there can still be packets in-flight or to be
     * retransmitted. (in contrast to SCTP_SENDER_DRY_EVENT).
     *
     * Note that it's NOT ALLOWED to call into this library from within this
     * callback.
     */
    @Deprecated
    default void NotifyOutgoingMessageBufferEmpty() {}

    /**
     * Called when the library has received an SCTP message in full and delivers
     * it to the upper layer.
     *
     * It is allowed to call into this library from within this callback.
     */
    @CalledByNative
    void OnMessageReceived(@NotNull DcSctpMessage message);

    /** Version of OnMessageReceived optimizing JNI */
    @CalledByNative
    default void OnMessageReceived_(@NotNull byte[] payload, int ppid, short streamID)
    {
        DcSctpMessage message = new DcSctpMessage(streamID, ppid, payload);
        OnMessageReceived(message);
    }

    /**
     * Triggered when an non-fatal error is reported by either this library or
     * from the other peer (by sending an ERROR command). These should be logged,
     * but no other action need to be taken as the association is still viable.
     *
     * It is allowed to call into this library from within this callback.
     */
    @CalledByNative
    void OnError(@NotNull ErrorKind error, @NotNull String message);

    /**
     * Triggered when the socket has aborted - either as decided by this socket
     * due to e.g. too many retransmission attempts, or by the peer when
     * receiving an ABORT command. No other callbacks will be done after this
     * callback, unless reconnecting.
     *
     * It is allowed to call into this library from within this callback.
     */
    @CalledByNative
    void OnAborted(@NotNull ErrorKind error, @NotNull String message);

    /**
     * Called when calling {@link DcSctpSocketInterface#connect} succeeds, but also for incoming successful
     * connection attempts.
     *
     * It is allowed to call into this library from within this callback.
     */
    @CalledByNative
    void OnConnected();

    /**
     * Called when the socket is closed in a controlled way. No other
     * callbacks will be done after this callback, unless reconnecting.
     *
     * It is allowed to call into this library from within this callback.
     */
    @CalledByNative
    void OnClosed();

    /**
     * On connection restarted (by peer). This is just a notification, and the
     * association is expected to work fine after this call, but there could have
     * been packet loss as a result of restarting the association.
     *
     * It is allowed to call into this library from within this callback.
     */
    @CalledByNative
    void OnConnectionRestarted();

    /**
     * Indicates that a stream reset request has failed.
     *
     * It is allowed to call into this library from within this callback.
     */
    @CalledByNative
    void OnStreamsResetFailed(
            @NotNull short[] outgoing_streams,
            @NotNull String reason);

    /**
     * Indicates that a stream reset request has been performed.
     *
     * It is allowed to call into this library from within this callback.
     */
    @CalledByNative
    void OnStreamsResetPerformed(
            @NotNull short[] outgoing_streams);

    /**
     * When a peer has reset some of its outgoing streams, this will be called. An
     * empty list indicates that all streams have been reset.
     *
     * It is allowed to call into this library from within this callback.
     */
    @CalledByNative
    void OnIncomingStreamsReset(
            @NotNull short[] incoming_streams);

    /**
     * Will be called when the amount of data buffered to be sent falls to or
     * below the threshold set when calling {@link DcSctpSocketInterface#setBufferedAmountLowThreshold}.
     *
     * It is allowed to call into this library from within this callback.
     */
    @CalledByNative
    default void OnBufferedAmountLow(short stream_id) {}

    /**
     * Will be called when the total amount of data buffered (in the entire send
     * buffer, for all streams) falls to or below the threshold specified in
     * `DcSctpOptions::total_buffered_amount_low_threshold`.
     */
    @CalledByNative
    default void OnTotalBufferedAmountLow() {}

    /* Omitting lifecycle events for now. */


    @ExposeToNative
    enum DelayPrecision
    {
        /**
         * This may include up to a 17 ms leeway in addition to OS timer precision.
         * See PostDelayedTask() for more information.
         */
        @CalledByNative
        kLow,
        /**
         * This does not have the additional delay that kLow has, but it is still
         * limited by OS timer precision. See PostDelayedHighPrecisionTask() for
         * more information.
         */
        @CalledByNative
        kHigh,
    }
}
