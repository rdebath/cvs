# Copyright (C) 2004 Free Software Foundation, Inc.
# This file is free software, distributed under the terms of the GNU
# General Public License.  As a special exception to the GNU General
# Public License, this file may be distributed as part of a program
# that contains a configuration script generated by Autoconf, under
# the same distribution terms as the rest of that program.
#
# Generated by gnulib-tool.
#
# This file represents the compiled summary of the specification in
# gnulib-cache.m4. It lists the computed macro invocations that need
# to be invoked from configure.ac.
# In projects using CVS, this file can be treated like other built files.


# This macro should be invoked from ./configure.in, in the section
# "Checks for programs", right after AC_PROG_CC, and certainly before
# any checks for libraries, header files, types and library functions.
AC_DEFUN([gl_EARLY],
[
  AC_REQUIRE([AC_GNU_SOURCE])
  AC_REQUIRE([gl_USE_SYSTEM_EXTENSIONS])
])

# This macro should be invoked from ./configure.in, in the section
# "Check for header files, types and library functions".
AC_DEFUN([gl_INIT],
[
  gl_FUNC_ALLOCA
  gl_ALLOCSA
  gl_FUNC_ATEXIT
  AC_FUNC_CANONICALIZE_FILE_NAME
  gl_FUNC_CHDIR_LONG
  gl_CLOSEOUT
  gl_DIRNAME
  gl_FUNC_DUP2
  gl_ERROR
  gl_EXITFAIL
  dnl gl_USE_SYSTEM_EXTENSIONS must be added quite early to configure.ac.
  gl_FILE_NAME_CONCAT
  # No macro. You should also use one of fnmatch-posix or fnmatch-gnu.
  gl_FUNC_FNMATCH_POSIX
  gl_FUNC_FPENDING
  gl_FUNC_FTRUNCATE
  gl_FUNC_GETCWD
  gl_GETDATE
  gl_FUNC_GETDELIM
  gl_FUNC_GETHOSTNAME
  gl_FUNC_GETLINE
  gl_GETLOGIN_R
  gl_GETNDELIM2
  gl_GETNLINE
  gl_GETOPT
  gl_GETPAGESIZE
  gl_FUNC_GETPASS_GNU
  dnl you must add AM_GNU_GETTEXT([external]) or similar to configure.ac.
  gl_GETTIME
  AC_FUNC_GETTIMEOFDAY_CLOBBER
  gl_FUNC_LSTAT
  AC_FUNC_MALLOC
  gl_MBCHAR
  gl_MBITER
  gl_MD5
  gl_FUNC_MEMCHR
  gl_FUNC_MEMMOVE
  gl_FUNC_MEMPCPY
  gl_FUNC_MEMRCHR
  gl_MINMAX
  gl_FUNC_MKDIR_TRAILING_SLASH
  gl_FUNC_MKSTEMP
  gl_FUNC_MKTIME
  gl_FUNC_NANOSLEEP
  gl_FUNC_OPENAT
  gl_PAGEALIGN_ALLOC
  gl_PATHMAX
  gl_QUOTEARG
  gl_FUNC_READLINK
  AC_FUNC_REALLOC
  gl_REGEX
  vb_FUNC_RENAME
  gl_C_RESTRICT
  gl_FUNC_RPMATCH
  gl_SAVE_CWD
  gt_FUNC_SETENV
  gl_SIZE_MAX
  gt_TYPE_SSIZE_T
  gl_STAT_MACROS
  AM_STDBOOL_H
  gl_STDINT_H
  gl_STRCASE
  gl_FUNC_STRDUP
  gl_FUNC_STRERROR
  gl_FUNC_GNU_STRFTIME
  gl_FUNC_STRSTR
  gl_FUNC_STRTOL
  gl_FUNC_STRTOUL
  gl_TIME_R
  gl_TIMESPEC
  gl_FUNC_TZSET_CLOBBER
  gl_UNISTD_SAFER
  gl_FUNC_GLIBC_UNLOCKED_IO
  gl_FUNC_VASNPRINTF
  gl_FUNC_VASPRINTF
  gl_XALLOC
  gl_XGETCWD
  gl_XREADLINK
  gl_XSIZE
  gl_YESNO
])

# This macro records the list of files which have been installed by
# gnulib-tool and may be removed by future gnulib-tool invocations.
AC_DEFUN([gl_FILE_LIST], [
  build-aux/config.rpath
  doc/getdate.texi
  lib/alloca.c
  lib/alloca_.h
  lib/allocsa.c
  lib/allocsa.h
  lib/allocsa.valgrind
  lib/asnprintf.c
  lib/asprintf.c
  lib/atexit.c
  lib/basename.c
  lib/canonicalize.c
  lib/canonicalize.h
  lib/chdir-long.c
  lib/chdir-long.h
  lib/closeout.c
  lib/closeout.h
  lib/cycle-check.c
  lib/cycle-check.h
  lib/dev-ino.h
  lib/dirname.c
  lib/dirname.h
  lib/dup2.c
  lib/dup-safer.c
  lib/error.c
  lib/error.h
  lib/exitfail.c
  lib/exitfail.h
  lib/exit.h
  lib/fd-safer.c
  lib/filenamecat.c
  lib/filenamecat.h
  lib/fnmatch.c
  lib/fnmatch_.h
  lib/fnmatch_loop.c
  lib/__fpending.c
  lib/__fpending.h
  lib/ftruncate.c
  lib/getcwd.c
  lib/getcwd.h
  lib/getdate.h
  lib/getdate.y
  lib/getdelim.c
  lib/getdelim.h
  lib/gethostname.c
  lib/getline.c
  lib/getline.h
  lib/getlogin_r.c
  lib/getlogin_r.h
  lib/getndelim2.c
  lib/getndelim2.h
  lib/getnline.c
  lib/getnline.h
  lib/getopt1.c
  lib/getopt.c
  lib/getopt_.h
  lib/getopt_int.h
  lib/getpagesize.h
  lib/getpass.c
  lib/getpass.h
  lib/gettext.h
  lib/gettime.c
  lib/gettimeofday.c
  lib/lstat.c
  lib/lstat.h
  lib/malloc.c
  lib/mbchar.c
  lib/mbchar.h
  lib/mbuiter.h
  lib/md5.c
  lib/md5.h
  lib/memchr.c
  lib/memmove.c
  lib/mempcpy.c
  lib/mempcpy.h
  lib/memrchr.c
  lib/memrchr.h
  lib/minmax.h
  lib/mkdir.c
  lib/mkstemp.c
  lib/mktime.c
  lib/nanosleep.c
  lib/openat.c
  lib/openat.h
  lib/pagealign_alloc.c
  lib/pagealign_alloc.h
  lib/pathmax.h
  lib/pipe-safer.c
  lib/printf-args.c
  lib/printf-args.h
  lib/printf-parse.c
  lib/printf-parse.h
  lib/quotearg.c
  lib/quotearg.h
  lib/readlink.c
  lib/realloc.c
  lib/regcomp.c
  lib/regex.c
  lib/regexec.c
  lib/regex.h
  lib/regex_internal.c
  lib/regex_internal.h
  lib/rename.c
  lib/rpmatch.c
  lib/save-cwd.c
  lib/save-cwd.h
  lib/setenv.c
  lib/setenv.h
  lib/size_max.h
  lib/stat-macros.h
  lib/stdbool_.h
  lib/stdint_.h
  lib/strcasecmp.c
  lib/strcase.h
  lib/strdup.c
  lib/strdup.h
  lib/strerror.c
  lib/strftime.c
  lib/strftime.h
  lib/stripslash.c
  lib/strncasecmp.c
  lib/strnlen1.c
  lib/strnlen1.h
  lib/strstr.c
  lib/strstr.h
  lib/strtol.c
  lib/strtoul.c
  lib/tempname.c
  lib/time_r.c
  lib/time_r.h
  lib/timespec.h
  lib/unistd--.h
  lib/unistd-safer.h
  lib/unlocked-io.h
  lib/unsetenv.c
  lib/vasnprintf.c
  lib/vasnprintf.h
  lib/vasprintf.c
  lib/vasprintf.h
  lib/xalloc-die.c
  lib/xalloc.h
  lib/xgetcwd.c
  lib/xgetcwd.h
  lib/xgethostname.c
  lib/xgethostname.h
  lib/xmalloc.c
  lib/xreadlink.c
  lib/xreadlink.h
  lib/xsize.h
  lib/yesno.c
  lib/yesno.h
  m4/alloca.m4
  m4/allocsa.m4
  m4/atexit.m4
  m4/bison.m4
  m4/canonicalize.m4
  m4/chdir-long.m4
  m4/clock_time.m4
  m4/closeout.m4
  m4/codeset.m4
  m4/d-ino.m4
  m4/dirname.m4
  m4/dos.m4
  m4/dup2.m4
  m4/eealloc.m4
  m4/eoverflow.m4
  m4/error.m4
  m4/exitfail.m4
  m4/extensions.m4
  m4/filenamecat.m4
  m4/fnmatch.m4
  m4/fpending.m4
  m4/ftruncate.m4
  m4/getcwd.m4
  m4/getcwd-path-max.m4
  m4/getdate.m4
  m4/getdelim.m4
  m4/gethostname.m4
  m4/getline.m4
  m4/getlogin_r.m4
  m4/getndelim2.m4
  m4/getnline.m4
  m4/getopt.m4
  m4/getpagesize.m4
  m4/getpass.m4
  m4/gettext.m4
  m4/gettime.m4
  m4/gettimeofday.m4
  m4/glibc21.m4
  m4/glibc2.m4
  m4/iconv.m4
  m4/intdiv0.m4
  m4/intmax.m4
  m4/intmax_t.m4
  m4/inttypes_h.m4
  m4/inttypes.m4
  m4/inttypes-pri.m4
  m4/isc-posix.m4
  m4/lcmessage.m4
  m4/lib-ld.m4
  m4/lib-link.m4
  m4/lib-prefix.m4
  m4/longdouble.m4
  m4/longlong.m4
  m4/lstat.m4
  m4/mbchar.m4
  m4/mbiter.m4
  m4/mbrtowc.m4
  m4/mbstate_t.m4
  m4/md5.m4
  m4/memchr.m4
  m4/memmove.m4
  m4/mempcpy.m4
  m4/memrchr.m4
  m4/minmax.m4
  m4/mkdir-slash.m4
  m4/mkstemp.m4
  m4/mktime.m4
  m4/mmap-anon.m4
  m4/nanosleep.m4
  m4/nls.m4
  m4/onceonly_2_57.m4
  m4/openat.m4
  m4/pagealign_alloc.m4
  m4/pathmax.m4
  m4/po.m4
  m4/printf-posix.m4
  m4/progtest.m4
  m4/quotearg.m4
  m4/readlink.m4
  m4/regex.m4
  m4/rename.m4
  m4/restrict.m4
  m4/rpmatch.m4
  m4/save-cwd.m4
  m4/setenv.m4
  m4/signed.m4
  m4/size_max.m4
  m4/ssize_t.m4
  m4/stat-macros.m4
  m4/stdbool.m4
  m4/stdint_h.m4
  m4/stdint.m4
  m4/st_mtim.m4
  m4/strcase.m4
  m4/strdup.m4
  m4/strerror.m4
  m4/strerror_r.m4
  m4/strftime.m4
  m4/strstr.m4
  m4/strtol.m4
  m4/strtoul.m4
  m4/time_r.m4
  m4/timespec.m4
  m4/tm_gmtoff.m4
  m4/tzset.m4
  m4/uint32_t.m4
  m4/uintmax_t.m4
  m4/ulonglong.m4
  m4/unistd-safer.m4
  m4/unlocked-io.m4
  m4/vasnprintf.m4
  m4/vasprintf.m4
  m4/wchar_t.m4
  m4/wint_t.m4
  m4/xalloc.m4
  m4/xgetcwd.m4
  m4/xreadlink.m4
  m4/xsize.m4
  m4/yesno.m4
])

# gnulib-comp.m4 ends here