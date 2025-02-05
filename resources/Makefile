# Minimal build steps that produce libdcsctp.a
# Copyright (C) 2025
# Author: Thomas Fitzsimmons <fitzsim@fitzsim.org>
# Version: 1
# SPDX-License-Identifier: Apache-2.0
CXXFLAGS = -fPIC -DWEBRTC_POSIX -Wno-return-type
CXXFLAGS += -I../..
CXXFLAGS += -I../../third_party/abseil-cpp
CXXFLAGS += -I../../third_party/crc32c/src/include
CXXFLAGS += -I../../third_party/crc32c/config
OBJS = \
	../../rtc_base/logging.o \
	../../rtc_base/strings/string_format.o \
	../../rtc_base/strings/string_builder.o \
	../../rtc_base/string_encode.o \
	../../api/units/time_delta.o \
	../../third_party/crc32c/src/src/crc32c.o \
	../../third_party/crc32c/src/src/crc32c_portable.o \
	./rx/reassembly_queue.o \
	./rx/data_tracker.o \
	./rx/interleaved_reassembly_streams.o \
	./rx/traditional_reassembly_streams.o \
	./testing/data_generator.o \
	./tx/stream_scheduler.o \
	./tx/retransmission_error_counter.o \
	./tx/rr_send_queue.o \
	./tx/retransmission_timeout.o \
	./tx/retransmission_queue.o \
	./tx/outstanding_data.o \
	./timer/timer.o \
	./timer/task_queue_timeout.o \
	./packet/parameter/supported_extensions_parameter.o \
	./packet/parameter/parameter.o \
	./packet/parameter/ssn_tsn_reset_request_parameter.o \
	./packet/parameter/state_cookie_parameter.o \
	./packet/parameter/heartbeat_info_parameter.o \
	./packet/parameter/outgoing_ssn_reset_request_parameter.o \
	./packet/parameter/add_outgoing_streams_request_parameter.o \
	./packet/parameter/add_incoming_streams_request_parameter.o \
	./packet/parameter/reconfiguration_response_parameter.o \
	./packet/parameter/forward_tsn_supported_parameter.o \
	./packet/parameter/zero_checksum_acceptable_chunk_parameter.o \
	./packet/parameter/incoming_ssn_reset_request_parameter.o \
	./packet/error_cause/invalid_stream_identifier_cause.o \
	./packet/error_cause/error_cause.o \
	./packet/error_cause/unresolvable_address_cause.o \
	./packet/error_cause/user_initiated_abort_cause.o \
	./packet/error_cause/stale_cookie_error_cause.o \
	./packet/error_cause/unrecognized_chunk_type_cause.o \
	./packet/error_cause/out_of_resource_error_cause.o \
	./packet/error_cause/protocol_violation_cause.o \
	./packet/error_cause/cookie_received_while_shutting_down_cause.o \
	./packet/error_cause/invalid_mandatory_parameter_cause.o \
	./packet/error_cause/missing_mandatory_parameter_cause.o \
	./packet/error_cause/no_user_data_cause.o \
	./packet/error_cause/unrecognized_parameter_cause.o \
	./packet/chunk/heartbeat_request_chunk.o \
	./packet/chunk/init_chunk.o \
	./packet/chunk/shutdown_chunk.o \
	./packet/chunk/error_chunk.o \
	./packet/chunk/idata_chunk.o \
	./packet/chunk/data_chunk.o \
	./packet/chunk/init_ack_chunk.o \
	./packet/chunk/cookie_echo_chunk.o \
	./packet/chunk/shutdown_complete_chunk.o \
	./packet/chunk/shutdown_ack_chunk.o \
	./packet/chunk/heartbeat_ack_chunk.o \
	./packet/chunk/cookie_ack_chunk.o \
	./packet/chunk/forward_tsn_chunk.o \
	./packet/chunk/iforward_tsn_chunk.o \
	./packet/chunk/reconfig_chunk.o \
	./packet/chunk/chunk.o \
	./packet/chunk/abort_chunk.o \
	./packet/chunk/sack_chunk.o \
	./packet/tlv_trait.o \
	./packet/chunk_validators.o \
	./packet/crc32c.o \
	./packet/sctp_packet.o \
	./fuzzers/dcsctp_fuzzers.o \
	./public/dcsctp_handover_state.o \
	./public/dcsctp_socket_factory.o \
	./public/text_pcap_packet_observer.o \
	./socket/transmission_control_block.o \
	./socket/heartbeat_handler.o \
	./socket/packet_sender.o \
	./socket/callback_deferrer.o \
	./socket/dcsctp_socket.o \
	./socket/state_cookie.o \
	./socket/stream_reset_handler.o \
	./common/handover_testing.o
OBJS += ./packet/error_cause/restart_of_an_association_with_new_address_cause.o

libdcsctp.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) libdcsctp.a
