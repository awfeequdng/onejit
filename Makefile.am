SUBDIRS = oneasm onejit onestl

pkgdata_DATA           = COPYING README.md

bin_PROGRAMS           = test_jit

AM_CPPFLAGS            = -I$(top_srcdir)/include
# onejit_CPPFLAGS      = -I$(top_srcdir)/include

test_jit_SOURCES       = test_main.cpp

EXTRA_test_jit_DEPENDENCIES = $(LIBONEASM) $(LIBONEJIT) $(LIBONESTL)
test_jit_LDADD              = $(LIBONEASM) $(LIBONEJIT) $(LIBONESTL)
# test_jit_LDFLAGS          = -export-dynamic $(LDFLAGS_BIN_EXPORT_DYNAMIC)
