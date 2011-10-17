
moduledir = $(prefix)/so

module_PROGRAMS = fault_disk.so
fault_disk_so_SOURCES = \
  fault_disk_file.cpp \
  fault_disk_system.cpp

INCLUDES = -I$(top_srcdir)/include

AM_CPPFLAGS = -fPIC
AM_LDFLAGS = -shared
