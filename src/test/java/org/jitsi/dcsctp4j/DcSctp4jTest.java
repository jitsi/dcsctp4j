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

import org.jitsi.utils.logging2.Logger;
import org.jitsi.utils.logging2.LoggerImpl;
import org.junit.jupiter.api.*;

import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.time.Clock;
import java.time.Instant;
import java.util.concurrent.*;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

import static org.junit.jupiter.api.Assertions.assertNotNull;

public class DcSctp4jTest {
    public static ScheduledExecutorService executor = Executors.newSingleThreadScheduledExecutor();

    @Test
    public void testLoad() {
        // Very basic test to verify that we can load and initialize the library.
        DcSctpSocketFactory factory = new DcSctpSocketFactory();
        assertNotNull(factory);
    }

    @Test
    public void testSocketCreate() {
        DcSctpSocketFactory factory = new DcSctpSocketFactory();
        SocketWrapper wrapper = new SocketWrapper();
        Logger logger = new LoggerImpl("testSocketCreate");
        TestCallbacks callbacks = new TestCallbacks(wrapper, Clock.systemUTC(), logger);
        DcSctpOptions options = new DcSctpOptions();

        wrapper.socket = factory.create("testSocketCreate", callbacks, null, options);
        assertNotNull(wrapper.socket);
    }

    @Test
    public void testSimpleConnection() {
        DcSctpSocketFactory factory = new DcSctpSocketFactory();

        SocketWrapper server = new SocketWrapper();
        SocketWrapper client = new SocketWrapper();

        Logger clientLogger = new LoggerImpl(this.getClass().getName() + ".simpleConnectionClient");
        clientLogger.addContext("name", "simpleConnectionClient");

        Logger serverLogger = new LoggerImpl(this.getClass().getName() + ".simpleConnectionServer");
        serverLogger.addContext("name", "simpleConnectionServer");

        TestCallbacks clientCallbacks = new TestCallbacks(client, Clock.systemUTC(), clientLogger) {
            @Override
            public SendPacketStatus sendPacketWithStatus(ByteBuffer data) {
                super.sendPacketWithStatus(data);
                ByteBuffer clone = cloneByteBuffer(data);
                try {
                    executor.submit(() -> server.socket.receivePacket(clone));
                }
                catch (Exception e) {
                    clientLogger.error("Error submitting packet to server", e);
                    return SendPacketStatus.kError;
                }
                return SendPacketStatus.kSuccess;
            }
        };
        TestCallbacks serverCallbacks = new TestCallbacks(client, Clock.systemUTC(), serverLogger) {
            @Override
            public SendPacketStatus sendPacketWithStatus(ByteBuffer data) {
                super.sendPacketWithStatus(data);
                ByteBuffer clone = cloneByteBuffer(data);
                try {
                    executor.submit(() -> client.socket.receivePacket(clone));
                }
                catch (Exception e) {
                    serverLogger.error("Error submitting packet to client", e);
                    return SendPacketStatus.kError;
                }
                return SendPacketStatus.kSuccess;
            }
        };
        DcSctpOptions options = new DcSctpOptions();

        server.socket =
                factory.create("simpleConnectionServer", serverCallbacks, null, options);
        client.socket =
                factory.create("simpleConnectionClient", clientCallbacks, null, options);

        client.socket.connect();
        try {
            Thread.sleep(100);
        } catch (InterruptedException ignored) {
        }
        DcSctpMessage message = new DcSctpMessage((short) 0, 0, "Hello".getBytes(StandardCharsets.UTF_8));
        SendOptions sendOptions = new SendOptions();
        client.socket.send(message, sendOptions);
        try {
            Thread.sleep(1000);
        } catch (InterruptedException ignored) {
        }
        client.socket.close();
        try {
            Thread.sleep(100);
        } catch (InterruptedException ignored) {
        }
    }

    public static ByteBuffer cloneByteBuffer(final ByteBuffer original) {
        // Create clone with same capacity as original.
        final ByteBuffer clone = (original.isDirect()) ?
                ByteBuffer.allocateDirect(original.capacity()) :
                ByteBuffer.allocate(original.capacity());

        clone.put(original);

        return clone;
    }

}



class SocketWrapper
{
    public DcSctpSocketInterface socket;
}

class TestTimeout implements Timeout, Runnable {
    SocketWrapper wrapper;
    long timeoutId;
    ScheduledFuture<?> future;

    TestTimeout(SocketWrapper wrapper)
    {
        this.wrapper = wrapper;
    }

    @Override
    public void start(long duration, long timeoutId) {
        this.timeoutId = timeoutId;
        future = DcSctp4jTest.executor.schedule(this, duration, TimeUnit.MILLISECONDS);
    }

    @Override
    public void stop() {
        future.cancel(false);
    }

    @Override
    public void run() {
        wrapper.socket.handleTimeout(timeoutId);
    }
}

class TestCallbacks implements DcSctpSocketCallbacks {
    private final Clock clock;
    private final SocketWrapper wrapper;
    private final Logger logger;

    public TestCallbacks(SocketWrapper wrapper, Clock clock, Logger logger)
    {
        this.wrapper = wrapper;
        this.clock = clock;
        this.logger = logger;
    }

    @Override
    public SendPacketStatus sendPacketWithStatus(ByteBuffer data) {
        logger.info("Sending " + data.limit() + " byte packet");
        return SendPacketStatus.kTemporaryFailure;
    }

    @Override
    public Timeout createTimeout(DelayPrecision precision) {
        return new TestTimeout(wrapper);
    }

    @Override
    public Instant Now() {
        return clock.instant();
    }

    @Override
    public long getRandomInt(long low, long high) {
        return ThreadLocalRandom.current().nextLong(low, high);
    }

    @Override
    public void OnMessageReceived(DcSctpMessage message) {
        String s = StandardCharsets.UTF_8.decode(message.getPayload()).toString();

        logger.info("Message received: " +
                "streamID " + message.getStreamID() +
                ", PPID " + message.getPpid() +
                ", payload " + s);
    }

    @Override
    public void OnError(ErrorKind error, String message) {
        logger.info("Error received: " +
                "errorKind " + error +
                ", message " + message);
    }

    @Override
    public void OnAborted(ErrorKind error, String message) {
        logger.info("SCTP connection aborted: " +
                "errorKind " + error +
                ", message " + message);
    }

    @Override
    public void OnConnected() {
        logger.info("SCTP connection connected");
    }

    @Override
    public void OnClosed() {
        logger.info("SCTP connection closed");
    }

    @Override
    public void OnConnectionRestarted() {
        logger.info("SCTP connection restarted");
    }

    private static String join(short[] shorts)
    {
        return IntStream.range(0, shorts.length).
                map(i -> (int)shorts[i]).
                mapToObj(Integer::toString).
                collect(Collectors.joining(", "));
    }

    @Override
    public void OnStreamsResetFailed(short[] outgoing_streams, String reason) {
        logger.info("Stream reset failed for outgoing streams " + join(outgoing_streams) +
                ": " + reason);
    }

    @Override
    public void OnStreamsResetPerformed(short[] outgoing_streams) {
        logger.info("Stream reset performed for outgoing streams " + join(outgoing_streams));
    }

    @Override
    public void OnIncomingStreamsReset(short[] incoming_streams) {
        logger.info("Incoming streams reset for streams " + join(incoming_streams));
    }
}
