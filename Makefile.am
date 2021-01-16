SUBDIRS = onejit

pkgdata_DATA           = COPYING README.md

bin_PROGRAMS           = jit

AM_CPPFLAGS            = -I$(top_srcdir)/include
# onejit_CPPFLAGS      = -I$(top_srcdir)/include

jit_SOURCES            = main.cpp

EXTRA_jit_DEPENDENCIES = $(LIBONEJIT)
jit_LDADD              = $(LIBONEJIT)
# jit_LDFLAGS          = -export-dynamic $(LDFLAGS_BIN_EXPORT_DYNAMIC)
