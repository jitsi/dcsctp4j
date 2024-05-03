#ifndef ENUM_CLASSES_H
#define ENUM_CLASSES_H

#include "type_mapping.h"
#include <net/dcsctp/public/dcsctp_socket_factory.h>

/* jnigen doesn't handle Java enum classes, so do these by hand. */

template <typename E> smjni::global_java_ref<E> enum_field(JNIEnv* env, smjni::java_class<E> clazz, const char* name);

class ErrorKind_class : public smjni::java_runtime::simple_java_class<jErrorKind>
{
public:
    ErrorKind_class(JNIEnv * env);

    jErrorKind kNoError() const { return m_kNoError.c_ptr(); }
    jErrorKind kTooManyRetries() const { return m_kTooManyRetries.c_ptr(); }
    jErrorKind kNotConnected() const { return m_kNotConnected.c_ptr(); }
    jErrorKind kParseFailed() const { return m_kParseFailed.c_ptr(); }
    jErrorKind kWrongSequence() const { return m_kWrongSequence.c_ptr(); }
    jErrorKind kPeerReported() const { return m_kPeerReported.c_ptr(); }
    jErrorKind kProtocolViolation() const { return m_kProtocolViolation.c_ptr(); }
    jErrorKind kResourceExhaustion() const { return m_kResourceExhaustion.c_ptr(); }
    jErrorKind kUnsupportedOperation() const { return m_kUnsupportedOperation.c_ptr(); }

private:
    smjni::global_java_ref<jErrorKind> m_kNoError;
    smjni::global_java_ref<jErrorKind> m_kTooManyRetries;
    smjni::global_java_ref<jErrorKind> m_kNotConnected;
    smjni::global_java_ref<jErrorKind> m_kParseFailed;
    smjni::global_java_ref<jErrorKind> m_kWrongSequence;
    smjni::global_java_ref<jErrorKind> m_kPeerReported;
    smjni::global_java_ref<jErrorKind> m_kProtocolViolation;
    smjni::global_java_ref<jErrorKind> m_kResourceExhaustion;
    smjni::global_java_ref<jErrorKind> m_kUnsupportedOperation;

public:
    jErrorKind map(JNIEnv *, dcsctp::ErrorKind) const;
};

class ResetStreamsStatus_class : public smjni::java_runtime::simple_java_class<jResetStreamsStatus>
{
public:
    ResetStreamsStatus_class(JNIEnv * env);

    jResetStreamsStatus kNotConnected() const { return m_kNotConnected.c_ptr(); }
    jResetStreamsStatus kPerformed() const { return m_kPerformed.c_ptr(); }
    jResetStreamsStatus kNotSupported() const { return m_kNotSupported.c_ptr(); }

private:
    smjni::global_java_ref<jResetStreamsStatus> m_kNotConnected;
    smjni::global_java_ref<jResetStreamsStatus> m_kPerformed;
    smjni::global_java_ref<jResetStreamsStatus> m_kNotSupported;

public:
    jResetStreamsStatus map(JNIEnv *, dcsctp::ResetStreamsStatus) const;
};

class SendPacketStatus_class : public smjni::java_runtime::simple_java_class<jSendPacketStatus>
{
public:
    SendPacketStatus_class(JNIEnv * env);

    jSendPacketStatus kSuccess() const { return m_kSuccess.c_ptr(); }
    jSendPacketStatus kTemporaryFailure() const { return m_kTemporaryFailure.c_ptr(); };
    jSendPacketStatus kError() const { return m_kError.c_ptr(); }

private:
    smjni::global_java_ref<jSendPacketStatus> m_kSuccess;
    smjni::global_java_ref<jSendPacketStatus> m_kTemporaryFailure;
    smjni::global_java_ref<jSendPacketStatus> m_kError;

public:
    jSendPacketStatus map(JNIEnv *, dcsctp::SendPacketStatus) const;
    dcsctp::SendPacketStatus map(JNIEnv *, jSendPacketStatus) const;
};


class SendStatus_class : public smjni::java_runtime::simple_java_class<jSendStatus>
{
public:
    SendStatus_class(JNIEnv * env);

    jSendStatus kSuccess() const { return m_kSuccess.c_ptr(); }
    jSendStatus kErrorMessageEmpty() const { return m_kErrorMessageEmpty.c_ptr(); };
    jSendStatus kErrorMessageTooLarge() const { return m_kErrorMessageTooLarge.c_ptr(); }
    jSendStatus kErrorResourceExhaustion() const { return m_kErrorResourceExhaustion.c_ptr(); }
    jSendStatus kErrorShuttingDown() const { return m_kErrorShuttingDown.c_ptr(); }

private:
    smjni::global_java_ref<jSendStatus> m_kSuccess;
    smjni::global_java_ref<jSendStatus> m_kErrorMessageEmpty;
    smjni::global_java_ref<jSendStatus> m_kErrorMessageTooLarge;
    smjni::global_java_ref<jSendStatus> m_kErrorResourceExhaustion;
    smjni::global_java_ref<jSendStatus> m_kErrorShuttingDown;

public:
    jSendStatus map(JNIEnv *, dcsctp::SendStatus) const;
};


class SocketState_class : public smjni::java_runtime::simple_java_class<jSocketState>
{
public:
    SocketState_class(JNIEnv * env);

    jSocketState kClosed() const { return m_kClosed.c_ptr(); }
    jSocketState kConnecting() const { return m_kConnecting.c_ptr(); }
    jSocketState kConnected() const { return m_kConnected.c_ptr(); }
    jSocketState kShuttingDown() const { return m_kShuttingDown.c_ptr(); }

private:
    smjni::global_java_ref<jSocketState> m_kClosed;
    smjni::global_java_ref<jSocketState> m_kConnecting;
    smjni::global_java_ref<jSocketState> m_kConnected;
    smjni::global_java_ref<jSocketState> m_kShuttingDown;

public:
    jSocketState map(JNIEnv*, dcsctp::SocketState) const;
};

#define ENUM_CLASSES ErrorKind_class, ResetStreamsStatus_class, SendPacketStatus_class, SendStatus_class, SocketState_class
#endif
