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
import java.time.Instant;

// Callbacks that the DcSctpSocket will call synchronously to the owning
// client. It is allowed to call back into the library from callbacks that start
// with "On". It has been explicitly documented when it's not allowed to call
// back into this library from within a callback.
//
// Theses callbacks are only synchronously triggered as a result of the client
// calling a public method in `DcSctpSocketInterface`.
public interface DcSctpSocketCallbacks {
    // Called when the library wants the packet serialized as `data` to be sent.
    //
    // TODO(bugs.webrtc.org/12943): This method is deprecated, see
    // `SendPacketWithStatus`.
    //
    // Note that it's NOT ALLOWED to call into this library from within this
    // callback.
    default void sendPacket(ByteBuffer data) {}

    // Called when the library wants the packet serialized as `data` to be sent.
    //
    // Note that it's NOT ALLOWED to call into this library from within this
    // callback.
    default SendPacketStatus sendPacketWithStatus(
            ByteBuffer data) {
        sendPacket(data);
        return SendPacketStatus.kSuccess;
    }

    // Called when the library wants to create a Timeout. The callback must return
    // an object that implements that interface.
    //
    // Low precision tasks are scheduled more efficiently by using leeway to
    // reduce Idle Wake Ups and is the preferred precision whenever possible. High
    // precision timeouts do not have this leeway, but is still limited by OS
    // timer precision. At the time of writing, kLow's additional leeway may be up
    // to 17 ms, but please see webrtc::TaskQueueBase::DelayPrecision for
    // up-to-date information.
    //
    // Note that it's NOT ALLOWED to call into this library from within this
    // callback.
    default Timeout createTimeout(DelayPrecision precision) {
        // TODO(hbos): When dependencies have migrated to this new signature, make
        // this pure virtual and delete the other version.
        return createTimeout();
    }
    // TODO(hbos): When dependencies have migrated to the other signature, delete
    // this version.
    default Timeout createTimeout() {
        return createTimeout(DelayPrecision.kLow);
    }

    // Returns the current time in milliseconds (from any epoch).
    //
    // TODO(bugs.webrtc.org/15593): This method is deprecated, see `Now`.
    //
    // Note that it's NOT ALLOWED to call into this library from within this
    // callback.
    default long timeMillis() { return 0; }

    // Returns the current time (from any epoch).
    //
    // This callback will eventually replace `TimeMillis()`.
    //
    // Note that it's NOT ALLOWED to call into this library from within this
    // callback.
    default Instant Now() {
        return Instant.ofEpochMilli(timeMillis());
    }

    // Called when the library needs a random number uniformly distributed between
    // `low` (inclusive) and `high` (exclusive). The random numbers used by the
    // library are not used for cryptographic purposes. There are no requirements
    // that the random number generator must be secure.
    //
    // Note that it's NOT ALLOWED to call into this library from within this
    // callback.
    int getRandomInt(int low, int high);

    // Triggered when the outgoing message buffer is empty, meaning that there are
    // no more queued messages, but there can still be packets in-flight or to be
    // retransmitted. (in contrast to SCTP_SENDER_DRY_EVENT).
    //
    // Note that it's NOT ALLOWED to call into this library from within this
    // callback.
    @Deprecated
    default void NotifyOutgoingMessageBufferEmpty() {}

    // Called when the library has received an SCTP message in full and delivers
    // it to the upper layer.
    //
    // It is allowed to call into this library from within this callback.
    void OnMessageReceived(DcSctpMessage message);

    // Triggered when an non-fatal error is reported by either this library or
    // from the other peer (by sending an ERROR command). These should be logged,
    // but no other action need to be taken as the association is still viable.
    //
    // It is allowed to call into this library from within this callback.
    void OnError(ErrorKind error, String message);

    // Triggered when the socket has aborted - either as decided by this socket
    // due to e.g. too many retransmission attempts, or by the peer when
    // receiving an ABORT command. No other callbacks will be done after this
    // callback, unless reconnecting.
    //
    // It is allowed to call into this library from within this callback.
    void OnAborted(ErrorKind error, String message);

    // Called when calling `Connect` succeeds, but also for incoming successful
    // connection attempts.
    //
    // It is allowed to call into this library from within this callback.
    void OnConnected();


    enum DelayPrecision {
        // This may include up to a 17 ms leeway in addition to OS timer precision.
        // See PostDelayedTask() for more information.
        kLow,
        // This does not have the additional delay that kLow has, but it is still
        // limited by OS timer precision. See PostDelayedHighPrecisionTask() for
        // more information.
        kHigh,
    }
}
