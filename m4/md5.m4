# md5.m4 serial 6
dnl Copyright (C) 2002, 2003, 2004 Free Software Foundation, Inc.
dnl This file is free software; the Free Software Foundation
dnl gives unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.

AC_DEFUN([gl_MD5],
[
  dnl Prerequisites of lib/md5.h.
  AC_REQUIRE([gl_AC_TYPE_UINT32_T])

  dnl Prerequisites of lib/md5.c.
  AC_REQUIRE([AC_C_BIGENDIAN])
  :
])
