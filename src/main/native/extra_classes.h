#ifndef EXTRA_CLASSES_H
#define EXTRA_CLASSES_H

#include "type_mapping.h"
#include <net/dcsctp/public/dcsctp_socket.h>

/* Standard Java classes that couldn't be annotated with jnigen annotations. */

class Integer_class : public smjni::java_runtime::simple_java_class<jInteger>
{
public:
    Integer_class(JNIEnv * env);

    smjni::local_java_ref<jInteger> ctor(JNIEnv * env, jint value) const
        { return m_ctor(env, *this, value); }
    jint intValue(JNIEnv * env, const smjni::auto_java_ref<jInteger> & self) const
        { return m_intValue(env, self); }
private:
    const smjni::java_constructor<jInteger, jint> m_ctor;
    const smjni::java_method<jint, jInteger> m_intValue;

public:
    std::optional<int> toOptionalInt(JNIEnv * env, jInteger jint) const {
        if (jint == nullptr) {
            return std::optional<int>();
        }
        else {
            return std::optional<int>(intValue(env, jint));
        }
    }

    jInteger fromOptionalInt(JNIEnv * env, std::optional<int> optint) const {
        if (!optint) {
            return nullptr;
        }
        else {
            return ctor(env, *optint).release();
        }
    }
};


inline Integer_class::Integer_class(JNIEnv * env):
    simple_java_class(env),
    m_ctor(env, *this),
    m_intValue(env, *this, "intValue")
{}


class Long_class : public smjni::java_runtime::simple_java_class<jLong>
{
public:
    Long_class(JNIEnv * env);

    smjni::local_java_ref<jLong> ctor(JNIEnv * env, jlong value) const
        { return m_ctor(env, *this, value); }
    jlong longValue(JNIEnv * env, const smjni::auto_java_ref<jLong> & self) const
        { return m_longValue(env, self); }
private:
    const smjni::java_constructor<jLong, jlong> m_ctor;
    const smjni::java_method<jlong, jLong> m_longValue;
};


inline Long_class::Long_class(JNIEnv * env):
    simple_java_class(env),
    m_ctor(env, *this),
    m_longValue(env, *this, "longValue")
{}


class Instant_class : public smjni::java_runtime::simple_java_class<jInstant>
{
public:
    Instant_class(JNIEnv * env);

    jlong getEpochSecond(JNIEnv * env, const smjni::auto_java_ref<jInstant> & self) const
        { return m_getEpochSecond(env, self); }
    jint getNano(JNIEnv * env, const smjni::auto_java_ref<jInstant> & self) const
        { return m_getNano(env, self); }
private:
    const smjni::java_method<jlong, jInstant> m_getEpochSecond;
    const smjni::java_method<jint, jInstant> m_getNano;

public:
    webrtc::Timestamp toTimestamp(JNIEnv * env, const smjni::auto_java_ref<jInstant> & self) const {
        return webrtc::Timestamp::Micros(getEpochSecond(env, self) * 1'000'000 +
                                         getNano(env, self) / 1000);
    }
};

inline Instant_class::Instant_class(JNIEnv * env):
    simple_java_class(env),
    m_getEpochSecond(env, *this, "getEpochSecond"),
    m_getNano(env, *this, "getNano")
{}

#define EXTRA_CLASSES Integer_class, Long_class, Instant_class

#endif
