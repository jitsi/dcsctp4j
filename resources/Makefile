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
# Add GCC-compatible flags extracted from "ninja -v" output for x86-64 target.
CXXFLAGS += -DUSE_UDEV -DUSE_AURA=1 -DUSE_GLIB=1 -DUSE_OZONE=1
CXXFLAGS += -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D_FORTIFY_SOURCE=2
CXXFLAGS += -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE
CXXFLAGS += -D_GNU_SOURCE -D_LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_NONE
CXXFLAGS += -D_GLIBCXX_ASSERTIONS=1
CXXFLAGS += -DNDEBUG -DNVALGRIND
CXXFLAGS += -DDYNAMIC_ANNOTATIONS_ENABLED=0 -DWEBRTC_ENABLE_PROTOBUF=1
CXXFLAGS += -DWEBRTC_STRICT_FIELD_TRIALS=0
CXXFLAGS += -DWEBRTC_INCLUDE_INTERNAL_AUDIO_DEVICE -DRTC_ENABLE_VP9
CXXFLAGS += -DRTC_DAV1D_IN_INTERNAL_DECODER_FACTORY -DWEBRTC_HAVE_SCTP
CXXFLAGS += -DWEBRTC_ENABLE_LIBEVENT -DWEBRTC_LIBRARY_IMPL -DWEBRTC_ENABLE_AVX2
CXXFLAGS += -DWEBRTC_NON_STATIC_TRACE_EVENT_HANDLERS=1 -DWEBRTC_POSIX
CXXFLAGS += -DWEBRTC_LINUX -DABSL_ALLOCATOR_NOTHROW=1 -Wall -Wextra
CXXFLAGS += -Wimplicit-fallthrough -Wextra-semi -Wno-missing-field-initializers
CXXFLAGS += -Wno-unused-parameter -Wno-psabi -Wno-cast-function-type
CXXFLAGS += -Wno-invalid-offsetof -Wshadow -Werror
CXXFLAGS += -fno-delete-null-pointer-checks -fno-ident -fno-strict-aliasing
CXXFLAGS += -fstack-protector -funwind-tables -fPIC -pthread
CXXFLAGS += -fmerge-all-constants -ffp-contract=off -m64
CXXFLAGS += -Wno-builtin-macro-redefined -D__DATE__= -D__TIME__=
CXXFLAGS += -D__TIMESTAMP__= -no-canonical-prefixes
CXXFLAGS += -ftrivial-auto-var-init=pattern $(DEBUG) -fdata-sections
CXXFLAGS += -ffunction-sections -fno-math-errno -fno-omit-frame-pointer
CXXFLAGS += -gdwarf-4 -g2 -ggnu-pubnames -fvisibility=hidden -Wno-shadow
CXXFLAGS += -Wctad-maybe-unsupported -Wundef -std=c++17 -Wno-trigraphs
CXXFLAGS += -fno-exceptions -fno-rtti
CXXFLAGS += -fvisibility-inlines-hidden -Wnon-virtual-dtor -Woverloaded-virtual
# Silence GCC 12 warnings.
CXXFLAGS += -Wno-return-type -Wno-overloaded-virtual -Wno-comment -Wno-undef
CXXFLAGS += -Wno-redundant-move
# Add include paths to fix compilation errors due to missing headers.
CXXFLAGS += -I$(S)
CXXFLAGS += -I$(S)/third_party/abseil-cpp
CXXFLAGS += -I$(S)/third_party/crc32c/config
CXXFLAGS += -I$(S)/third_party/crc32c/src/include
# Assume all non-test .cc files under net/dcsctp are needed by libdcsctp4j.so.
# "gn/ninja" includes 37 extra objects but they are not needed by
# libdcsctp4j.so.
TESTS = $(wildcard $(S)/net/dcsctp/*/*_test*.cc $(S)/net/dcsctp/*/*/*_test*.cc)
SOURCES = $(wildcard $(S)/net/dcsctp/*/*.cc $(S)/net/dcsctp/*/*/*.cc)
OBJECTS = $(patsubst %.cc,%.o,$(filter-out $(TESTS),$(SOURCES)))
# Assume all source files are in subdirectories and subsubdirectories of
# net/dcsctp.
DIRS = $(wildcard $(S)/*/ $(S)/*/*/ $(S)/*/*/*/ $(S)/*/*/*/*/ $(S)/*/*/*/*/*/)
# Add other objects required to fix libdcsctp4j.so errors; first compilation
# errors, then undefined symbol errors reported at runtime by the dynamic
# linker.
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
OBJECTS += $(S)/third_party/abseil-cpp/absl/types/bad_variant_access.o
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
