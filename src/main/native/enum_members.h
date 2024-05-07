#ifndef ENUM_MEMBERS_H
#define ENUM_MEMBERS_H

#include "type_mapping.h"
#include <net/dcsctp/public/dcsctp_socket_factory.h>

/* Wrappers for the actual members defined by enum classes. */

class DelayPrecision_members
{
public:
    DelayPrecision_members(JNIEnv * env);

    jDcSctpSocketCallbacks_DelayPrecision kLow() const { return m_kLow.c_ptr(); }
    jDcSctpSocketCallbacks_DelayPrecision kHigh() const { return m_kHigh.c_ptr(); }

private:
    smjni::global_java_ref<jDcSctpSocketCallbacks_DelayPrecision> m_kLow;
    smjni::global_java_ref<jDcSctpSocketCallbacks_DelayPrecision> m_kHigh;

public:
    jDcSctpSocketCallbacks_DelayPrecision map(JNIEnv *, webrtc::TaskQueueBase::DelayPrecision) const;
};

class ErrorKind_members
{
public:
    ErrorKind_members(JNIEnv * env);

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

class ResetStreamsStatus_members
{
public:
    ResetStreamsStatus_members(JNIEnv * env);

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

class SendPacketStatus_members
{
public:
    SendPacketStatus_members(JNIEnv * env);

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


class SendStatus_members
{
public:
    SendStatus_members(JNIEnv * env);

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


class SocketState_members
{
public:
    SocketState_members(JNIEnv * env);

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

template<typename... Classes> class enum_members_table
{
private:
    struct table : public std::tuple<Classes...>
    {
        table(JNIEnv * env) : std::tuple<Classes...>(std::forward<Classes>(env)...)
        { }
    };

public:
    static void init(JNIEnv* env) {
        instance = new enum_members_table(env);
    }
    template<typename T>
        static const T & get() { return std::get<T>(instance->m_table); }
private:
    enum_members_table(JNIEnv* env):
    m_table(env)
    { }

    table m_table;
    static enum_members_table* instance;
};

#define ENUM_MEMBERS DelayPrecision_members, ErrorKind_members, ResetStreamsStatus_members, SendPacketStatus_members, SendStatus_members, SocketState_members

template<typename... Classes>
    enum_members_table<Classes...> * enum_members_table<Classes...>::instance = nullptr;

using enum_members = enum_members_table<ENUM_MEMBERS>;

#endif
