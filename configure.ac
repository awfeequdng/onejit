#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([onejit],[0.0.0],[github.com/cosmos72/onejit])
AC_CONFIG_SRCDIR([onejit/archid.cpp])
AC_CONFIG_HEADERS([include/onejit_config.h])
AC_CONFIG_AUX_DIR([admin])
AM_INIT_AUTOMAKE([1.14 foreign subdir-objects])

# Checks for programs.
AC_PROG_CXX
AC_PROG_RANLIB
AM_PROG_AR

# Checks for libraries.

# capstone is only used by tests, continue even if not found
PKG_CHECK_MODULES([CAPSTONE], [capstone >= 4.0.0],
                  AC_DEFINE([HAVE_LIBCAPSTONE], 1, [Define to 1 if you have the capstone library.]),
                  [true])

# Check for MIR library. Do not prepend -lmir to LIBS
AC_DEFINE([HAVE_LIBMIR], [], [Define to 1 if you have the `mir' library (-lmir)])
AC_CHECK_LIB([mir], [MIR_new_module], [AC_DEFINE([HAVE_LIBMIR])])

# Check for pthread library, needed by MIR. Do not prepend -lpthread to LIBS
AC_DEFINE([HAVE_LIBPTHREAD], [], [Define to 1 if you have the `pthread' library (-lpthread)])
AC_CHECK_LIB([pthread], [pthread_create], [AC_DEFINE([HAVE_LIBPTHREAD])])

# Checks for library functions.
AC_CHECK_FUNCS([ffs ffsl])

AC_LANG(C++)

# Checks for header files.
AC_CHECK_HEADERS([cerrno cstddef cstdint cstdio cstring strings.h mir.h mir-gen.h mir/mir.h mir/mir-gen.h])

# Checks for typedefs, structures, and compiler characteristics.

################################################################################

# C++ flags '-g -O2' are already tested by AC_PROG_CXX
# C++ flags '-p -pg' are incompatible with -fomit-frame-pointer
initial_CXXFLAGS="$CXXFLAGS"
for try_flag in -Os -std=c++11 -fomit-frame-pointer -pipe -Wall -W -Wextra -Wshadow -Wundef \
                -Wcast-qual -Wcast-align -Wpointer-arith -Wsign-compare -Wno-type-limits
do
  save_CXXFLAGS="$CXXFLAGS"
  CXXFLAGS="$CXXFLAGS $try_flag"
  try_flag_id="`echo \"$try_flag\" | tr =+- _x_`"
  AC_CACHE_CHECK(for C++ compiler flag $try_flag, ac_cv_cflag$try_flag_id,
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
      int ret0(void) {
        return 0;
      }
     ]], [[
      int a = ret0();
    ]])],[
      eval "ac_cv_cflag$try_flag_id=yes"
    ],[
      eval "ac_cv_cflag$try_flag_id=no"
      CXXFLAGS="$save_CXXFLAGS"
    ])
  )
done
# CXXFLAGS="$initial_CXXFLAGS"

HAVE_MIR=0
if test "$ac_cv_lib_mir_MIR_new_module" = "yes"; then
  if test "$ac_cv_header_mir_h" = "yes" -o "$ac_cv_header_mir_mir_h" = "yes"; then
    if test "$ac_cv_header_mir_gen_h" = "yes" -o "$ac_cv_header_mir_mir_gen_h" = "yes"; then
      HAVE_MIR=1
    fi
  fi
fi
if test "$HAVE_MIR" != 0; then
  AC_DEFINE([HAVE_MIR], 1, [Define to 1 if you have the MIR header files and library.])
  if test "$HAVE_PTHREAD" != 0; then
    AC_SUBST([MIR_LIBS], ['-lmir -lpthread'])
  else
    AC_SUBST([MIR_LIBS], ['-lmir'])
  fi
  AC_MSG_NOTICE([Compiling with MIR jit native code generator.])
else
  AC_MSG_WARN([Compiling without MIR jit native code generator. Consider installing MIR from https://github.com/vnmakarov/mir])
fi

AC_SUBST([CAPSTONE_CFLAGS])
AC_SUBST([CAPSTONE_LIBS])

AC_SUBST(LIBONEJIT,     '$(top_builddir)/onejit/libonejit.a')
AC_SUBST(LIBONESTL,     '$(top_builddir)/onestl/libonestl.a')

AC_CONFIG_FILES([Makefile onejit/Makefile onestl/Makefile test/Makefile])

AC_OUTPUT
