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

import org.jetbrains.annotations.Nullable;
import smjni.jnigen.CalledByNative;
import smjni.jnigen.ExposeToNative;

@ExposeToNative
public class DcSctpOptions
{
    static
    {
        DcSctp4j.init();
    }

    @CalledByNative
    private final long ptr;

    public DcSctpOptions()
    {
        this(construct());
    }

    @CalledByNative
    private DcSctpOptions(long ptr)
    {
        this.ptr = ptr;
        DcSctp4j.CLEANER.register(this, () -> destruct(ptr));
    }

    private static native long construct();

    private static native void destruct(long ptr);

    /**
     * The local port for which the socket is supposed to be bound to. Incoming
     * packets will be verified that they are sent to this port number and all
     * outgoing packets will have this port number as source port.
     */
    public native int getLocalPort();

    public native void setLocalPort(int localPort);

    /**
     * The remote port to send packets to. All outgoing packets will have this
     * port number as destination port.
     */
    public native int getRemotePort();

    public native void setRemotePort(int remotePort);

    /**
     * The announced maximum number of incoming streams. Note that this value is
     * constant and can't be currently increased in run-time as "Add Incoming
     * Streams Request" in RFC6525 isn't supported.
     *
     * The socket implementation doesn't have any per-stream fixed costs, which is
     * why the default value is set to be the maximum value.
     */
    public native int getAnnouncedMaximumIncomingStreams();

    public void setAnnouncedMaximumIncomingStreams(int announcedMaximumIncomingStreams)
    {
        if (announcedMaximumIncomingStreams < 0 || announcedMaximumIncomingStreams > 65535)
        {
            throw new IllegalArgumentException("Invalid value for announcedMaximumIncomingStreams: " +
                    announcedMaximumIncomingStreams);
        }
        setAnnouncedMaximumIncomingStreams_(announcedMaximumIncomingStreams);
    }

    public native void setAnnouncedMaximumIncomingStreams_(int announcedMaximumIncomingStreams);


    /**
     * The announced maximum number of outgoing streams. Note that this value is
     * constant and can't be currently increased in run-time as "Add Outgoing
     * Streams Request" in RFC6525 isn't supported.
     *
     * The socket implementation doesn't have any per-stream fixed costs, which is
     * why the default value is set to be the maximum value.
     */
    public native int getAnnouncedMaximumOutgoingStreams();

    public void setAnnouncedMaximumOutgoingStreams(int announcedMaximumOutgoingStreams)
    {
        if (announcedMaximumOutgoingStreams < 0 || announcedMaximumOutgoingStreams > 65535)
        {
            throw new IllegalArgumentException("Invalid value for announcedMaximumOutgoingStreams: " +
                    announcedMaximumOutgoingStreams);
        }
        setAnnouncedMaximumOutgoingStreams_(announcedMaximumOutgoingStreams);
    }

    public native void setAnnouncedMaximumOutgoingStreams_(int announcedMaximumOutgoingStreams);

    /**
     * Maximum SCTP packet size. The library will limit the size of generated
     * packets to be less than or equal to this number. This does not include any
     * overhead of DTLS, TURN, UDP or IP headers.
     */
    public native long getMtu();

    public native void setMtu(long mtu);

    /**
     * The largest allowed message payload to be sent. Messages will be rejected
     * if their payload is larger than this value. Note that this doesn't affect
     * incoming messages, which may larger than this value (but smaller than
     * `maxReceiverWindowBufferSize`).
     */
    public native long getMaxMessageSize();

    public native void setMaxMessageSize(long maxMessageSize);

    /**
     * The default stream priority, if not overridden by
     * {@link DcSctpSocketInterface#setStreamPriority}. The default value is selected to be
     * compatible with https://www.w3.org/TR/webrtc-priority/, section 4.2-4.3.
     */
    public native short getDefaultStreamPriority();

    public native void setDefaultStreamPriority(short defaultStreamPriority);

    /**
     * Maximum received window buffer size. This should be a bit larger than the
     * largest sized message you want to be able to receive. This essentially
     * limits the memory usage on the receive side. Note that memory is allocated
     * dynamically, and this represents the maximum amount of buffered data. The
     * actual memory usage of the library will be smaller in normal operation, and
     * will be larger than this due to other allocations and overhead if the
     * buffer is fully utilized.
     */
    public native long getMaxReceiverWindowBufferSize();

    public native void setMaxReceiverWindowBufferSize(long maxReceiverWindowBufferSize);

    /**
     * Send queue total size limit. It will not be possible to queue more data if
     * the queue size is larger than this number.
     */
    public native long getMaxSendBufferSize();

    public native void setMaxSendBufferSize(long maxSendBufferSize);

    /**
     * Per stream send queue size limit. Similar to `maxSendBufferSize`, but
     * limiting the size of individual streams.
     */
    public native long getPerStreamSendQueueLimit();

    public native void setPerStreamSendQueueLimit(long perStreamSendQueueLimit);

    /**
     * A threshold that, when the amount of data in the send buffer goes below
     * this value, will trigger {@link DcSctpSocketCallbacks#OnTotalBufferedAmountLow}.
     */
    public native long getTotalBufferedAmountLowThreshold();

    public native void setTotalBufferedAmountLowThreshold(long totalBufferedAmountLowThreshold);

    /**
     * Max allowed RTT value. When the RTT is measured and it's found to be larger
     * than this value, it will be discarded and not used for e.g. any RTO
     * calculation. The default value is an extreme maximum but can be adapted
     * to better match the environment.
     */
    public native long getRttMax();

    public native void setRttMax(long rttMax);

    /** Initial RTO value. */
    public native long getRtoInitial();

    public native void setRtoInitial(long rtoInitial);

    /** Maximum RTP value. */
    public native long getRtoMax();

    public native void setRtoMax(long rtoMax);

    /**
     * Minimum RTO value. This must be larger than an expected peer delayed ack
     * timeout.
     */
    public native long getRtoMin();

    public native void setRtoMin(long rtoMin);

    /** T1-init timeout. */
    public native long getT1InitTimeout();

    public native void setT1InitTimeout(long t1InitTimeout);

    /** T1-cookie timeout. */
    public native long getT1CookieTimeout();

    public native void setT1CookieTimeout(long t1CookieTimeout);

    /** T2-shutdown timeout. */
    public native long getT2ShutdownTimeout();

    public native void setT2ShutdownTimeout(long t2ShutdownTimeout);

    /**
     * For t1-init, t1-cookie, t2-shutdown, t3-rtx, this value - if set - will be
     * the upper bound on how large the exponentially backed off timeout can
     * become. The lower the duration, the faster the connection can recover on
     * transient network issues. Setting this value may require changing
     * `max_retransmissions` and `max_init_retransmits` to ensure that the
     * connection is not closed too quickly.
     */
    @Nullable
    public native Long getMaxTimerBackoffDuration();

    public native void setMaxTimerBackoffDuration(@Nullable Long maxTimerBackoffDuration);

    /** Hearbeat interval (on idle connections only). Set to zero to disable. */
    public native long getHeartbeatInterval();

    public native void setHeartbeatInterval(long heartbeatInterval);

    /**
     * The maximum time when a SACK will be sent from the arrival of an
     * unacknowledged packet. Whatever is smallest of RTO/2 and this will be used.
     */
    public native long getDelayedAckMaxTimeout();

    public native void setDelayedAckMaxTimeout(long delayedAckMaxTimeout);

    /**
     * The minimum limit for the measured RTT variance
     *
     * Setting this below the expected delayed ack timeout (+ margin) of the peer
     * might result in unnecessary retransmissions, as the maximum time it takes
     * to ACK a DATA chunk is typically RTT + ATO (delayed ack timeout), and when
     * the SCTP channel is quite idle, and heartbeats dominate the source of RTT
     * measurement, the RTO would converge with the smoothed RTT (SRTT). The
     * default ATO is 200ms in usrsctp, and a 20ms (10%) margin would include the
     * processing time of received packets and the clock granularity when setting
     * the delayed ack timer on the peer.
     *
     * This is described for TCP in
     * https://datatracker.ietf.org/doc/html/rfc6298#section-4.
     */
    public native long getMinRttVariance();

    public native void setMinRttVariance(long minRttVariance);

    /**
     * The initial congestion window size, in number of MTUs.
     * See https://tools.ietf.org/html/rfc4960#section-7.2.1 which defaults at ~3
     * and https://research.google/pubs/pub36640/ which argues for at least ten
     * segments.
     */
    public native long getCwndMtusInitial();

    public native void setCwndMtusInitial(long cwndMtusInitial);

    /**
     * The minimum congestion window size, in number of MTUs, upon detection of
     * packet loss by SACK. Note that if the retransmission timer expires, the
     * congestion window will be as small as one MTU. See
     * https://tools.ietf.org/html/rfc4960#section-7.2.3.
     */
    public native long getCwndMtusMin();

    public native void setCwndMtusMin(long cwndMtusMin);

    /**
     * When the congestion window is at or above this number of MTUs, the
     * congestion control algorithm will avoid filling the congestion window
     * fully, if that results in fragmenting large messages into quite small
     * packets. When the congestion window is smaller than this option, it will
     * aim to fill the congestion window as much as it can, even if it results in
     * creating small fragmented packets.
     */
    public native long getAvoidFragmentationCwndMtus();

    public native void setAvoidFragmentationCwndMtus(long avoidFragmentationCwndMtus);

    /**
     * The number of packets that may be sent at once. This is limited to avoid
     * bursts that too quickly fill the send buffer. Typically in a a socket in
     * its "slow start" phase (when it sends as much as it can), it will send
     * up to three packets for every SACK received, so the default limit is set
     * just above that, and then mostly applicable for (but not limited to) fast
     * retransmission scenarios.
     */
    public native int getMaxBurst();

    public native void setMaxBurst(int maxBurst);

    /**
     * Maximum Data Retransmit Attempts (per DATA chunk). Set to absl::nullopt for
     * no limit.
     */
    @Nullable
    public native Integer getMaxRetransmissions();

    public native void setMaxRetransmissions(@Nullable Integer maxRetransmissions);

    /**
     * Max.Init.Retransmits (https://tools.ietf.org/html/rfc4960#section-15). Set
     * to null for no limit.
     */
    @Nullable
    public native Integer getMaxInitRetransmits();

    public native void setMaxInitRetransmits(@Nullable Integer maxInitRetransmits);

    /** RFC3758 Partial Reliability Extension */
    public native boolean isEnablePartialReliability();

    public native void setEnablePartialReliability(boolean enablePartialReliability);

    /** RFC8260 Stream Schedulers and User Message Interleaving */
    public native boolean isEnableMessageInterleaving();

    public native void setEnableMessageInterleaving(boolean enableMessageInterleaving);

    /** If RTO should be added to heartbeatInterval */
    public native boolean isHeartbeatIntervalIncludeRtt();

    public native void setHeartbeatIntervalIncludeRtt(boolean heartbeatIntervalIncludeRtt);

    /** Disables SCTP packet crc32 verification. For fuzzers only! */
    public native boolean isDisableChecksumVerification();

    public native void setDisableChecksumVerification(boolean disableChecksumVerification);

    /**
     * Controls the "zero checksum option" feature, as defined in
     * https://www.ietf.org/archive/id/draft-ietf-tsvwg-sctp-zero-checksum-06.html.
     * To have this feature enabled, both peers must be configured to use the
     * same (defined, not "none") alternate error detection method.
     */
    public native int getZeroChecksumAlternateErrorDetectionMethod();

    public native void setZeroChecksumAlternateErrorDetectionMethod(int zeroChecksumAlternateErrorDetectionMethod);

    /**
     * The largest safe SCTP packet. Starting from the minimum guaranteed MTU
     * value of 1280 for IPv6 (which may not support fragmentation), take off 85
     * bytes for DTLS/TURN/TCP/IP and ciphertext overhead.
     *
     * Additionally, it's possible that TURN adds an additional 4 bytes of
     * overhead after a channel has been established, so an additional 4 bytes is
     * subtracted
     *
     * 1280 IPV6 MTU
     *  -40 IPV6 header
     *   -8 UDP
     *  -24 GCM Cipher
     *  -13 DTLS record header
     *   -4 TURN ChannelData
     * = 1191 bytes.
     */
    public static final long kMaxSafeMTUSize = 1191;

    public static final int ZeroChecksumAlternateErrorDetectionMethod_None = 0;

    public static final int ZeroChecksumAlternateErrorDetectionMethod_LowerLayerDtls = 1;
}
