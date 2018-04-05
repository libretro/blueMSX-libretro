LOCAL_PATH := $(call my-dir)

CORE_DIR := $(LOCAL_PATH)/..

HAVE_COMPAT := 1

include $(CORE_DIR)/Makefile.common

COREFLAGS := -DANDROID $(COREDEFINES) $(INCFLAGS)

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
