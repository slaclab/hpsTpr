#
# Disable CCACHE
CCACHE_DISABLE=1
LDFLAGS += -Wl,-Bstatic -static-libgcc
#LDFLAGS += -Wl,-Bstatic
#

ifeq ($(PACKAGE_TOP),)
ifneq ($(EPICS_PACKAGE_TOP),)
	PACKAGE_TOP=$(EPICS_PACKAGE_TOP)
else
	$(error PACKAGE_TOP or EPICS_PACKAGE_TOP must be provided in the environment or on the command line)
endif
endif

CPSW_DIR=$(PACKAGE_TOP)/cpsw/framework/R4.4.1/src

# may override CPSW_DIR / DEVICELIB_DIR from release.mak
# must set SRCDIR (is redefined by recursive make)
SRCDIR= .
-include $(SRCDIR)/release.mak

include $(CPSW_DIR)/defs.mak
INSTALL_DIR=../

#HEADERS = RamControl.hh TPGMini.hh TPG.hh AmcCarrier.hh
CXXFLAGS += -g -DFRAMEWORK_R3_4
HEADERS = tprPatternYaml.hh  tprTriggerYaml.hh  tprsh.hh
hpstpr_SRCS += tprPatternYaml.cc  tprTriggerYaml.cc

STATIC_LIBRARIES+=hpstpr


PromLoad_SRCS = McsRead.cpp EvrCardG2Prom.cpp PromLoad.cpp
PromLoad_LIBS = $(CPSW_LIBS)
PROGRAMS    += PromLoad


PromVerify_SRCS = McsRead.cpp EvrCardG2Prom.cpp PromVerify.cpp
PromVerify_LIBS = $(CPSW_LIBS)
PROGRAMS    += PromVerify

tprtest_SRCS = tprtest.cc tpr.cc
PROGRAMS    += tprtest

#tpr_stream_SRCS = tpr_stream.cc
#tpr_stream_LIBS = $(CPSW_LIBS)
#PROGRAMS    += tpr_stream

include $(CPSW_DIR)/rules.mak
