#
# Disable CCACHE
CCACHE_DISABLE=1
#LDFLAGS += -Wl,-Bstatic -static-libgcc
#LDFLAGS += -Wl,-Bstatic
#
CPSW_DIR=/afs/slac/g/lcls/package/cpsw/framework/R4.2.0/src

# may override CPSW_DIR / DEVICELIB_DIR from release.mak
# must set SRCDIR (is redefined by recursive make)
SRCDIR= .
-include $(SRCDIR)/release.mak

include $(CPSW_DIR)/defs.mak
INSTALL_DIR=../

#HEADERS = RamControl.hh TPGMini.hh TPG.hh AmcCarrier.hh
CXXFLAGS += -g -DFRAMEWORK_R3_4
HEADERS = tprPatternYaml.hh  tprTriggerYaml.hh
hpstpr_SRCS += tprPatternYaml.cc  tprTriggerYaml.cc

STATIC_LIBRARIES+=hpstpr


#tpr_stream_SRCS = tpr_stream.cc
#tpr_stream_LIBS = $(CPSW_LIBS)
#PROGRAMS    += tpr_stream

include $(CPSW_DIR)/rules.mak
