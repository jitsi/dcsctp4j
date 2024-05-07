#include "dcsctp4j.h"

using namespace smjni;
using namespace dcsctp;

template <typename E> global_java_ref<E> enum_field(JNIEnv* env, java_class<E> clazz, const char* name)
{
    java_static_field<E, E> field(env, clazz, name);
    return field.get(env, clazz);
}


DelayPrecision_members::DelayPrecision_members(JNIEnv * env) :
    m_kLow(java_classes::get<DcSctpSocketCallbacks_DelayPrecision_class>().get_kLow(env)),
    m_kHigh(java_classes::get<DcSctpSocketCallbacks_DelayPrecision_class>().get_kHigh(env))
{}

jDcSctpSocketCallbacks_DelayPrecision DelayPrecision_members::map(JNIEnv * env, webrtc::TaskQueueBase::DelayPrecision delayPrecision) const
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


ErrorKind_members::ErrorKind_members(JNIEnv * env) :
    m_kNoError(java_classes::get<ErrorKind_class>().get_kNoError(env)),
    m_kTooManyRetries(java_classes::get<ErrorKind_class>().get_kTooManyRetries(env)),
    m_kNotConnected(java_classes::get<ErrorKind_class>().get_kNotConnected(env)),
    m_kParseFailed(java_classes::get<ErrorKind_class>().get_kParseFailed(env)),
    m_kWrongSequence(java_classes::get<ErrorKind_class>().get_kWrongSequence(env)),
    m_kPeerReported(java_classes::get<ErrorKind_class>().get_kPeerReported(env)),
    m_kProtocolViolation(java_classes::get<ErrorKind_class>().get_kProtocolViolation(env)),
    m_kResourceExhaustion(java_classes::get<ErrorKind_class>().get_kResourceExhaustion(env)),
    m_kUnsupportedOperation(java_classes::get<ErrorKind_class>().get_kUnsupportedOperation(env))
{}

jErrorKind ErrorKind_members::map(JNIEnv * env, ErrorKind errorKind) const
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


ResetStreamsStatus_members::ResetStreamsStatus_members(JNIEnv * env) :
    m_kNotConnected(java_classes::get<ResetStreamsStatus_class>().get_kNotConnected(env)),
    m_kPerformed(java_classes::get<ResetStreamsStatus_class>().get_kPerformed(env)),
    m_kNotSupported(java_classes::get<ResetStreamsStatus_class>().get_kNotSupported(env))
{}

jResetStreamsStatus ResetStreamsStatus_members::map(JNIEnv *env, ResetStreamsStatus resetStreamsStatus) const
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


SendPacketStatus_members::SendPacketStatus_members(JNIEnv * env) :
    m_kSuccess(java_classes::get<SendPacketStatus_class>().get_kSuccess(env)),
    m_kTemporaryFailure(java_classes::get<SendPacketStatus_class>().get_kTemporaryFailure(env)),
    m_kError(java_classes::get<SendPacketStatus_class>().get_kError(env))
{}

jSendPacketStatus SendPacketStatus_members::map(JNIEnv* env, SendPacketStatus sendStatus) const
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

SendPacketStatus SendPacketStatus_members::map(JNIEnv* env, jSendPacketStatus sendStatus) const
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



SendStatus_members::SendStatus_members(JNIEnv * env) :
    m_kSuccess(java_classes::get<SendStatus_class>().get_kSuccess(env)),
    m_kErrorMessageEmpty(java_classes::get<SendStatus_class>().get_kErrorMessageEmpty(env)),
    m_kErrorMessageTooLarge(java_classes::get<SendStatus_class>().get_kErrorMessageTooLarge(env)),
    m_kErrorResourceExhaustion(java_classes::get<SendStatus_class>().get_kErrorResourceExhaustion(env)),
    m_kErrorShuttingDown(java_classes::get<SendStatus_class>().get_kErrorShuttingDown(env))
{}


jSendStatus SendStatus_members::map(JNIEnv* env, SendStatus sendStatus) const
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


SocketState_members::SocketState_members(JNIEnv * env) :
    m_kClosed(java_classes::get<SocketState_class>().get_kClosed(env)),
    m_kConnecting(java_classes::get<SocketState_class>().get_kConnecting(env)),
    m_kConnected(java_classes::get<SocketState_class>().get_kConnected(env)),
    m_kShuttingDown(java_classes::get<SocketState_class>().get_kShuttingDown(env))
{}

jSocketState SocketState_members::map(JNIEnv *env, SocketState socketState) const
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
