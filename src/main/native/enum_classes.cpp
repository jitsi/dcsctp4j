#include "enum_classes.h"

using namespace smjni;
using namespace dcsctp;


template <typename E> global_java_ref<E> enum_field(JNIEnv* env, java_class<E> clazz, const char* name)
{
    java_static_field<E, E> field(env, clazz, name);
    return field.get(env, clazz);
}

DelayPrecision_class::DelayPrecision_class(JNIEnv * env) :
    simple_java_class(env),
    m_kLow(enum_field<jDcSctpSocketCallbacks_DelayPrecision>(env, *this, "kLow")),
    m_kHigh(enum_field<jDcSctpSocketCallbacks_DelayPrecision>(env, *this, "kHigh"))
{}

jDcSctpSocketCallbacks_DelayPrecision DelayPrecision_class::map(JNIEnv * env, webrtc::TaskQueueBase::DelayPrecision delayPrecision) const
{
    switch (delayPrecision) {
    case webrtc::TaskQueueBase::DelayPrecision::kLow:
        return kLow();
    case webrtc::TaskQueueBase::DelayPrecision::kHigh:
        return kHigh();
    default:
        auto ex = java_runtime::throwable().ctor(env, java_string_create(env, "Invalid value for DelayPrecision"));
        throw java_exception(ex);
    }
}


ErrorKind_class::ErrorKind_class(JNIEnv * env) :
    simple_java_class(env),
    m_kNoError(enum_field<jErrorKind>(env, *this, "kNoError")),
    m_kTooManyRetries(enum_field<jErrorKind>(env, *this, "kTooManyRetries")),
    m_kNotConnected(enum_field<jErrorKind>(env, *this, "kNotConnected")),
    m_kParseFailed(enum_field<jErrorKind>(env, *this, "kParseFailed")),
    m_kWrongSequence(enum_field<jErrorKind>(env, *this, "kWrongSequence")),
    m_kPeerReported(enum_field<jErrorKind>(env, *this, "kPeerReported")),
    m_kProtocolViolation(enum_field<jErrorKind>(env, *this, "kProtocolViolation")),
    m_kResourceExhaustion(enum_field<jErrorKind>(env, *this, "kResourceExhaustion")),
    m_kUnsupportedOperation(enum_field<jErrorKind>(env, *this, "kUnsupportedOperation"))
{}

jErrorKind ErrorKind_class::map(JNIEnv * env, ErrorKind errorKind) const
{
    switch (errorKind) {
    case ErrorKind::kNoError:
        return kNoError();
    case ErrorKind::kTooManyRetries:
        return kTooManyRetries();
    case ErrorKind::kNotConnected:
        return kNotConnected();
    case ErrorKind::kParseFailed:
        return kParseFailed();
    case ErrorKind::kWrongSequence:
        return kWrongSequence();
    case ErrorKind::kPeerReported:
        return kPeerReported();
    case ErrorKind::kProtocolViolation:
        return kProtocolViolation();
    case ErrorKind::kResourceExhaustion:
        return kResourceExhaustion();
    case ErrorKind::kUnsupportedOperation:
        return kUnsupportedOperation();
    default:
        auto ex = java_runtime::throwable().ctor(env, java_string_create(env, "Invalid value for ErrorKind"));
        throw java_exception(ex);
    }
}


ResetStreamsStatus_class::ResetStreamsStatus_class(JNIEnv * env) :
    simple_java_class(env),
    m_kNotConnected(enum_field<jResetStreamsStatus>(env, *this, "kNotConnected")),
    m_kPerformed(enum_field<jResetStreamsStatus>(env, *this, "kPerformed")),
    m_kNotSupported(enum_field<jResetStreamsStatus>(env, *this, "kNotSupported"))
{}

jResetStreamsStatus ResetStreamsStatus_class::map(JNIEnv *env, ResetStreamsStatus resetStreamsStatus) const
{
    switch (resetStreamsStatus) {
    case ResetStreamsStatus::kNotConnected:
        return kNotConnected();
    case ResetStreamsStatus::kPerformed:
        return kPerformed();
    case ResetStreamsStatus::kNotSupported:
        return kNotSupported();
    default:
        auto ex = java_runtime::throwable().ctor(env, java_string_create(env, "Invalid value for ResetStreamsStatus"));
        throw java_exception(ex);
    }
}


SendPacketStatus_class::SendPacketStatus_class(JNIEnv * env) :
    simple_java_class(env),
    m_kSuccess(enum_field<jSendPacketStatus>(env, *this, "kSuccess")),
    m_kTemporaryFailure(enum_field<jSendPacketStatus>(env, *this, "kTemporaryFailure")),
    m_kError(enum_field<jSendPacketStatus>(env, *this, "kError"))
{}

jSendPacketStatus SendPacketStatus_class::map(JNIEnv* env, SendPacketStatus sendStatus) const
{
    switch (sendStatus) {
    case SendPacketStatus::kSuccess:
        return kSuccess();
    case SendPacketStatus::kTemporaryFailure:
        return kTemporaryFailure();
    case SendPacketStatus::kError:
        return kError();
    default:
        auto ex = java_runtime::throwable().ctor(env, java_string_create(env, "Invalid value for SendPacketStatus"));
        throw java_exception(ex);
    }
}

SendPacketStatus SendPacketStatus_class::map(JNIEnv* env, jSendPacketStatus sendStatus) const
{
    if (env->IsSameObject(sendStatus, kSuccess())) {
        return SendPacketStatus::kSuccess;
    } else if (env->IsSameObject(sendStatus, kTemporaryFailure())) {
        return SendPacketStatus::kTemporaryFailure;
    } else if (env->IsSameObject(sendStatus, kError())) {
        return SendPacketStatus::kError;
    }
    else {
        auto ex = java_runtime::throwable().ctor(env, java_string_create(env, "Invalid value for SendPacketStatus"));
        throw java_exception(ex);
    }
}



SendStatus_class::SendStatus_class(JNIEnv * env) :
    simple_java_class(env),
    m_kSuccess(enum_field<jSendStatus>(env, *this, "kSuccess")),
    m_kErrorMessageEmpty(enum_field<jSendStatus>(env, *this, "kErrorMessageEmpty")),
    m_kErrorMessageTooLarge(enum_field<jSendStatus>(env, *this, "kErrorMessageTooLarge")),
    m_kErrorResourceExhaustion(enum_field<jSendStatus>(env, *this, "kErrorResourceExhaustion")),
    m_kErrorShuttingDown(enum_field<jSendStatus>(env, *this, "kErrorShuttingDown"))
{}


jSendStatus SendStatus_class::map(JNIEnv* env, SendStatus sendStatus) const
{
    switch (sendStatus) {
    case SendStatus::kSuccess:
        return kSuccess();
    case SendStatus::kErrorMessageEmpty:
        return kErrorMessageEmpty();
    case SendStatus::kErrorMessageTooLarge:
        return kErrorMessageTooLarge();
    case SendStatus::kErrorResourceExhaustion:
        return kErrorResourceExhaustion();
    case SendStatus::kErrorShuttingDown:
        return kErrorShuttingDown();
    default:
        auto ex = java_runtime::throwable().ctor(env, java_string_create(env, "Invalid value for SendStatus"));
        throw java_exception(ex);
    }
}


SocketState_class::SocketState_class(JNIEnv * env) :
    simple_java_class(env),
    m_kClosed(enum_field<jSocketState>(env, *this, "kClosed")),
    m_kConnecting(enum_field<jSocketState>(env, *this, "kConnecting")),
    m_kConnected(enum_field<jSocketState>(env, *this, "kConnected")),
    m_kShuttingDown(enum_field<jSocketState>(env, *this, "kShuttingDown"))
{}

jSocketState SocketState_class::map(JNIEnv *env, SocketState socketState) const
{
    switch (socketState) {
    case SocketState::kClosed:
        return kClosed();
    case SocketState::kConnecting:
        return kConnecting();
    case SocketState::kConnected:
        return kConnected();
    case SocketState::kShuttingDown:
        return kShuttingDown();
    default:
        auto ex = java_runtime::throwable().ctor(env, java_string_create(env, "Invalid value for SocketState"));
        throw java_exception(ex);
    }
}
