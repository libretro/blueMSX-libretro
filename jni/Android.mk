LOCAL_PATH := $(call my-dir)

CORE_DIR := $(LOCAL_PATH)/..

HAVE_COMPAT := 1

include $(CORE_DIR)/Makefile.common

# The blueMSX codebase pervasively registers device callbacks whose first
# parameter is a concrete struct pointer against void*-based callback
# typedefs (~1000 sites).  Clang 16+ (NDK r26+) treats this as an error
# by default; keep it a warning here, as retyping every callback would
# be unjustifiable churn.  Genuine bugs it would hide (wrong arity or
# return type) are caught by the Linux CI's clang job.
COREFLAGS := -DANDROID $(COREDEFINES) $(INCFLAGS) -Wno-c++11-narrowing -Wno-error=incompatible-function-pointer-types

GIT_VERSION := " $(shell git rev-parse --short HEAD || echo unknown)"
ifneq ($(GIT_VERSION)," unknown")
	COREFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"
endif

include $(CLEAR_VARS)
LOCAL_MODULE    := retro
LOCAL_SRC_FILES := $(SOURCES_CXX) $(SOURCES_C)
LOCAL_CXXFLAGS  := $(COREFLAGS)
LOCAL_CFLAGS    := $(COREFLAGS)
LOCAL_LDFLAGS   := -Wl,-version-script=$(CORE_DIR)/link.T
LOCAL_LDLIBS    := -lz

# armv5 clang workarounds
ifeq ($(TARGET_ARCH_ABI),armeabi)
	LOCAL_ARM_MODE := arm
	LOCAL_LDLIBS   += -latomic
endif

include $(BUILD_SHARED_LIBRARY)
