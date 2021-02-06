SUBDIRS = x64

noinst_LIBRARIES       = libonejit.a
# pkglib_LTLIBRARIES   =

AM_CPPFLAGS            = -I$(top_srcdir)/include
# libonejit_a_CXXFLAGS =

libonejit_a_SOURCES    = \
        archid.cpp assembler.cpp binary.cpp bits.cpp call.cpp code.cpp codeparser.cpp \
        compiler.cpp imm.cpp const.cpp error.cpp expr.cpp func.cpp functype.cpp \
        group.cpp kind.cpp label.cpp mem.cpp name.cpp \
        node.cpp nodeheader.cpp op.cpp opstmt.cpp \
        stmt0.cpp stmt1.cpp stmt2.cpp stmt3.cpp stmt4.cpp stmtn.cpp \
        tuple.cpp type.cpp unary.cpp local.cpp var.cpp

EXTRA_libonejit_a_DEPENDENCIES = $(LIBONEJIT_X64)
# libonejit_a_LDFLAGS  =
libonejit_a_LIBADD     = \
        x64/arg.o x64/asm_util.o x64/asm.o x64/asm0.o x64/asm1.o x64/asm2.o x64/asm3.o \
        x64/mem.o x64/rex_byte.o x64/scale.o
