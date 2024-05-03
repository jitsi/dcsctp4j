# Hacked makefile, good enough for dev, not portable.

all: libdcsctp4j.dylib

SimpleJNI_INC=${HOME}/Git/dcsctp4j/SimpleJNI/inc
SimpleJNI_OBJ=${HOME}/Git/dcsctp4j/SimpleJNI
JNIGen_INC=${HOME}/Git/dcsctp4j/target/native/jnigen
WebRTC_INC=${HOME}/Chromium/WebRTC/src
Abseil_INC=${HOME}/Chromium/WebRTC/src/third_party/abseil-cpp
WebRTC_OBJ=${HOME}/Chromium/WebRTC/src/out/Default/obj

CXXFLAGS=-std=gnu++17 -g -O2 -Wall -fPIC -I${WebRTC_INC} -I${Abseil_INC} -I${SimpleJNI_INC} -I${JNIGen_INC} -I${JAVA_HOME}/include -I${JAVA_HOME}/include/darwin

LDFLAGS=-L${WebRTC_OBJ} -L${SimpleJNI_OBJ}

LIBS=-lsmjni -ldcsctp

libdcsctp4j.dylib: dcsctp4j.o wrapped_objects.o mapped_objects.o native_sctp_socket.o enum_classes.o
	$(CXX) -shared -o $@ $+ ${LDFLAGS} ${LIBS}

.PHONY: clean

clean:
	rm *.o
