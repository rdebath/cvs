# getndelim2.m4 serial 3
dnl Copyright (C) 2003 Free Software Foundation, Inc.
dnl This file is free software, distributed under the terms of the GNU
dnl General Public License.  As a special exception to the GNU General
dnl Public License, this file may be distributed as part of a program
dnl that contains a configuration script generated by Autoconf, under
dnl the same distribution terms as the rest of that program.

AC_DEFUN([gl_GETNDELIM2],
[
  dnl The fix to Autoconf that forbids multiple inclusions in LIBOBJ of the
  dnl same filename went in before release 2.58, so after GNULIB requires
  dnl Autoconf 2.58 or greater, the variable set below and check to prevent
  dnl running AC_LIBOBJ twice will no longer be necessary.
  if test -z "$gl_getndelim2_invoked"; then
    gl_getndelim2_invoked=:
    AC_LIBOBJ(getndelim2)
  fi
  gl_PREREQ_GETNDELIM2
])

# Prerequisites of lib/getndelim2.h and lib/getndelim2.c.
AC_DEFUN([gl_PREREQ_GETNDELIM2],
[
  dnl Prerequisites of lib/getndelim2.h.
  AC_REQUIRE([gt_TYPE_SSIZE_T])
  dnl No prerequisites of lib/getndelim2.c.
])
