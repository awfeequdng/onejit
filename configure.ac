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
AC_LANG(C++)
AC_PROG_RANLIB
AM_PROG_AR

# Checks for libraries.

# capstone is only used by tests, continue even if not found
PKG_CHECK_MODULES([CAPSTONE], [capstone >= 4.0.0],
                  AC_DEFINE([HAVE_LIBCAPSTONE], 1, [Define to 1 if you have the capstone library.]),
                  [true])

# Checks for library functions.

# Checks for header files.
AC_CHECK_HEADERS([cerrno cstddef cstdint cstdio cstring])

# Checks for typedefs, structures, and compiler characteristics.

################################################################################

# C++ flags '-g -O2' are already tested by AC_PROG_CXX
# C++ flags '-p -pg' are incompatible with -fomit-frame-pointer
initial_CXXFLAGS="$CXXFLAGS"
for try_flag in -Os -std=c++11 -fomit-frame-pointer -pipe -Wall -W -Wextra -Wshadow \
                -Wundef -Wcast-qual -Wcast-align -Wpointer-arith -Wsign-compare
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



AC_SUBST([CAPSTONE_CFLAGS])
AC_SUBST([CAPSTONE_LIBS])

AC_SUBST(LIBONEJIT_X64, '$(top_builddir)/onejit/x86/libonejit_x86.a')
AC_SUBST(LIBONEJIT,     '$(top_builddir)/onejit/libonejit.a')
AC_SUBST(LIBONESTL,     '$(top_builddir)/onestl/libonestl.a')

AC_CONFIG_FILES([Makefile onejit/Makefile onejit/x86/Makefile onestl/Makefile test/Makefile])

AC_OUTPUT
