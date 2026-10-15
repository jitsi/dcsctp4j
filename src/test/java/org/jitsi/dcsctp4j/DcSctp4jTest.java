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
import org.jitsi.utils.logging2.Logger;
import org.jitsi.utils.logging2.LoggerImpl;
import org.junit.jupiter.api.*;

import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.time.Clock;
import java.time.Instant;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.*;
import java.util.function.BooleanSupplier;
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
        TestCallbacks callbacks = new TestCallbacks(wrapper, Clock.systemUTC(), logger, null, false);
        DcSctpOptions options = new DcSctpOptions();

        wrapper.socket = factory.create("testSocketCreate", callbacks, null, options);
        assertNotNull(wrapper.socket);
        assertFalse(callbacks.failed);
    }

    @Test @Disabled /* dcsctp is compiled with -fno-exceptions, so we can't throw through it. */
    public void testExceptionThroughJNI() {
        DcSctpSocketFactory factory = new DcSctpSocketFactory();
        SocketWrapper wrapper = new SocketWrapper();
        Logger logger = new LoggerImpl("testException");
        TestCallbacks callbacks = new TestCallbacks(wrapper, Clock.systemUTC(), logger, null, false) {
            @Override
            public Timeout createTimeout(@NotNull DelayPrecision precision) {
                throw new IllegalStateException("I'm not happy!");
            }
        };
        DcSctpOptions options = new DcSctpOptions();

        IllegalStateException e = assertThrows(IllegalStateException.class,
                () -> wrapper.socket = factory.create("textException", callbacks, null, options),
                "Expected exception to be successfully thrown through JNI"
                );
        assertEquals("I'm not happy!", e.getMessage());
        assertFalse(callbacks.failed);
    }

    /** A connected client/server socket pair, with the callbacks each side was created with. */
    private static class ConnectedPair {
        final SocketWrapper client = new SocketWrapper();
        final SocketWrapper server = new SocketWrapper();
        TestCallbacks clientCallbacks;
        TestCallbacks serverCallbacks;
    }

    /**
     * Creates a client and a server socket that send their packets to each other, connects them,
     * and waits for both sides to report the connection established.
     */
    private ConnectedPair connectPair(String name) {
        return connectPair(name, false);
    }

    private ConnectedPair connectPair(String name, boolean pullMode) {
        DcSctpSocketFactory factory = new DcSctpSocketFactory();
        ConnectedPair pair = new ConnectedPair();

        Logger clientLogger = new LoggerImpl(this.getClass().getName() + "." + name + "Client");
        clientLogger.addContext("name", name + "Client");
        Logger serverLogger = new LoggerImpl(this.getClass().getName() + "." + name + "Server");
        serverLogger.addContext("name", name + "Server");

        pair.clientCallbacks = new TestCallbacks(pair.client, Clock.systemUTC(), clientLogger, pair.server, pullMode);
        pair.serverCallbacks = new TestCallbacks(pair.server, Clock.systemUTC(), serverLogger, pair.client, pullMode);
        DcSctpOptions options = new DcSctpOptions();
        options.setEnableReceivePullMode(pullMode);

        pair.server.socket = factory.create(name + "Server", pair.serverCallbacks, null, options);
        pair.client.socket = factory.create(name + "Client", pair.clientCallbacks, null, options);

        pair.client.socket.connect();
        waitUntil(() -> pair.client.socket.state() == SocketState.kConnected
                && pair.server.socket.state() == SocketState.kConnected, 2000);
        assertEquals(SocketState.kConnected, pair.client.socket.state());
        assertEquals(SocketState.kConnected, pair.server.socket.state());
        return pair;
    }

    @Test
    public void testSimpleConnection() {
        testConnection(false);
    }

    @Test
    public void testPullModeConnection() {
        testConnection(true);
    }

    private void testConnection(boolean pullMode) {
        ConnectedPair pair = connectPair(pullMode ? "pullModeConnection" : "pushModeConnection", pullMode);
        SocketWrapper client = pair.client;
        SocketWrapper server = pair.server;

        DcSctpMessage message = new DcSctpMessage((short) 0, 0, "Hello".getBytes(StandardCharsets.UTF_8));
        SendOptions sendOptions = new SendOptions();
        client.socket.send(message, sendOptions);
        waitUntil(() -> !pair.serverCallbacks.messagesReceived.isEmpty(), 2000);

        assertEquals(1, pair.serverCallbacks.messagesReceived.size(),
                "server should have received the message via " + (pullMode ? "OnMessageReady" : "OnMessageReceived"));
        DcSctpMessage received = pair.serverCallbacks.messagesReceived.get(0);
        assertEquals(0, received.getStreamID());
        assertEquals(0, received.getPpid());
        assertArrayEquals("Hello".getBytes(StandardCharsets.UTF_8), received.getPayload());
        assertEquals(0, pair.clientCallbacks.messagesReceived.size(), "client should not receive anything");
        assertEquals(SocketState.kConnected, client.socket.state());
        assertEquals(SocketState.kConnected, server.socket.state());

        client.socket.shutdown();
        waitUntil(() -> client.socket.state() == SocketState.kClosed
                && server.socket.state() == SocketState.kClosed, 2000);

        assertEquals(SocketState.kClosed, client.socket.state());
        assertEquals(SocketState.kClosed, server.socket.state());

        assertFalse(pair.clientCallbacks.failed);
        assertFalse(pair.serverCallbacks.failed);
    }

    /** Invalid (offset, length) pairs for a 64-byte array. */
    private static final int[][] INVALID_BOUNDS = {
            {-16, 32},                      // negative offset
            {0, -1},                        // negative length
            {Integer.MAX_VALUE - 100, 200}, // offset + length overflows int
            {0x40000000, 0x40000000},       // offset + length overflows int to exactly Integer.MIN_VALUE
            {65, 0},                        // offset past end
            {1, 64},                        // classic overrun
    };

    @Test
    public void testReceivePacketRejectsInvalidBounds() {
        DcSctpSocketFactory factory = new DcSctpSocketFactory();
        SocketWrapper wrapper = new SocketWrapper();
        Logger logger = new LoggerImpl("testReceivePacketRejectsInvalidBounds");
        TestCallbacks callbacks = new TestCallbacks(wrapper, Clock.systemUTC(), logger, null, false);
        wrapper.socket = factory.create("testReceivePacketRejectsInvalidBounds", callbacks, null, new DcSctpOptions());

        byte[] data = new byte[64];
        for (int[] args : INVALID_BOUNDS) {
            assertThrows(IllegalArgumentException.class,
                    () -> wrapper.socket.receivePacket(data, args[0], args[1]),
                    "offset " + args[0] + " length " + args[1] + " should be rejected");
        }

        // Valid boundary cases must not throw (the packet is simply dropped as malformed).
        assertDoesNotThrow(() -> wrapper.socket.receivePacket(data, 0, 64));
        assertDoesNotThrow(() -> wrapper.socket.receivePacket(data, 64, 0));
        assertDoesNotThrow(() -> wrapper.socket.receivePacket(data, 32, 32));
    }

    /**
     * The Java check in front of the native method means the test above only covers the Java side.
     * Call the native method directly through reflection to verify the native check as well.
     */
    @Test
    public void testNativeReceivePacketRejectsInvalidBounds() throws Exception {
        DcSctpSocketFactory factory = new DcSctpSocketFactory();
        SocketWrapper wrapper = new SocketWrapper();
        Logger logger = new LoggerImpl("testNativeReceivePacketRejectsInvalidBounds");
        TestCallbacks callbacks = new TestCallbacks(wrapper, Clock.systemUTC(), logger, null, false);
        wrapper.socket = factory.create("testNativeReceivePacketRejectsInvalidBounds", callbacks, null, new DcSctpOptions());

        Class<?> clazz = wrapper.socket.getClass();
        Method receivePacket_ = clazz.getDeclaredMethod("receivePacket_", long.class, byte[].class, int.class, int.class);
        receivePacket_.setAccessible(true);
        Field ptrField = clazz.getDeclaredField("ptr");
        ptrField.setAccessible(true);
        long ptr = ptrField.getLong(wrapper.socket);

        byte[] data = new byte[64];
        for (int[] args : INVALID_BOUNDS) {
            InvocationTargetException e = assertThrows(InvocationTargetException.class,
                    () -> receivePacket_.invoke(wrapper.socket, ptr, data, args[0], args[1]),
                    "offset " + args[0] + " length " + args[1] + " should be rejected natively");
            // Make sure it was the bounds check that rejected it, not some unrelated failure.
            assertNotNull(e.getCause());
            assertEquals("Invalid values for array length", e.getCause().getMessage());
        }
        assertDoesNotThrow(() -> receivePacket_.invoke(wrapper.socket, ptr, data, 0, 64));
        assertDoesNotThrow(() -> receivePacket_.invoke(wrapper.socket, ptr, data, 64, 0));
    }

    @Test
    public void testIncomingStreamsReset() {
        ConnectedPair pair = connectPair("streamsReset");
        SocketWrapper client = pair.client;
        SocketWrapper server = pair.server;
        TestCallbacks clientCallbacks = pair.clientCallbacks;
        TestCallbacks serverCallbacks = pair.serverCallbacks;

        // Use stream 3 so that it exists on both sides before resetting it.
        DcSctpMessage message = new DcSctpMessage((short) 3, 0, "Hello".getBytes(StandardCharsets.UTF_8));
        client.socket.send(message, new SendOptions());
        // The reset is only performed, not deferred, once the message has been delivered and acked.
        waitUntil(() -> !serverCallbacks.messagesReceived.isEmpty() && client.socket.bufferedAmount((short) 3) == 0, 2000);
        assertEquals(1, serverCallbacks.messagesReceived.size(), "server should receive the message on stream 3");
        assertEquals(3, serverCallbacks.messagesReceived.get(0).getStreamID());
        sleep(50);

        ResetStreamsStatus status = client.socket.resetStreams(Collections.singletonList((short) 3));
        assertEquals(ResetStreamsStatus.kPerformed, status);
        waitUntil(() -> !clientCallbacks.streamsResetPerformed.isEmpty()
                && !serverCallbacks.incomingStreamsReset.isEmpty(), 2000);
        // Allow a little extra time so that any spurious additional callbacks would show up.
        sleep(100);

        // The requester is told its outgoing streams were reset...
        assertEquals(1, clientCallbacks.streamsResetPerformed.size(),
                "client should get exactly one OnStreamsResetPerformed");
        assertArrayEquals(new short[]{3}, clientCallbacks.streamsResetPerformed.get(0));
        assertEquals(0, clientCallbacks.incomingStreamsReset.size(),
                "client should not get OnIncomingStreamsReset");

        // ...and the peer is told its incoming streams were reset, via the *incoming* callback.
        assertEquals(1, serverCallbacks.incomingStreamsReset.size(),
                "server should get exactly one OnIncomingStreamsReset");
        assertArrayEquals(new short[]{3}, serverCallbacks.incomingStreamsReset.get(0));
        assertEquals(0, serverCallbacks.streamsResetPerformed.size(),
                "server should not get OnStreamsResetPerformed");
        assertEquals(0, serverCallbacks.streamsResetFailed.size());
        assertEquals(0, clientCallbacks.streamsResetFailed.size());

        client.socket.shutdown();
        sleep(100);
    }

    /** Polls {@code condition} until it holds or {@code timeoutMillis} elapses. */
    private static void waitUntil(BooleanSupplier condition, long timeoutMillis) {
        long deadline = System.currentTimeMillis() + timeoutMillis;
        while (!condition.getAsBoolean() && System.currentTimeMillis() < deadline) {
            sleep(10);
        }
    }

    @Test
    public void testConnectWithConnectionToken() {
        DcSctpSocketFactory factory = new DcSctpSocketFactory();
        ConnectedPair pair = new ConnectedPair();
        Logger clientLogger = new LoggerImpl(this.getClass().getName() + ".tokenClient");
        Logger serverLogger = new LoggerImpl(this.getClass().getName() + ".tokenServer");
        pair.clientCallbacks = new TestCallbacks(pair.client, Clock.systemUTC(), clientLogger, pair.server, false);
        pair.serverCallbacks = new TestCallbacks(pair.server, Clock.systemUTC(), serverLogger, pair.client, false);
        DcSctpOptions options = new DcSctpOptions();

        pair.server.socket = factory.create("tokenServer", pair.serverCallbacks, null, options);
        pair.client.socket = factory.create("tokenClient", pair.clientCallbacks, null, options);

        // Out-of-band connection: each side generates a token, the tokens are exchanged
        // out of band, and each side finishes the connection with both tokens.
        byte[] clientToken = factory.generateConnectionToken(options,
                (low, high) -> ThreadLocalRandom.current().nextLong(low, high));
        byte[] serverToken = factory.generateConnectionToken(options,
                (low, high) -> ThreadLocalRandom.current().nextLong(low, high));
        assertTrue(clientToken.length > 0);
        assertTrue(serverToken.length > 0);
        assertFalse(Arrays.equals(clientToken, serverToken), "tokens should carry different random tags");

        // Garbage is rejected without connecting.
        assertFalse(pair.client.socket.connectWithConnectionToken(new byte[]{1, 2, 3}, serverToken));
        assertEquals(SocketState.kClosed, pair.client.socket.state());

        assertTrue(pair.client.socket.connectWithConnectionToken(clientToken, serverToken));
        assertTrue(pair.server.socket.connectWithConnectionToken(serverToken, clientToken));
        assertEquals(SocketState.kConnected, pair.client.socket.state());
        assertEquals(SocketState.kConnected, pair.server.socket.state());

        // Connecting again on a connected socket is a programming error, not a token problem.
        assertThrows(IllegalStateException.class,
                () -> pair.client.socket.connectWithConnectionToken(clientToken, serverToken));

        // The association works: a message gets through and the sockets stay connected.
        DcSctpMessage message = new DcSctpMessage((short) 0, 0, "Hello".getBytes(StandardCharsets.UTF_8));
        pair.client.socket.send(message, new SendOptions());
        waitUntil(() -> !pair.serverCallbacks.messagesReceived.isEmpty(), 2000);
        assertEquals(1, pair.serverCallbacks.messagesReceived.size(), "server should receive the message");
        assertArrayEquals("Hello".getBytes(StandardCharsets.UTF_8),
                pair.serverCallbacks.messagesReceived.get(0).getPayload());
        assertEquals(SocketState.kConnected, pair.client.socket.state());
        assertEquals(SocketState.kConnected, pair.server.socket.state());

        pair.client.socket.shutdown();
        waitUntil(() -> pair.client.socket.state() == SocketState.kClosed
                && pair.server.socket.state() == SocketState.kClosed, 2000);
        assertEquals(SocketState.kClosed, pair.client.socket.state());
        assertEquals(SocketState.kClosed, pair.server.socket.state());
        assertFalse(pair.clientCallbacks.failed);
        assertFalse(pair.serverCallbacks.failed);
    }

    /**
     * The random-number callback given to generateConnectionToken runs inside dcsctp, which is
     * compiled without exception support, so its failures have to be carried around dcsctp and
     * surface to the caller as if the callback had been called directly.
     */
    @Test
    public void testGenerateConnectionTokenCallbackFailures() {
        DcSctpSocketFactory factory = new DcSctpSocketFactory();
        DcSctpOptions options = new DcSctpOptions();

        // A throwing callback propagates its own exception.
        IllegalStateException thrown = assertThrows(IllegalStateException.class,
                () -> factory.generateConnectionToken(options, (low, high) -> {
                    throw new IllegalStateException("no entropy today");
                }));
        assertEquals("no entropy today", thrown.getMessage());

        // An out-of-range result is rejected (one past the end of the half-open range).
        long[] range = new long[2];
        IllegalArgumentException outOfRange = assertThrows(IllegalArgumentException.class,
                () -> factory.generateConnectionToken(options, (low, high) -> {
                    range[0] = low;
                    range[1] = high;
                    return high;
                }));
        assertEquals("getRandomInt returned " + range[1] + ", outside [" + range[0] + ", " + range[1] + ")",
                outOfRange.getMessage());
        assertThrows(IllegalArgumentException.class,
                () -> factory.generateConnectionToken(options, (low, high) -> low - 1));

        // The factory is still usable afterwards.
        byte[] token = factory.generateConnectionToken(options,
                (low, high) -> ThreadLocalRandom.current().nextLong(low, high));
        assertTrue(token.length > 0);
    }

    private static void sleep(long millis) {
        try {
            Thread.sleep(millis);
        } catch (InterruptedException ignored) {
        }
    }

    public static void main(String[] args) throws Exception {
        DcSctp4jTest test = new DcSctp4jTest();
        System.out.println("Running testLoad");
        test.testLoad();
        System.out.println("Running testSocketCreate");
        test.testSocketCreate();
        System.out.println("Running testSimpleConnection");
        test.testSimpleConnection();
        System.out.println("Running testPullModeConnection");
        test.testPullModeConnection();
        System.out.println("Running testReceivePacketRejectsInvalidBounds");
        test.testReceivePacketRejectsInvalidBounds();
        System.out.println("Running testNativeReceivePacketRejectsInvalidBounds");
        test.testNativeReceivePacketRejectsInvalidBounds();
        System.out.println("Running testIncomingStreamsReset");
        test.testIncomingStreamsReset();
        System.out.println("Running testConnectWithConnectionToken");
        test.testConnectWithConnectionToken();
        System.out.println("Running testGenerateConnectionTokenCallbackFailures");
        test.testGenerateConnectionTokenCallbackFailures();
        /* System.out.println("Testing testExceptionThroughJNI");
        test.testExceptionThroughJNI(); */
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
    private final boolean pullMode;

    private final SocketWrapper dest;

    public volatile boolean failed = false;
    /** Messages received, whether delivered by OnMessageReceived or fetched after OnMessageReady. */
    public final List<DcSctpMessage> messagesReceived = Collections.synchronizedList(new ArrayList<>());

    public final List<short[]> streamsResetPerformed = Collections.synchronizedList(new ArrayList<>());
    public final List<short[]> incomingStreamsReset = Collections.synchronizedList(new ArrayList<>());
    public final List<short[]> streamsResetFailed = Collections.synchronizedList(new ArrayList<>());

    public TestCallbacks(SocketWrapper wrapper, Clock clock, Logger logger, SocketWrapper dest, boolean pullMode)
    {
        this.wrapper = wrapper;
        this.clock = clock;
        this.logger = logger;
        this.pullMode = pullMode;
        this.dest = dest;
    }

    @Override
    public SendPacketStatus sendPacketWithStatus(@NotNull byte[] data) {
        logger.info("Sending " + data.length + " byte packet");
        if (dest == null) {
            return SendPacketStatus.kError;
        }
        if (dest.socket == null) {
            return SendPacketStatus.kTemporaryFailure;
        }
        try {
            DcSctp4jTest.executor.submit(() ->
            {
                try {
                    dest.socket.receivePacket(data, 0, data.length);
                } catch (Exception e) {
                    failed = true;
                    logger.error("Error processing received packet", e);
                }
            });
        }
        catch (Exception e) {
            logger.error("Error submitting packet", e);
            failed = true;
            return SendPacketStatus.kError;
        }
        return SendPacketStatus.kSuccess;

    }

    @Override
    public Timeout createTimeout(@NotNull DelayPrecision precision) {
        return new TestTimeout(wrapper);
    }

    @NotNull
    @Override
    public Instant Now() {
        return clock.instant();
    }

    @Override
    public long getRandomInt(long low, long high) {
        return ThreadLocalRandom.current().nextLong(low, high);
    }

    @Override
    public void OnMessageReceived(@NotNull DcSctpMessage message) {
        try {
            assertFalse(pullMode, "OnMessageReceived must not be called in pull mode");
            messagesReceived.add(message);
            String s = new String(message.getPayload(), StandardCharsets.UTF_8);

            logger.info("Message received: " +
                    "streamID " + message.getStreamID() +
                    ", PPID " + message.getPpid() +
                    ", payload " + s);
        } catch (Throwable e) {
            failed = true;
            logger.error("Error processing OnMessageReceived", e);
        }
    }

    @Override
    public void OnMessageReady() {
        try {
            assertTrue(pullMode);
            long messagesReady = wrapper.socket.messagesReady();
            assertTrue(messagesReady > 0);
            logger.info(messagesReady + " messages ready");

            // One OnMessageReady may announce several messages; drain the queue.
            DcSctpMessage message;
            while ((message = wrapper.socket.getNextMessage()) != null) {
                messagesReceived.add(message);
                String s = new String(message.getPayload(), StandardCharsets.UTF_8);

                logger.info("Message received with pull: " +
                        "streamID " + message.getStreamID() +
                        ", PPID " + message.getPpid() +
                        ", payload " + s);
            }
            assertEquals(0, wrapper.socket.messagesReady());
        } catch (Throwable e) {
            failed = true;
            logger.error("Error processing OnMessageReady", e);
        }
    }

    @Override
    public void OnError(@NotNull ErrorKind error, @NotNull String message) {
        logger.info("Error received: " +
                "errorKind " + error +
                ", message " + message);
    }

    @Override
    public void OnAborted(@NotNull ErrorKind error, @NotNull String message) {
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
    public void OnStreamsResetFailed(@NotNull short[] outgoing_streams, @NotNull String reason) {
        logger.info("Stream reset failed for outgoing streams " + join(outgoing_streams) +
                ": " + reason);
        streamsResetFailed.add(outgoing_streams);
    }

    @Override
    public void OnStreamsResetPerformed(@NotNull short[] outgoing_streams) {
        logger.info("Stream reset performed for outgoing streams " + join(outgoing_streams));
        streamsResetPerformed.add(outgoing_streams);
    }

    @Override
    public void OnIncomingStreamsReset(@NotNull short[] incoming_streams) {
        logger.info("Incoming streams reset for streams " + join(incoming_streams));
        incomingStreamsReset.add(incoming_streams);
    }
}
