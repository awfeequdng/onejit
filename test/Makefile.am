SUBDIRS =

bin_PROGRAMS           = test_jit

AM_CPPFLAGS            = -I$(top_srcdir)/include
AM_CXXFLAGS            = $(CAPSTONE_CFLAGS)

test_jit_SOURCES       = test_disasm.cpp test_expr.cpp test_eval.cpp test_func.cpp test_make_func.cpp \
                         test_main.cpp test_mir.cpp test_optimize.cpp test_regallocator.cpp \
                         test_stl.cpp test_stmt.cpp test_x64.cpp
# test_jit_CXXFLAGS    =

EXTRA_test_jit_DEPENDENCIES = $(LIBONEJIT) $(LIBONESTL)
test_jit_LDADD              = $(LIBONEJIT) $(LIBONESTL) $(CAPSTONE_LIBS) $(MIR_LIBS)
# test_jit_LDFLAGS          = -export-dynamic $(LDFLAGS_BIN_EXPORT_DYNAMIC)
