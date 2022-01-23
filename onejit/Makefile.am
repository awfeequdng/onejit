SUBDIRS =

noinst_LIBRARIES       = libonejit.a
# pkglib_LTLIBRARIES   =

AM_CPPFLAGS            = -I$(top_srcdir)/include
# libonejit_a_CXXFLAGS =

libonejit_a_SOURCES    = \
        abi.cpp archid.cpp assembler.cpp bits.cpp code.cpp codeparser.cpp compiler.cpp \
        imm.cpp error.cpp eval.cpp flowgraph.cpp func.cpp funcheader.cpp \
        group.cpp id.cpp kind.cpp op.cpp opstmt.cpp \
        optimizer.cpp optimizer_binary.cpp optimizer_tuple.cpp \
        space.cpp type.cpp value.cpp value_fmt.cpp \
        \
        ir/binary.cpp ir/call.cpp ir/childrange.cpp ir/comma.cpp ir/const.cpp \
        ir/expr.cpp ir/functype.cpp ir/label.cpp ir/header.cpp ir/mem.cpp ir/name.cpp \
        ir/node.cpp \
        ir/stmt0.cpp ir/stmt1.cpp ir/stmt2.cpp ir/stmt3.cpp ir/stmt4.cpp ir/stmtn.cpp \
        ir/tuple.cpp ir/unary.cpp ir/util.cpp ir/var.cpp \
        \
        reg/allocator.cpp \
        \
        mir/address.cpp mir/assembler.cpp mir/compiler.cpp mir/mem.cpp mir/util.cpp \
        \
        x64/address.cpp x64/arg.cpp x64/asm0.cpp x64/asm1.cpp x64/asm2.cpp x64/asm3.cpp x64/asmn.cpp \
        x64/assembler.cpp x64/compiler.cpp x64/mem.cpp x64/rex_byte.cpp x64/scale.cpp x64/util.cpp

EXTRA_libonejit_a_DEPENDENCIES =
# libonejit_a_LDFLAGS  =
libonejit_a_LIBADD     =
