SUBDIRS = x86

noinst_LIBRARIES       = libonejit.a
# pkglib_LTLIBRARIES   =

AM_CPPFLAGS            = -I$(top_srcdir)/include
# libonejit_a_CPPFLAGS = -I$(top_srcdir)/include

libonejit_a_SOURCES    = \
        archid.cpp assembler.cpp binary.cpp bits.cpp call.cpp code.cpp codeparser.cpp \
        compiler.cpp imm.cpp const.cpp error.cpp expr.cpp func.cpp functype.cpp \
        group.cpp kind.cpp label.cpp mem.cpp name.cpp \
        node.cpp nodeheader.cpp op.cpp opstmt.cpp \
        stmt0.cpp stmt1.cpp stmt2.cpp stmt3.cpp stmt4.cpp stmtn.cpp \
        tuple.cpp type.cpp unary.cpp local.cpp var.cpp

EXTRA_libonejit_a_DEPENDENCIES = $(LIBONEJIT_X86)
# libonejit_a_LDFLAGS  =
libonejit_a_LIBADD     = \
        x86/addr.o x86/arg.o x86/asm_util.o x86/asm_x64.o x86/asm0_x64.o \
        x86/asm1_x64.o x86/asm2_x64.o x86/asm3_x64.o x86/rex_byte_x64.o
