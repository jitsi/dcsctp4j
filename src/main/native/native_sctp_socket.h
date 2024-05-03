#ifndef NATIVE_SCTP_SOCKET_H
#define NATIVE_SCTP_SOCKET_H

#include <smjni/smjni.h>
#include "all_classes.h"
#include "wrapped_objects.h"

struct NativeSctpSocket {
    NativeSctpSocket(jDcSctpSocketCallbacks jCallbacks);

    WrappedSocketCallbacks callbacks;
    std::unique_ptr<dcsctp::DcSctpSocketInterface> socket;
};

dcsctp::DcSctpMessage convertDcSctpMessage(JNIEnv* env, jDcSctpMessage jMessage);
dcsctp::SendOptions convertSendOptions(JNIEnv* env, jSendOptions jOptions);

#endif
