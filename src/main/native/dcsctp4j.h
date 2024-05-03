#ifndef DCSCTP4J_H
#define DCSCTP4J_H

#include <smjni/smjni.h>
#include "all_classes.h"
#include "extra_classes.h"
#include "enum_classes.h"
#include <net/dcsctp/public/dcsctp_socket_factory.h>
#include "wrapped_objects.h"


using java_classes = smjni::java_class_table<JNIGEN_ALL_GENERATED_CLASSES, EXTRA_CLASSES, ENUM_CLASSES>;

#define NATIVE_PROLOG  try {
#define NATIVE_EPILOG  } \
                       catch(java_exception & ex) \
                       { \
                           ex.raise(env);\
                       }\
                       catch(std::exception & ex)\
                       {\
                           java_exception::translate(env, ex);\
                       }

/* Compilers will complain if functions returning a value don't return anything, even though the value returned doesn't
   matter to JNI if an exception has been raised. */
#define NATIVE_EPILOG_Z NATIVE_EPILOG return 0;

#endif
