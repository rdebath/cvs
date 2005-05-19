# glob.m4 serial 1
dnl Copyright (C) 2005 Free Software Foundation, Inc.
dnl This file is free software; the Free Software Foundation
dnl gives unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.

# The glob module assumes you want GNU glob, with glob_pattern_p etc,
# rather than vanilla POSIX glob.  This means your your code should
# always include <glob.h> for the glob prototypes.

AC_DEFUN([gl_GLOB_SUBSTITUTE],
[
  gl_PREREQ_GLOB

  GLOB_H=glob.h
  AC_LIBOBJ([glob])
  AC_DEFINE([GLOB_PREFIX], [[rpl_]],
    [Define to rpl_ if the glob replacement functions and variables
     should be used.])
  AC_SUBST([GLOB_H])
])

AC_DEFUN([gl_GLOB],
[ GLOB_H=
  AC_CHECK_HEADERS([glob.h], [], [GLOB_H=glob.h])

  if test -z "$GLOB_H"; then
    AC_COMPILE_IFELSE(
[[#include <gnu-versions.h>
char a[_GNU_GLOB_INTERFACE_VERSION == 1 ? 1 : -1];]],
      [], [GLOB_H=glob.h])
  fi

  if test -n "$GLOB_H"; then
    gl_GLOB_SUBSTITUTE
  fi
])

# Prerequisites of lib/glob.*.
AC_DEFUN([gl_PREREQ_GLOB],
[ AC_REQUIRE([gl_CHECK_TYPE_STRUCT_DIRENT_D_TYPE])dnl
  AC_REQUIRE([AC_HEADER_DIRENT])dnl
  AC_CHECK_HEADERS_ONCE([sys/cdefs.h unistd.h])dnl
  AC_CHECK_FUNCS_ONCE([getlogin_r __posix_getpwnam_r getpwnam_r stat64])dnl
  AC_CHECK_TYPES([struct dirent64])dnl
  :])
