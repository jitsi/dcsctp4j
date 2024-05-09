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

import static org.junit.jupiter.api.Assertions.*;

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
        TestCallbacks callbacks = new TestCallbacks(wrapper, Clock.systemUTC(), logger, null);
        DcSctpOptions options = new DcSctpOptions();

        wrapper.socket = factory.create("testSocketCreate", callbacks, null, options);
        assertNotNull(wrapper.socket);
    }

    @Test
    public void testExceptionThroughJNI() {
        DcSctpSocketFactory factory = new DcSctpSocketFactory();
        SocketWrapper wrapper = new SocketWrapper();
        Logger logger = new LoggerImpl("testException");
        TestCallbacks callbacks = new TestCallbacks(wrapper, Clock.systemUTC(), logger, null) {
            @Override
            public Timeout createTimeout(DelayPrecision precision) {
                throw new IllegalStateException("I'm not happy!");
            }
        };
        DcSctpOptions options = new DcSctpOptions();

        IllegalStateException e = assertThrows(IllegalStateException.class,
                () -> wrapper.socket = factory.create("textException", callbacks, null, options),
                "Expected exception to be successfully thrown through JNI"
                );
        assertEquals("I'm not happy!", e.getMessage());

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

        TestCallbacks clientCallbacks =
                new TestCallbacks(client, Clock.systemUTC(), clientLogger, server);
        TestCallbacks serverCallbacks =
                new TestCallbacks(client, Clock.systemUTC(), serverLogger, client);
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
        client.socket.shutdown();
        try {
            Thread.sleep(100);
        } catch (InterruptedException ignored) {
        }
    }

    public static void main(String[] args) {
        DcSctp4jTest test = new DcSctp4jTest();
        System.out.println("Running testLoad");
        test.testLoad();
        System.out.println("Running testSocketCreate");
        test.testSocketCreate();
        System.out.println("Running testSimpleConnection");
        test.testSimpleConnection();
        DcSctp4jTest.executor.shutdown();
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

    private final SocketWrapper dest;

    public TestCallbacks(SocketWrapper wrapper, Clock clock, Logger logger, SocketWrapper dest)
    {
        this.wrapper = wrapper;
        this.clock = clock;
        this.logger = logger;
        this.dest = dest;
    }

    public static ByteBuffer cloneByteBuffer(final ByteBuffer original) {
        // Create clone with same capacity as original.
        final ByteBuffer clone = (original.isDirect()) ?
                ByteBuffer.allocateDirect(original.capacity()) :
                ByteBuffer.allocate(original.capacity());

        clone.put(original);

        return clone;
    }

    @Override
    public SendPacketStatus sendPacketWithStatus(byte[] data) {
        logger.info("Sending " + data.length + " byte packet");
        if (dest == null) {
            return SendPacketStatus.kError;
        }
        if (dest.socket == null) {
            return SendPacketStatus.kTemporaryFailure;
        }
        try {
            DcSctp4jTest.executor.submit(() -> dest.socket.receivePacket(data));
        }
        catch (Exception e) {
            logger.error("Error submitting packet", e);
            return SendPacketStatus.kError;
        }
        return SendPacketStatus.kSuccess;

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
        String s = new String(message.getPayload(), StandardCharsets.UTF_8);

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
