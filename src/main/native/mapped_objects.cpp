#include <smjni/smjni.h>
#include "all_classes.h"
#include <net/dcsctp/public/dcsctp_options.h>
#include "dcsctp4j.h"

using namespace smjni;
using namespace dcsctp;


jlong JNICALL DcSctpOptions_class::construct(JNIEnv* env, jclass)
{
NATIVE_PROLOG
    auto options = new DcSctpOptions();
    return (jlong)(intptr_t)options;
NATIVE_EPILOG_Z
}

void JNICALL DcSctpOptions_class::destruct(JNIEnv* env, jclass, jlong ptr)
{
NATIVE_PROLOG
    auto options = (DcSctpOptions*)(intptr_t)ptr;
    delete options;
NATIVE_EPILOG
}

jint JNICALL DcSctpOptions_class::getLocalPort(JNIEnv* env, jDcSctpOptions jOptions)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    return options->local_port;
NATIVE_EPILOG_Z
}

void JNICALL DcSctpOptions_class::setLocalPort(JNIEnv* env, jDcSctpOptions jOptions, jint localPort)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    options->local_port = localPort;
NATIVE_EPILOG
}

jint JNICALL DcSctpOptions_class::getRemotePort(JNIEnv* env, jDcSctpOptions jOptions)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    return options->remote_port;
NATIVE_EPILOG_Z
}

void JNICALL DcSctpOptions_class::setRemotePort(JNIEnv* env, jDcSctpOptions jOptions, jint remotePort)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    options->remote_port = remotePort;
NATIVE_EPILOG
}

jint JNICALL DcSctpOptions_class::getAnnouncedMaximumIncomingStreams(JNIEnv* env, jDcSctpOptions jOptions)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    return options->announced_maximum_incoming_streams;
NATIVE_EPILOG_Z
}

void JNICALL DcSctpOptions_class::setAnnouncedMaximumIncomingStreams_(JNIEnv* env, jDcSctpOptions jOptions, jint announcedMaximumIncomingStreams)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    if (announcedMaximumIncomingStreams < 0 || announcedMaximumIncomingStreams > 65535) {
        auto ex = java_runtime::throwable().ctor(env, java_string_create(env, "Invalid value for announcedMaximumIncomingStreams"));
        throw java_exception(ex);
    }
    else {
        options->announced_maximum_incoming_streams = announcedMaximumIncomingStreams;
    }
NATIVE_EPILOG
}


jint JNICALL DcSctpOptions_class::getAnnouncedMaximumOutgoingStreams(JNIEnv* env, jDcSctpOptions jOptions)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    return options->announced_maximum_outgoing_streams;
NATIVE_EPILOG_Z
}

void JNICALL DcSctpOptions_class::setAnnouncedMaximumOutgoingStreams_(JNIEnv* env, jDcSctpOptions jOptions, jint announcedMaximumOutgoingStreams)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    if (announcedMaximumOutgoingStreams < 0 || announcedMaximumOutgoingStreams > 65535) {
        auto ex = java_runtime::throwable().ctor(env, java_string_create(env, "Invalid value for announcedMaximumOutgoingStreams"));
        throw java_exception(ex);
    }
    else {
        options->announced_maximum_outgoing_streams = announcedMaximumOutgoingStreams;
    }
NATIVE_EPILOG
}


jlong JNICALL DcSctpOptions_class::getMtu(JNIEnv* env, jDcSctpOptions jOptions)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    return options->mtu;
NATIVE_EPILOG_Z
}

void JNICALL DcSctpOptions_class::setMtu(JNIEnv* env, jDcSctpOptions jOptions, jlong mtu)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    options->mtu = mtu;
NATIVE_EPILOG
}


jlong JNICALL DcSctpOptions_class::getMaxMessageSize(JNIEnv* env, jDcSctpOptions jOptions)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    return options->max_message_size;
NATIVE_EPILOG_Z
}

void JNICALL DcSctpOptions_class::setMaxMessageSize(JNIEnv* env, jDcSctpOptions jOptions, jlong maxMessageSize)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    options->max_message_size = maxMessageSize;
NATIVE_EPILOG
}


jshort JNICALL DcSctpOptions_class::getDefaultStreamPriority(JNIEnv* env, jDcSctpOptions jOptions)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    return *options->default_stream_priority;
NATIVE_EPILOG_Z
}

void JNICALL DcSctpOptions_class::setDefaultStreamPriority(JNIEnv* env, jDcSctpOptions jOptions, jshort defaultStreamPriority)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    options->default_stream_priority = StreamPriority(defaultStreamPriority);
NATIVE_EPILOG
}


jlong JNICALL DcSctpOptions_class::getMaxReceiverWindowBufferSize(JNIEnv* env, jDcSctpOptions jOptions)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    return options->max_receiver_window_buffer_size;
NATIVE_EPILOG_Z
}

void JNICALL DcSctpOptions_class::setMaxReceiverWindowBufferSize(JNIEnv* env, jDcSctpOptions jOptions, jlong maxReceiverWindowBufferSize)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    options->max_receiver_window_buffer_size = maxReceiverWindowBufferSize;
NATIVE_EPILOG
}


jlong JNICALL DcSctpOptions_class::getMaxSendBufferSize(JNIEnv* env, jDcSctpOptions jOptions)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    return options->max_send_buffer_size;
NATIVE_EPILOG_Z
}

void JNICALL DcSctpOptions_class::setMaxSendBufferSize(JNIEnv* env, jDcSctpOptions jOptions, jlong maxSendBufferSize)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    options->max_send_buffer_size = maxSendBufferSize;
NATIVE_EPILOG
}


jlong JNICALL DcSctpOptions_class::getPerStreamSendQueueLimit(JNIEnv* env, jDcSctpOptions jOptions)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    return options->per_stream_send_queue_limit;
NATIVE_EPILOG_Z
}

void JNICALL DcSctpOptions_class::setPerStreamSendQueueLimit(JNIEnv* env, jDcSctpOptions jOptions, jlong perStreamSendQueueLimit)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    options->per_stream_send_queue_limit = perStreamSendQueueLimit;
NATIVE_EPILOG
}


jlong JNICALL DcSctpOptions_class::getTotalBufferedAmountLowThreshold(JNIEnv* env, jDcSctpOptions jOptions)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    return options->total_buffered_amount_low_threshold;
NATIVE_EPILOG_Z
}

void JNICALL DcSctpOptions_class::setTotalBufferedAmountLowThreshold(JNIEnv* env, jDcSctpOptions jOptions, jlong totalBufferedAmountLowThreshold)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    options->total_buffered_amount_low_threshold = totalBufferedAmountLowThreshold;
NATIVE_EPILOG
}


jlong JNICALL DcSctpOptions_class::getRttMax(JNIEnv* env, jDcSctpOptions jOptions)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    return *options->rtt_max;
NATIVE_EPILOG_Z
}

void JNICALL DcSctpOptions_class::setRttMax(JNIEnv* env, jDcSctpOptions jOptions, jlong rttMax)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    options->rtt_max = DurationMs(rttMax);
NATIVE_EPILOG
}

jlong JNICALL DcSctpOptions_class::getRtoInitial(JNIEnv* env, jDcSctpOptions jOptions)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    return *options->rto_initial;
NATIVE_EPILOG_Z
}

void JNICALL DcSctpOptions_class::setRtoInitial(JNIEnv* env, jDcSctpOptions jOptions, jlong rtoInitial)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    options->rto_initial = DurationMs(rtoInitial);
NATIVE_EPILOG
}

jlong JNICALL DcSctpOptions_class::getRtoMax(JNIEnv* env, jDcSctpOptions jOptions)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    return *options->rto_max;
NATIVE_EPILOG_Z
}

void JNICALL DcSctpOptions_class::setRtoMax(JNIEnv* env, jDcSctpOptions jOptions, jlong rtoMax)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    options->rto_max = DurationMs(rtoMax);
NATIVE_EPILOG
}

jlong JNICALL DcSctpOptions_class::getRtoMin(JNIEnv* env, jDcSctpOptions jOptions)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    return *options->rto_min;
NATIVE_EPILOG_Z
}

void JNICALL DcSctpOptions_class::setRtoMin(JNIEnv* env, jDcSctpOptions jOptions, jlong rtoMin)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    options->rto_min = DurationMs(rtoMin);
NATIVE_EPILOG
}

jlong JNICALL DcSctpOptions_class::getT1InitTimeout(JNIEnv* env, jDcSctpOptions jOptions)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    return *options->t1_init_timeout;
NATIVE_EPILOG_Z
}

void JNICALL DcSctpOptions_class::setT1InitTimeout(JNIEnv* env, jDcSctpOptions jOptions, jlong t1InitTimeout)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    options->t1_init_timeout = DurationMs(t1InitTimeout);
NATIVE_EPILOG
}

jlong JNICALL DcSctpOptions_class::getT1CookieTimeout(JNIEnv* env, jDcSctpOptions jOptions)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    return *options->t1_cookie_timeout;
NATIVE_EPILOG_Z
}

void JNICALL DcSctpOptions_class::setT1CookieTimeout(JNIEnv* env, jDcSctpOptions jOptions, jlong t1CookieTimeout)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    options->t1_cookie_timeout = DurationMs(t1CookieTimeout);
NATIVE_EPILOG
}

jlong JNICALL DcSctpOptions_class::getT2ShutdownTimeout(JNIEnv* env, jDcSctpOptions jOptions)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    return *options->t2_shutdown_timeout;
NATIVE_EPILOG_Z
}

void JNICALL DcSctpOptions_class::setT2ShutdownTimeout(JNIEnv* env, jDcSctpOptions jOptions, jlong t2ShutdownTimeout)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    options->t2_shutdown_timeout = DurationMs(t2ShutdownTimeout);
NATIVE_EPILOG
}

jLong JNICALL DcSctpOptions_class::getMaxTimerBackoffDuration(JNIEnv* env, jDcSctpOptions jOptions)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    if (!options->max_timer_backoff_duration) {
        return nullptr;
    }
    return java_classes::get<Long_class>().ctor(env, **options->max_timer_backoff_duration).release();
NATIVE_EPILOG_Z
}

void JNICALL DcSctpOptions_class::setMaxTimerBackoffDuration(JNIEnv* env, jDcSctpOptions jOptions, jLong maxTimerBackoffDuration)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    if (maxTimerBackoffDuration != nullptr) {
        options->max_timer_backoff_duration =
            std::optional(DurationMs(java_classes::get<Long_class>().longValue(env, maxTimerBackoffDuration)));
    }
    else {
        options->max_timer_backoff_duration = std::optional<DurationMs>();
    }
NATIVE_EPILOG
}

jlong JNICALL DcSctpOptions_class::getHeartbeatInterval(JNIEnv* env, jDcSctpOptions jOptions)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    return *options->heartbeat_interval;
NATIVE_EPILOG_Z
}

void JNICALL DcSctpOptions_class::setHeartbeatInterval(JNIEnv* env, jDcSctpOptions jOptions, jlong heartbeatInterval)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    options->heartbeat_interval = DurationMs(heartbeatInterval);
NATIVE_EPILOG
}


jlong JNICALL DcSctpOptions_class::getDelayedAckMaxTimeout(JNIEnv* env, jDcSctpOptions jOptions)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    return *options->delayed_ack_max_timeout;
NATIVE_EPILOG_Z
}

void JNICALL DcSctpOptions_class::setDelayedAckMaxTimeout(JNIEnv* env, jDcSctpOptions jOptions, jlong delayedAckMaxTimeout)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    options->delayed_ack_max_timeout = DurationMs(delayedAckMaxTimeout);
NATIVE_EPILOG
}


jlong JNICALL DcSctpOptions_class::getMinRttVariance(JNIEnv* env, jDcSctpOptions jOptions)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    return *options->min_rtt_variance;
NATIVE_EPILOG_Z
}

void JNICALL DcSctpOptions_class::setMinRttVariance(JNIEnv* env, jDcSctpOptions jOptions, jlong minRttVariance)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    options->min_rtt_variance = DurationMs(minRttVariance);
NATIVE_EPILOG
}


jlong JNICALL DcSctpOptions_class::getCwndMtusInitial(JNIEnv* env, jDcSctpOptions jOptions)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    return options->cwnd_mtus_initial;
NATIVE_EPILOG_Z
}

void JNICALL DcSctpOptions_class::setCwndMtusInitial(JNIEnv* env, jDcSctpOptions jOptions, jlong cwndMtusInitial)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    options->cwnd_mtus_initial = cwndMtusInitial;
NATIVE_EPILOG
}


jlong JNICALL DcSctpOptions_class::getCwndMtusMin(JNIEnv* env, jDcSctpOptions jOptions)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    return options->cwnd_mtus_min;
NATIVE_EPILOG_Z
}

void JNICALL DcSctpOptions_class::setCwndMtusMin(JNIEnv* env, jDcSctpOptions jOptions, jlong cwndMtusMin)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    options->cwnd_mtus_min = cwndMtusMin;
NATIVE_EPILOG
}


jlong JNICALL DcSctpOptions_class::getAvoidFragmentationCwndMtus(JNIEnv* env, jDcSctpOptions jOptions)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    return options->avoid_fragmentation_cwnd_mtus;
NATIVE_EPILOG_Z
}

void JNICALL DcSctpOptions_class::setAvoidFragmentationCwndMtus(JNIEnv* env, jDcSctpOptions jOptions, jlong avoidFragmentationCwndMtus)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    options->avoid_fragmentation_cwnd_mtus = avoidFragmentationCwndMtus;
NATIVE_EPILOG
}


jint JNICALL DcSctpOptions_class::getMaxBurst(JNIEnv* env, jDcSctpOptions jOptions)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    return options->max_burst;
NATIVE_EPILOG_Z
}

void JNICALL DcSctpOptions_class::setMaxBurst(JNIEnv* env, jDcSctpOptions jOptions, jint maxBurst)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    options->max_burst = maxBurst;
NATIVE_EPILOG
}


jInteger JNICALL DcSctpOptions_class::getMaxRetransmissions(JNIEnv* env, jDcSctpOptions jOptions)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    return java_classes::get<Integer_class>().fromOptionalInt(env, options->max_retransmissions);
NATIVE_EPILOG_Z
}

void JNICALL DcSctpOptions_class::setMaxRetransmissions(JNIEnv* env, jDcSctpOptions jOptions, jInteger maxRetransmissions)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    options->max_retransmissions = java_classes::get<Integer_class>().toOptionalInt(env, maxRetransmissions);
NATIVE_EPILOG
}


jInteger JNICALL DcSctpOptions_class::getMaxInitRetransmits(JNIEnv* env, jDcSctpOptions jOptions)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    return java_classes::get<Integer_class>().fromOptionalInt(env, options->max_init_retransmits);
NATIVE_EPILOG_Z
}

void JNICALL DcSctpOptions_class::setMaxInitRetransmits(JNIEnv* env, jDcSctpOptions jOptions, jInteger maxInitRetransmits)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    options->max_init_retransmits = java_classes::get<Integer_class>().toOptionalInt(env, maxInitRetransmits);
NATIVE_EPILOG
}


jboolean JNICALL DcSctpOptions_class::isEnablePartialReliability(JNIEnv* env, jDcSctpOptions jOptions)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    return options->enable_partial_reliability ? java_true : java_false;
NATIVE_EPILOG_Z
}

void JNICALL DcSctpOptions_class::setEnablePartialReliability(JNIEnv* env, jDcSctpOptions jOptions, jboolean enablePartialReliability)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    options->enable_partial_reliability = enablePartialReliability ? true : false;
NATIVE_EPILOG
}


jboolean JNICALL DcSctpOptions_class::isEnableMessageInterleaving(JNIEnv* env, jDcSctpOptions jOptions)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    return options->enable_message_interleaving ? java_true : java_false;
NATIVE_EPILOG_Z
}

void JNICALL DcSctpOptions_class::setEnableMessageInterleaving(JNIEnv* env, jDcSctpOptions jOptions, jboolean enableMessageInterleaving)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    options->enable_message_interleaving = enableMessageInterleaving ? true : false;
NATIVE_EPILOG
}


jboolean JNICALL DcSctpOptions_class::isHeartbeatIntervalIncludeRtt(JNIEnv* env, jDcSctpOptions jOptions)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    return options->heartbeat_interval_include_rtt ? java_true : java_false;
NATIVE_EPILOG_Z
}

void JNICALL DcSctpOptions_class::setHeartbeatIntervalIncludeRtt(JNIEnv* env, jDcSctpOptions jOptions, jboolean heartbeatIntervalIncludeRtt)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    options->heartbeat_interval_include_rtt = heartbeatIntervalIncludeRtt ? true : false;
NATIVE_EPILOG
}


jboolean JNICALL DcSctpOptions_class::isDisableChecksumVerification(JNIEnv* env, jDcSctpOptions jOptions)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    return options->disable_checksum_verification ? java_true : java_false;
NATIVE_EPILOG_Z
}

void JNICALL DcSctpOptions_class::setDisableChecksumVerification(JNIEnv* env, jDcSctpOptions jOptions, jboolean disableChecksumVerification)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    options->disable_checksum_verification = disableChecksumVerification ? true : false;
NATIVE_EPILOG
}


jint JNICALL DcSctpOptions_class::getZeroChecksumAlternateErrorDetectionMethod(JNIEnv* env, jDcSctpOptions jOptions)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    return *options->zero_checksum_alternate_error_detection_method;
NATIVE_EPILOG_Z
}

void JNICALL DcSctpOptions_class::setZeroChecksumAlternateErrorDetectionMethod(JNIEnv* env, jDcSctpOptions jOptions, jint zeroChecksumAlternateErrorDetectionMethod)
{
NATIVE_PROLOG
    DcSctpOptions* options = (DcSctpOptions*)(intptr_t)java_classes::get<DcSctpOptions_class>().get_ptr(env, jOptions);

    options->zero_checksum_alternate_error_detection_method = ZeroChecksumAlternateErrorDetectionMethod(zeroChecksumAlternateErrorDetectionMethod);
NATIVE_EPILOG
}


void JNICALL Metrics_class::destruct(JNIEnv* env, jclass, jlong ptr)
{
NATIVE_PROLOG
    auto metrics = (Metrics*)(intptr_t)ptr;
    delete metrics;
NATIVE_EPILOG
}

jlong JNICALL Metrics_class::getTxPacketsCount(JNIEnv* env, jMetrics jmetrics)
{
NATIVE_PROLOG
    auto metrics = (Metrics*)(intptr_t)java_classes::get<Metrics_class>().get_ptr(env, jmetrics);
    return metrics->tx_packets_count;
NATIVE_EPILOG_Z
}

jlong JNICALL Metrics_class::getTxMessagesCount(JNIEnv* env, jMetrics jmetrics)
{
NATIVE_PROLOG
    auto metrics = (Metrics*)(intptr_t)java_classes::get<Metrics_class>().get_ptr(env, jmetrics);
    return metrics->tx_messages_count;
NATIVE_EPILOG_Z
}

jlong JNICALL Metrics_class::getRtxPacketsCount(JNIEnv* env, jMetrics jmetrics)
{
NATIVE_PROLOG
    auto metrics = (Metrics*)(intptr_t)java_classes::get<Metrics_class>().get_ptr(env, jmetrics);
    return metrics->rtx_packets_count;
NATIVE_EPILOG_Z
}

jlong JNICALL Metrics_class::getRtxBytesCount(JNIEnv* env, jMetrics jmetrics)
{
NATIVE_PROLOG
    auto metrics = (Metrics*)(intptr_t)java_classes::get<Metrics_class>().get_ptr(env, jmetrics);
    return metrics->rtx_bytes_count;
NATIVE_EPILOG_Z
}

jlong JNICALL Metrics_class::getCwndBytes(JNIEnv* env, jMetrics jmetrics)
{
NATIVE_PROLOG
    auto metrics = (Metrics*)(intptr_t)java_classes::get<Metrics_class>().get_ptr(env, jmetrics);
    return metrics->cwnd_bytes;
NATIVE_EPILOG_Z
}

jint JNICALL Metrics_class::getSrttMs(JNIEnv* env, jMetrics jmetrics)
{
NATIVE_PROLOG
    auto metrics = (Metrics*)(intptr_t)java_classes::get<Metrics_class>().get_ptr(env, jmetrics);
    return metrics->srtt_ms;
NATIVE_EPILOG_Z
}

jlong JNICALL Metrics_class::getUnackDataCount(JNIEnv* env, jMetrics jmetrics)
{
NATIVE_PROLOG
    auto metrics = (Metrics*)(intptr_t)java_classes::get<Metrics_class>().get_ptr(env, jmetrics);
    return metrics->unack_data_count;
NATIVE_EPILOG_Z
}

jlong JNICALL Metrics_class::getRxPacketsCount(JNIEnv* env, jMetrics jmetrics)
{
NATIVE_PROLOG
    auto metrics = (Metrics*)(intptr_t)java_classes::get<Metrics_class>().get_ptr(env, jmetrics);
    return metrics->rx_packets_count;
NATIVE_EPILOG_Z
}

jlong JNICALL Metrics_class::getRxMessagesCount(JNIEnv* env, jMetrics jmetrics)
{
NATIVE_PROLOG
    auto metrics = (Metrics*)(intptr_t)java_classes::get<Metrics_class>().get_ptr(env, jmetrics);
    return metrics->rx_messages_count;
NATIVE_EPILOG_Z
}

jint JNICALL Metrics_class::getPeerRwndBytes(JNIEnv* env, jMetrics jmetrics)
{
NATIVE_PROLOG
    auto metrics = (Metrics*)(intptr_t)java_classes::get<Metrics_class>().get_ptr(env, jmetrics);
    return metrics->peer_rwnd_bytes;
NATIVE_EPILOG_Z
}

jMetrics_SctpImplementation JNICALL Metrics_class::getPeerImplementation(JNIEnv* env, jMetrics jmetrics)
{
NATIVE_PROLOG
    auto metrics = (Metrics*)(intptr_t)java_classes::get<Metrics_class>().get_ptr(env, jmetrics);
    return enum_members::get<SctpImplementation_members>().map(env, metrics->peer_implementation);
NATIVE_EPILOG_Z
}

jboolean JNICALL Metrics_class::usesMessageInterleaving(JNIEnv* env, jMetrics jmetrics)
{
NATIVE_PROLOG
    auto metrics = (Metrics*)(intptr_t)java_classes::get<Metrics_class>().get_ptr(env, jmetrics);
    return metrics->uses_message_interleaving;
NATIVE_EPILOG_Z
}

jboolean JNICALL Metrics_class::usesZeroChecksum(JNIEnv* env, jMetrics jmetrics)
{
NATIVE_PROLOG
    auto metrics = (Metrics*)(intptr_t)java_classes::get<Metrics_class>().get_ptr(env, jmetrics);
    return metrics->uses_zero_checksum;
NATIVE_EPILOG_Z
}

jshort JNICALL Metrics_class::getNegotiatedMaximumIncomingStreams(JNIEnv* env, jMetrics jmetrics)
{
NATIVE_PROLOG
    auto metrics = (Metrics*)(intptr_t)java_classes::get<Metrics_class>().get_ptr(env, jmetrics);
    return metrics->negotiated_maximum_incoming_streams;
NATIVE_EPILOG_Z
}

jshort JNICALL Metrics_class::getNegotiatedMaximumOutgoingStreams(JNIEnv* env, jMetrics jmetrics)
{
NATIVE_PROLOG
    auto metrics = (Metrics*)(intptr_t)java_classes::get<Metrics_class>().get_ptr(env, jmetrics);
    return metrics->negotiated_maximum_outgoing_streams;
NATIVE_EPILOG_Z
}
