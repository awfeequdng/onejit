#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([onejit], [0.0.0], [github.com/cosmos72/onejit])
AC_CONFIG_SRCDIR([src/archid.cpp])
AC_CONFIG_HEADERS([include/config.h])
AC_CONFIG_AUX_DIR([admin])
AM_INIT_AUTOMAKE([1.14 foreign subdir-objects])

# Checks for programs.
AC_PROG_CXX
AC_LANG(C++)

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([cstdint])

# Checks for typedefs, structures, and compiler characteristics.

################################################################################

# C++ flags '-g -O2' are already tested by AC_PROG_CXX
# C++ flags '-p -pg' are incompatible with -fomit-frame-pointer
initial_CXXFLAGS="$CXXFLAGS"
for try_flag in -std=c++11 -fomit-frame-pointer -pipe -Wall -Wshadow -Wundef \
                -Wcast-qual -Wcast-align -Wpointer-arith -Wsign-compare
do
  save_CXXFLAGS="$CXXFLAGS"
  CXXFLAGS="$CXXFLAGS $try_flag"
  try_flag_id="`echo \"$try_flag\" | tr =+- _x_`"
  AC_CACHE_CHECK(for C++ compiler flag $try_flag, ac_cv_cflag$try_flag_id, AC_TRY_COMPILE(
  [
    int ret0(void) {
      return 0;
    }
  ] , [
    int a = ret0();
  ], [
    eval "ac_cv_cflag$try_flag_id=yes"
  ], [
    eval "ac_cv_cflag$try_flag_id=no"
    CXXFLAGS="$save_CXXFLAGS"
  ]))
done
# custom_CXXFLAGS="$CXXFLAGS"
# CXXFLAGS="$initial_CXXFLAGS"


# Checks for library functions.

AC_CONFIG_FILES([Makefile src/Makefile])

AC_OUTPUT
