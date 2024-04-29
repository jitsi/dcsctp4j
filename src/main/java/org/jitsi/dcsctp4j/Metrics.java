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

// Tracked metrics, which is the return value of GetMetrics. Optional members
// will be unset when they are not yet known.
public class Metrics {
    // Transmission stats and metrics.

    // Number of packets sent.
    public long tx_packets_count = 0;

    // Number of messages requested to be sent.
    public long tx_messages_count = 0;

    // Number of packets retransmitted. Since SCTP packets can contain both
    // retransmitted DATA chunks and DATA chunks that are transmitted for the
    // first time, this represents an upper bound as it's incremented every time a
    // packet contains a retransmitted DATA chunk.
    public long rtx_packets_count = 0;

    // Total number of bytes retransmitted. This includes the payload and
    // DATA/I-DATA headers, but not SCTP packet headers.
    public long rtx_bytes_count = 0;

    // The current congestion window (cwnd) in bytes, corresponding to spinfo_cwnd
    // defined in RFC6458.
    public long cwnd_bytes = 0;

    // Smoothed round trip time, corresponding to spinfo_srtt defined in RFC6458.
    public int srtt_ms = 0;

    // Number of data items in the retransmission queue that haven’t been
    // acked/nacked yet and are in-flight. Corresponding to sstat_unackdata
    // defined in RFC6458. This may be an approximation when there are messages in
    // the send queue that haven't been fragmented/packetized yet.
    public long unack_data_count = 0;

    // Receive stats and metrics.

    // Number of packets received.
    public long rx_packets_count = 0;

    // Number of messages received.
    public long rx_messages_count = 0;

    // The peer’s last announced receiver window size, corresponding to
    // sstat_rwnd defined in RFC6458.
    public int peer_rwnd_bytes = 0;

    // Returns the detected SCTP implementation of the peer. As this is not
    // explicitly signalled during the connection establishment, heuristics is
    // used to analyze e.g. the state cookie in the INIT-ACK chunk.
    SctpImplementation peer_implementation = SctpImplementation.kUnknown;

    // Indicates if RFC8260 User Message Interleaving has been negotiated by both
    // peers.
    public boolean uses_message_interleaving = false;

    // Indicates if draft-tuexen-tsvwg-sctp-zero-checksum-00 has been negotiated
    // by both peers.
    public boolean uses_zero_checksum = false;

    // The number of negotiated incoming and outgoing streams, which is configured
    // locally as `DcSctpOptions::announced_maximum_incoming_streams` and
    // `DcSctpOptions::announced_maximum_outgoing_streams`, and which will be
    // signaled by the peer during connection.
    public short negotiated_maximum_incoming_streams = 0;
    public short negotiated_maximum_outgoing_streams = 0;

    // Represent known SCTP implementations.
    public static enum SctpImplementation {
        // There is not enough information toto determine any SCTP implementation.
        kUnknown,
        // This implementation.
        kDcsctp,
        // https://github.com/sctplab/usrsctp.
        kUsrSctp,
        // Any other implementation.
        kOther,
    }
}
