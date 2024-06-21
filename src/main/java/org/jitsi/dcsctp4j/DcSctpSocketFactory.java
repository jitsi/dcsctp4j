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
import org.jetbrains.annotations.Nullable;
import smjni.jnigen.ExposeToNative;

import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;

@ExposeToNative
public class DcSctpSocketFactory
{
    static
    {
        DcSctp4j.init();
    }

    private final long ptr;

    public DcSctpSocketFactory()
    {
        ptr = construct();
        long ptrCopy = ptr;
        DcSctp4j.CLEANER.register(this, () -> destruct(ptrCopy));
    }

    private static native long construct();

    private static native void destruct(long ptr);

    public DcSctpSocketInterface create(
            @NotNull String logPrefix,
            @NotNull DcSctpSocketCallbacks callbacks,
            @Nullable PacketObserver packetObserver,
            @NotNull DcSctpOptions options)
    {
        long socketPtr = create_(ptr, logPrefix, callbacks, packetObserver, options);
        return new NativeSctpSocket(callbacks, packetObserver, socketPtr);
    }

    private native long create_(
            long ptr,
            @NotNull String logPrefix,
            @NotNull DcSctpSocketCallbacks callbacks,
            @Nullable PacketObserver packetObserver,
            @NotNull DcSctpOptions options);

    @ExposeToNative
    static private class NativeSctpSocket implements DcSctpSocketInterface
    {
        @NotNull DcSctpSocketCallbacks callbacks;
        @Nullable PacketObserver packetObserver;

        private final long ptr;
        NativeSctpSocket(@NotNull DcSctpSocketCallbacks callbacks, @Nullable PacketObserver packetObserver, long ptr)
        {
            this.callbacks = callbacks;
            this.packetObserver = packetObserver;
            this.ptr = ptr;
            DcSctp4j.CLEANER.register(this, () -> destruct(ptr));
        }

        private static native void destruct(long ptr);

        @Override
        public synchronized void receivePacket(byte @NotNull [] data, int offset, int length)
        {
            if (offset + length > data.length)
            {
                throw new IllegalArgumentException("Array length " + data.length +
                        " too short for offset " + offset + " length " + length);
            }
            receivePacket_(ptr, data, offset, length);
        }

        private native void receivePacket_(long ptr, byte[] data, int offset, int length);

        @Override
        public synchronized void handleTimeout(long timeoutId)
        {
            handleTimeout_(ptr, timeoutId);
        }

        private native void handleTimeout_(long ptr, long timeoutId);

        @Override
        public synchronized void connect()
        {
            connect_(ptr);
        }

        private native void connect_(long ptr);

        @Override
        public synchronized void shutdown()
        {
            shutdown_(ptr);
        }

        private native void shutdown_(long ptr);

        @Override
        public synchronized void close()
        {
            close_(ptr);
        }

        private native void close_(long ptr);

        @NotNull
        @Override
        public synchronized SocketState state()
        {
            return state_(ptr);
        }

        private native SocketState state_(long ptr);

        @NotNull
        @Override
        public synchronized DcSctpOptions options()
        {
            return options_(ptr);
        }

        private native DcSctpOptions options_(long ptr);

        @Override
        public synchronized void setStreamPriority(short streamId, short streamPriority)
        {
            setStreamPriority_(ptr, streamId, streamPriority);
        }

        private native void setStreamPriority_(long ptr, short streamId, short streamPriority);

        @Override
        public synchronized short getStreamPriority(short streamId)
        {
            return getStreamPriority_(ptr, streamId);
        }

        private native short getStreamPriority_(long ptr, short streamId);

        @NotNull
        @Override
        public synchronized SendStatus send(@NotNull DcSctpMessage message, @NotNull SendOptions options)
        {
            /* This is expected to be one of the most common methods, so unwrap the java objects to minimize the JNI
             * overhead */
            int nativeStatus = send_(ptr,
                    message.getPayload(), message.getPpid(), message.getStreamID(),
                    options.isUnordered, options.lifetime, options.maxRetransmissions, options.lifecycleId);
            return SendStatus.fromNativeStatus(nativeStatus);
        }

        private native int send_(long ptr,
                                 byte[] payload, int ppid, short streamID,
                                 boolean isUnordered, Long lifetime, Long maxRetransmissions, long lifecycleId);

        @NotNull
        @Override
        public synchronized List<SendStatus> sendMany(@NotNull List<@NotNull DcSctpMessage> messages,
                                                      @NotNull SendOptions options)
        {
            int[] nativeStatuses = sendMany_(ptr, messages.toArray(new DcSctpMessage[0]), options);
            return Arrays.stream(nativeStatuses).mapToObj(SendStatus::fromNativeStatus).collect(Collectors.toList());
        }

        private native int[] sendMany_(long ptr, DcSctpMessage[] messages, SendOptions options);

        /* Streams API doesn't include ShortStream, so do this manually. */
        private short[] unboxList(List<Short> l)
        {
            short[] ret = new short[l.size()];
            int idx = 0;
            for (short s: l) {
                ret[idx++] = s;
            }
            return ret;
        }

        @NotNull
        @Override
        public synchronized ResetStreamsStatus resetStreams(@NotNull List<Short> outgoingStreams)
        {
            return resetStreams_(ptr, unboxList(outgoingStreams));
        }

        private native ResetStreamsStatus resetStreams_(long ptr, short[] outgoingStreams);

        @Override
        public synchronized long bufferedAmount(short streamId)
        {
            return bufferedAmount_(ptr, streamId);
        }

        private native long bufferedAmount_(long ptr, short streamId);

        @Override
        public synchronized long bufferedAmountLowThreshold(short streamId)
        {
            return bufferedAmountLowThreshold_(ptr, streamId);
        }

        private native long bufferedAmountLowThreshold_(long ptr, short streamId);

        @Override
        public synchronized void setBufferedAmountLowThreshold(short streamId, long bytes)
        {
            setBufferedAmountLowThreshold_(ptr, streamId, bytes);
        }

        private native void setBufferedAmountLowThreshold_(long ptr, short streamId, long bytes);

        @Nullable
        @Override
        public synchronized Metrics getMetrics()
        {
            return getMetrics_(ptr);
        }

        private native Metrics getMetrics_(long ptr);
    }
}
