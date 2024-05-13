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

import java.util.List;

public interface DcSctpSocketInterface {
    /** To be called when an incoming SCTP packet is to be processed. */
    void receivePacket(byte[] data, int offset, int length);

    /** To be called when a timeout has expired. The [timeout_id] is provided
     when the timeout was initiated. */
    void handleTimeout(long timeoutId);

    /** Connects the socket. This is an asynchronous operation, and
     [DcSctpSocketCallbacks.OnConnected] will be called on success. */
    void connect();

    // Puts this socket to the state in which the original socket was when its
    // `DcSctpSocketHandoverState` was captured by `GetHandoverStateAndClose`.
    // `RestoreFromState` is allowed only on the closed socket.
    // `DcSctpSocketCallbacks::OnConnected` will be called if a connected socket
    // state is restored.
    // `DcSctpSocketCallbacks::OnError` will be called on error.
    // NOTE: Skipped, not currently needed
    // void restoreFromState(DcSctpSocketHandoverState state);

    // Gracefully shutdowns the socket and sends all outstanding data. This is an
    // asynchronous operation and `DcSctpSocketCallbacks::OnClosed` will be called
    // on success.
    void shutdown();

    // Closes the connection non-gracefully. Will send ABORT if the connection is
    // not already closed. No callbacks will be made after Close() has returned.
    void close();

    // The socket state.
    SocketState state();

    // The options it was created with.
    DcSctpOptions options();

    // Sets the priority of an outgoing stream. The initial value, when not set,
    // is `DcSctpOptions::default_stream_priority`.
    void setStreamPriority(short streamId, short streamPriority);

    // Returns the currently set priority for an outgoing stream. The initial
    // value, when not set, is `DcSctpOptions::default_stream_priority`.
    short getStreamPriority(short streamId);

    // Sends the message `message` using the provided send options.
    // Sending a message is an asynchronous operation, and the `OnError` callback
    // may be invoked to indicate any errors in sending the message.
    //
    // The association does not have to be established before calling this method.
    // If it's called before there is an established association, the message will
    // be queued.
    SendStatus send(DcSctpMessage message, SendOptions options);

    // Sends the messages `messages` using the provided send options.
    // Sending a message is an asynchronous operation, and the `OnError` callback
    // may be invoked to indicate any errors in sending the message.
    //
    // This has identical semantics to Send, except that it may coalesce many
    // messages into a single SCTP packet if they would fit.
    List<SendStatus> sendMany(List<DcSctpMessage> messages, SendOptions options);

    // Resetting streams is an asynchronous operation and the results will
    // be notified using `DcSctpSocketCallbacks::OnStreamsResetDone()` on success
    // and `DcSctpSocketCallbacks::OnStreamsResetFailed()` on failure. Note that
    // only outgoing streams can be reset.
    //
    // When it's known that the peer has reset its own outgoing streams,
    // `DcSctpSocketCallbacks::OnIncomingStreamReset` is called.
    //
    // Note that resetting a stream will also remove all queued messages on those
    // streams, but will ensure that the currently sent message (if any) is fully
    // sent before closing the stream.
    //
    // Resetting streams can only be done on an established association that
    // supports stream resetting. Calling this method on e.g. a closed association
    // or streams that don't support resetting will not perform any operation.
    ResetStreamsStatus resetStreams(List<Short> outgoingStreams);

    // Returns the number of bytes of data currently queued to be sent on a given
    // stream.
    long bufferedAmount(short streamId);

    // Returns the number of buffered outgoing bytes that is considered "low" for
    // a given stream. See `setBufferedAmountLowThreshold`.
    long bufferedAmountLowThreshold(short streamId);

    // Used to specify the number of bytes of buffered outgoing data that is
    // considered "low" for a given stream, which will trigger an
    // OnBufferedAmountLow event. The default value is zero (0).
    void setBufferedAmountLowThreshold(short streamId, long bytes);

    // Retrieves the latest metrics. If the socket is not fully connected,
    // `null` will be returned. Note that metrics are not guaranteed to
    // be carried over if this socket is handed over by calling
    // `GetHandoverStateAndClose`.
    Metrics getMetrics();

    // Returns empty bitmask if the socket is in the state in which a snapshot of
    // the state can be made by `GetHandoverStateAndClose()`. Return value is
    // invalidated by a call to any non-const method.
    // NOTE: Skipped, not currently needed
    // HandoverReadinessStatus getHandoverReadiness();

    // Collects a snapshot of the socket state that can be used to reconstruct
    // this socket in another process. On success this socket object is closed
    // synchronously and no callbacks will be made after the method has returned.
    // The method fails if the socket is not in a state ready for handover.
    // nullopt indicates the failure. `DcSctpSocketCallbacks::OnClosed` will be
    // called on success.
    // NOTE: Skipped, not currently needed
    // DcSctpSocketHandoverState getHandoverStateAndClose();



}
