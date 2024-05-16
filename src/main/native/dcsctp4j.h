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

#ifndef DCSCTP4J_H
#define DCSCTP4J_H

#include <smjni/smjni.h>
#include "all_classes.h"
#include "extra_classes.h"
#include "enum_members.h"
#include <net/dcsctp/public/dcsctp_socket_factory.h>
#include "wrapped_objects.h"

using java_classes = smjni::java_class_table<JNIGEN_ALL_GENERATED_CLASSES, EXTRA_CLASSES>;

#define NATIVE_PROLOG  try {
#define NATIVE_EPILOG  } \
                       catch(smjni::java_exception & ex) \
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
