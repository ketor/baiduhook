
BHOOK_SRC = \
  src/ELFManager.cpp \
  src/AsmHelper.cpp \
  src/BinaryFile.cpp \
  src/HookImpl.cpp \
  src/HookManager.cpp \
  src/FaultUnit.cpp \
  src/Message.cpp \
  src/Core.cpp \
  src/bhook.cpp

BHOOK_INCLUDE = \
  include/elfdef.h \
  include/ELFManager.h \
  include/AsmHelper.h \
  include/BinaryFile.h \
  include/HookImpl.h \
  include/HookManager.h \
  include/FaultUnit.h \
  include/Message.h \
  include/Core.h \
  include/bhook.h \
  include/common.h

BHOOK_BIN_IN = \
  scripts/bhook.in

BHOOK_BIN = \
  scripts/bhook

BHOOK_TEST = \
  test/disk/* \
  test/memory/* \
  test/net/* \
  test/program/* \
  test/system/* \
  test/unittest/*

EXTRA_DIST = $(BHOOK_INCLUDE)
EXTRA_DIST += $(BHOOK_BIN_IN)
EXTRA_DIST += $(BHOOK_TEST)
#DIST_SUBDIRS = src include preload script

INCLUDES = -I$(top_srcdir)/include

lib_LIBRARIES = libbhook.a
libbhook_a_SOURCES = $(BHOOK_SRC)
include_HEADERS = include/bhook.h

moduledir = $(prefix)/so
module_PROGRAMS = bhook_main.so
bhook_main_so_SOURCES = $(BHOOK_SRC)
bhook_main_so_CPPFLAGS = -fPIC
bhook_main_so_LDFLAGS = -shared -ldl

SUBDIRS = preload/disk preload/memory preload/net

bin_SCRIPTS = $(BHOOK_BIN)

