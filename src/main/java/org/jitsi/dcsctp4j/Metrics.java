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

/**
 * Tracked metrics, which is the return value of GetMetrics. Optional members
 * will be unset when they are not yet known.
 */
@ExposeToNative
public class Metrics
{
    @CalledByNative
    private final long ptr;

    @CalledByNative
    private Metrics(long ptr)
    {
        this.ptr = ptr;
        DcSctp4j.CLEANER.register(this, () -> destruct(ptr));
    }

    private static native void destruct(long ptr);

    // Transmission stats and metrics.

    /** Number of packets sent. */
    public native long getTxPacketsCount();

    /** Number of messages requested to be sent. */
    public native long getTxMessagesCount();

    /**
     * Number of packets retransmitted. Since SCTP packets can contain both
     * retransmitted DATA chunks and DATA chunks that are transmitted for the
     * first time, this represents an upper bound as it's incremented every time a
     * packet contains a retransmitted DATA chunk.
     */
    public native long getRtxPacketsCount();

    /**
     * Total number of bytes retransmitted. This includes the payload and
     * DATA/I-DATA headers, but not SCTP packet headers.
     */
    public native long getRtxBytesCount();

    /**
     * The current congestion window (cwnd) in bytes, corresponding to spinfo_cwnd
     * defined in RFC6458.
     */
    public native long getCwndBytes();

    /** Smoothed round trip time, corresponding to spinfo_srtt defined in RFC6458. */
    public native int getSrttMs();

    /**
     * Number of data items in the retransmission queue that haven’t been
     * acked/nacked yet and are in-flight. Corresponding to sstat_unackdata
     * defined in RFC6458. This may be an approximation when there are messages in
     * the send queue that haven't been fragmented/packetized yet.
     */
    public native long getUnackDataCount();

    // Receive stats and metrics.

    /** Number of packets received. */
    public native long getRxPacketsCount();

    /** Number of messages received. */
    public native long getRxMessagesCount();

    /**
     * The peer’s last announced receiver window size, corresponding to
     * sstat_rwnd defined in RFC6458.
     */
    public native int getPeerRwndBytes();

    /**
     * Returns the detected SCTP implementation of the peer. As this is not
     * explicitly signalled during the connection establishment, heuristics is
     * used to analyze e.g. the state cookie in the INIT-ACK chunk.
     */
    @NotNull
    public native SctpImplementation getPeerImplementation();

    /**
     * Indicates if RFC8260 User Message Interleaving has been negotiated by both
     * peers.
     */
    public native boolean usesMessageInterleaving();

    /**
     * Indicates if draft-tuexen-tsvwg-sctp-zero-checksum-00 has been negotiated
     * by both peers.
     */
    public native boolean usesZeroChecksum();

    /**
     * The number of negotiated incoming and outgoing streams, which is configured
     * locally as `DcSctpOptions::announcedMaximumIncomingStreams` and
     * `DcSctpOptions::announcedMaximumOutgoingStreams`, and which will be
     * signaled by the peer during connection.
     */
    public native short getNegotiatedMaximumIncomingStreams();

    public native short getNegotiatedMaximumOutgoingStreams();

    /** Represent known SCTP implementations. */
    @ExposeToNative
    public enum SctpImplementation
    {
        /** There is not enough information toto determine any SCTP implementation. */
        @CalledByNative
        kUnknown,
        /** This implementation. */
        @CalledByNative
        kDcsctp,
        /** https://github.com/sctplab/usrsctp. */
        @CalledByNative
        kUsrSctp,
        /** Any other implementation. */
        @CalledByNative
        kOther,
    }
}
