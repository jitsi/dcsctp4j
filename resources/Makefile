# Minimal build steps that produce libdcsctp.a
# Copyright (C) 2025
# Author: Thomas Fitzsimmons <fitzsim@fitzsim.org>
# Version: 1
# SPDX-License-Identifier: Apache-2.0
# "gn" has no support for ppc64le.  This Makefile can be used in place of "gn"
# and "ninja" to build libdcsctp.a on non-"gn" architectures.  The Makefile was
# written from scratch starting from the assumption that all non-test net/dcsctp
# .cc files were needed by libdcsctp4j.so.  Runtime testing of Jitsi confirms
# that the resulting libdcsctp.a satisfies libdcsctp4j.so's needs on ppc64le.
S := $(VPATH)
DEBUG ?= -O2
# Flags extracted from "ninja -v" output for the x64 target, built with the clang
# that the WebRTC checkout provides (which this Makefile is also used with).
# Omitted: the target and sysroot (passed in through CXX), -msse3 and other
# x64-specific options, the Chromium clang plugin, and build-specific defines
# such as CR_CLANG_REVISION and CR_SYSROOT_KEY.
CXXFLAGS += -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D_FORTIFY_SOURCE=3
CXXFLAGS += -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE
CXXFLAGS += -D_GNU_SOURCE -D_LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_NONE
CXXFLAGS += -D_GLIBCXX_ASSERTIONS=1
CXXFLAGS += -DUSE_UDEV -DUSE_AURA=1 -DUSE_GLIB=1 -DUSE_OZONE=1
CXXFLAGS += -DNDEBUG -DNVALGRIND
CXXFLAGS += -DDYNAMIC_ANNOTATIONS_ENABLED=0 -DWEBRTC_ENABLE_PROTOBUF=1
CXXFLAGS += -DWEBRTC_STRICT_FIELD_TRIALS=0
CXXFLAGS += -DWEBRTC_INCLUDE_INTERNAL_AUDIO_DEVICE -DRTC_ENABLE_VP9
CXXFLAGS += -DRTC_DAV1D_IN_INTERNAL_DECODER_FACTORY -DWEBRTC_HAVE_SCTP
CXXFLAGS += -DWEBRTC_ENCODER_PSNR_STATS -DWEBRTC_LIBRARY_IMPL
CXXFLAGS += -DPROTOBUF_ENABLE_DEBUG_LOGGING_MAY_LEAK_PII=0
CXXFLAGS += -DWEBRTC_DEPRECATE_PLAN_B -DWEBRTC_NON_STATIC_TRACE_EVENT_HANDLERS=1
CXXFLAGS += -DWEBRTC_POSIX -DWEBRTC_LINUX -DABSL_ALLOCATOR_NOTHROW=1
CXXFLAGS += -Wall -Wextra -Wimplicit-fallthrough -Wextra-semi
CXXFLAGS += -Wunreachable-code-aggressive -Wgnu -Wno-gnu-anonymous-struct
CXXFLAGS += -Wno-gnu-conditional-omitted-operand -Wno-gnu-include-next
CXXFLAGS += -Wno-gnu-label-as-value -Wno-gnu-redeclared-enum
CXXFLAGS += -Wno-gnu-statement-expression -Wno-gnu-zero-variadic-macro-arguments
CXXFLAGS += -Wno-zero-length-array -Wthread-safety -Wno-unused-template
CXXFLAGS += -Wno-missing-field-initializers -Wno-unused-parameter -Wno-psabi
CXXFLAGS += -Wno-stringop-overread -Wloop-analysis
CXXFLAGS += -Wno-unneeded-internal-declaration -Wno-cast-function-type
CXXFLAGS += -Wno-thread-safety-reference-return -Wno-nontrivial-memcall
CXXFLAGS += -Wno-uninitialized-const-pointer -Wno-unused-but-set-global
CXXFLAGS += -Wshadow -Werror -Werror=\#pragma-messages
CXXFLAGS += -fno-delete-null-pointer-checks -fno-strict-overflow -fno-ident
CXXFLAGS += -fno-math-errno -fno-strict-aliasing -fstack-protector
CXXFLAGS += -funwind-tables -fPIC -pthread -fdiagnostics-show-inlining-chain
CXXFLAGS += -fmerge-all-constants -fno-lifetime-dse
CXXFLAGS += -mllvm -instcombine-lower-dbg-declare=0
CXXFLAGS += -mllvm -split-threshold-for-reg-with-hint=0
CXXFLAGS += -ffp-contract=off -Wno-builtin-macro-redefined
CXXFLAGS += -D__DATE__= -D__TIME__= -D__TIMESTAMP__= -no-canonical-prefixes
CXXFLAGS += -ftrivial-auto-var-init=pattern $(DEBUG) -fdata-sections
CXXFLAGS += -ffunction-sections -fno-unique-section-names -fno-omit-frame-pointer
CXXFLAGS += -gdwarf-4 -g2 -gdwarf-aranges -gno-pubnames -fuse-ctor-homing
CXXFLAGS += -fsanitize=array-bounds -fsanitize-trap=array-bounds
CXXFLAGS += -fsanitize-ignore-for-ubsan-feature=array-bounds
CXXFLAGS += -Wno-unsafe-buffer-usage-in-static-sized-array
CXXFLAGS += -fsanitize=return -fsanitize-trap=return
CXXFLAGS += -fsanitize-ignore-for-ubsan-feature=return
CXXFLAGS += -fsanitize=unreachable -fsanitize-trap=unreachable
CXXFLAGS += -fsanitize-ignore-for-ubsan-feature=unreachable
CXXFLAGS += -fvisibility=hidden -Wheader-hygiene -Wstring-conversion
CXXFLAGS += -Wtautological-overlap-compare -Wexit-time-destructors
CXXFLAGS += -Wglobal-constructors -Wctad-maybe-unsupported -Wc++11-narrowing
CXXFLAGS += -Wundef -Wunused-lambda-capture -Wno-invalid-offsetof
CXXFLAGS += -Wenum-compare-conditional -Wno-nullability-completeness
CXXFLAGS += -std=c++20 -Wno-trigraphs -gsimple-template-names
CXXFLAGS += -fno-exceptions -fno-rtti -fvisibility-inlines-hidden
CXXFLAGS += -Wnon-virtual-dtor -Woverloaded-virtual
# gn adds these for arm64 targets; derive them from the compiler target here.
ifneq ($(shell $(CXX) -dM -E - </dev/null | grep -c __aarch64__),0)
CXXFLAGS += -DWEBRTC_ARCH_ARM64 -DWEBRTC_HAS_NEON -D__ARM_NEON__=1
endif
# Add include paths to fix compilation errors due to missing headers.
CXXFLAGS += -I$(S)
CXXFLAGS += -I$(S)/third_party/abseil-cpp
CXXFLAGS += -I$(S)/third_party/crc32c/config
CXXFLAGS += -I$(S)/third_party/crc32c/src/include
# Assume all non-test .cc files under net/dcsctp are needed by libdcsctp4j.so.
# "gn/ninja" includes 37 extra objects but they are not needed by
# libdcsctp4j.so.
TESTS = $(shell find $(S)/net/dcsctp -name "*_test*.cc")
SOURCES = $(shell find $(S)/net/dcsctp -name "*.cc")
OBJECTS = $(patsubst %.cc,%.o,$(filter-out $(TESTS),$(SOURCES)))
# Assume all source files are in subdirectories and subsubdirectories of
# webrtc/src.
DIRS = $(shell find $(S) -maxdepth 6 -type d -print)
# Add other objects required to fix libdcsctp4j.so errors; first compilation
# errors, then undefined symbol errors reported at runtime by the dynamic
# linker.
OBJECTS += $(S)/api/task_queue/task_queue_base.o
OBJECTS += $(S)/api/units/time_delta.o
OBJECTS += $(S)/rtc_base/checks.o
OBJECTS += $(S)/rtc_base/logging.o
OBJECTS += $(S)/rtc_base/platform_thread_types.o
OBJECTS += $(S)/rtc_base/string_encode.o
OBJECTS += $(S)/rtc_base/string_utils.o
OBJECTS += $(S)/rtc_base/strings/string_builder.o
OBJECTS += $(S)/rtc_base/strings/string_format.o
OBJECTS += $(S)/rtc_base/system_time.o
OBJECTS += $(S)/rtc_base/time_utils.o
OBJECTS += $(S)/third_party/abseil-cpp/absl/base/internal/raw_logging.o
# absl/crc is needed by net/dcsctp/packet/crc32c.cc and by absl/strings/cord.cc.
ABSL_CRC_TESTS = $(shell find $(S)/third_party/abseil-cpp/absl/crc -name "*_test*.cc" -o -name "*_benchmark*.cc")
ABSL_CRC_SOURCES = $(shell find $(S)/third_party/abseil-cpp/absl/crc -name "*.cc")
OBJECTS += $(patsubst %.cc,%.o,$(filter-out $(ABSL_CRC_TESTS),$(ABSL_CRC_SOURCES)))
# absl/strings is complicated, so assume all non-test and non-benchmark .cc files
# are needed.
ABSL_STRINGS_TESTS = $(shell find $(S)/third_party/abseil-cpp/absl/strings -name "*_test*.cc" -o -name "*_benchmark*.cc")
ABSL_STRINGS_SOURCES = $(shell find $(S)/third_party/abseil-cpp/absl/strings -name "*.cc")
ABSL_STRINGS_OBJECTS = $(patsubst %.cc,%.o,$(filter-out $(ABSL_STRINGS_TESTS),$(ABSL_STRINGS_SOURCES)))
OBJECTS += $(ABSL_STRINGS_OBJECTS)
OBJECTS += $(S)/third_party/crc32c/src/src/crc32c.o
OBJECTS += $(S)/third_party/crc32c/src/src/crc32c_portable.o
# Keep built object paths separate from source code paths.
ifeq ($(strip $(OBJDIR)),)
$(error "Set OBJDIR to absolute path in which to store output objects")
endif
OBJECTS := $(addprefix $(OBJDIR)/,$(OBJECTS))
OBJDIRS := $(addprefix $(OBJDIR)/,$(DIRS))
ifneq ($(strip $(VERBOSE)),1)
SILENT := @
endif
$(OBJDIR)/%.o: %.cc
	@echo CXX $@
	$(SILENT)$(CXX) $(CXXFLAGS) -c $< -o $@
$(OBJDIR)/libdcsctp.a: $(OBJECTS)
	@echo AR $@
	$(SILENT)$(AR) $(ARFLAGS) $@ $^
$(OBJECTS): | $(OBJDIRS)
$(OBJDIRS):
	@mkdir -p $@
