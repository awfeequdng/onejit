SUBDIRS =

noinst_LIBRARIES       = libonejit.a
# pkglib_LTLIBRARIES   =

AM_CPPFLAGS            = -I$(top_srcdir)/include
# libonejit_a_CXXFLAGS =

libonejit_a_SOURCES    = \
        archid.cpp assembler.cpp bits.cpp code.cpp codeparser.cpp \
        compiler.cpp imm.cpp error.cpp eval.cpp func.cpp group.cpp kind.cpp  \
        op.cpp opstmt.cpp optimizer.cpp optimizer_binary.cpp optimizer_tuple.cpp \
        space.cpp type.cpp local.cpp value.cpp value_fmt.cpp \
        \
        node/binary.cpp node/call.cpp node/childrange.cpp node/comma.cpp node/const.cpp \
        node/expr.cpp node/functype.cpp node/label.cpp node/mem.cpp node/name.cpp \
        node/node.cpp node/nodeheader.cpp \
        node/stmt0.cpp node/stmt1.cpp node/stmt2.cpp node/stmt3.cpp node/stmt4.cpp node/stmtn.cpp \
        node/tuple.cpp node/unary.cpp node/var.cpp \
        \
        x64/arg.cpp x64/asm0.cpp x64/asm1.cpp x64/asm2.cpp x64/asm3.cpp x64/asmn.cpp \
        x64/assembler.cpp x64/compiler.cpp x64/mem.cpp x64/rex_byte.cpp x64/scale.cpp x64/util.cpp

EXTRA_libonejit_a_DEPENDENCIES =
# libonejit_a_LDFLAGS  =
libonejit_a_LIBADD     =
