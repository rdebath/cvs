/* config.h - OpenVMS/AXP specific configuration
              June 1995 - <benjamin@cyclic.com> */

/* We only want to build the client */
#define CLIENT_SUPPORT 1
#undef SERVER_SUPPORT

/* VMS is case insensitive */
/* #define FOLD_FN_CHAR(c) tolower(c) */

/* Temporary files named "#booger.3.6~" aren't legal under VMS,
   Define this if you want to use names which are legal for VMS */
#define USE_VMS_FILENAMES 1

/* Only good for NT or DOS with hacked open */
/* #undef LINES_CRLF_TERMINATED */

/* Define to empty if the keyword does not work.  */
/* #undef const */

/* Define if you have <dirent.h>.  */
/* #undef DIRENT */

/* Define if you have <sys/param.h> */
/* #undef HAVE_SYS_PARAM_H */

/* Define to `int' if <sys/types.h> doesn't define.  */
/* #undef gid_t */

/* Define if you support file names longer than 14 characters.  */
/* #undef HAVE_LONG_FILE_NAMES */

/* Define if you have <sys/wait.h> that is POSIX.1 compatible.  */
/* #define HAVE_SYS_WAIT_H 1 OpenVMS POSIX has it, but VMS does not. */
#undef POSIX

/* Define if utime(file, NULL) sets file's timestamp to the present.  */
/* #undef HAVE_UTIME_NULL */

/* Define if on MINIX.  */
/* #undef _MINIX */

/* Define to `int' if <sys/types.h> doesn't define.  */
/* #undef mode_t */

/* Define if you don't have <dirent.h>, but have <ndir.h>.  */
/* #undef NDIR */

/* Define to `int' if <sys/types.h> doesn't define.  */
/* #undef pid_t */

/* Define if the system does not provide POSIX.1 features except
   with this defined.  */
/* #undef _POSIX_1_SOURCE */

/* Define if you need to in order for stat and other things to work.  */
/* #undef _POSIX_SOURCE */

/* Define as the return type of signal handlers (int or void).  */
#define RETSIGTYPE void

/* Define to `unsigned' if <sys/types.h> doesn't define.  */
/* #undef size_t */

/* Define if you have the ANSI C header files.  */
#define STDC_HEADERS 1

/* Define if you don't have <dirent.h>, but have <sys/dir.h>.  */
/* #undef SYSDIR */

/* Define if you don't have <dirent.h>, but have <sys/ndir.h>.  */
/* #undef SYSNDIR */

/* Define if your <sys/time.h> declares struct tm.  */
/* #undef TM_IN_SYS_TIME */

/* Define to `int' if <sys/types.h> doesn't define.  */
/* #undef uid_t */

/* Define if the closedir function returns void instead of int.  */
/* #undef VOID_CLOSEDIR */

/* Define if you want direct TCP access to server */
#define USE_DIRECT_TCP 1

/* Define if you have MIT Kerberos version 4 available.  */
/* #undef HAVE_KERBEROS */

/* The number of bytes in a int.  */
#define SIZEOF_INT 4

/* The number of bytes in a long.  */
#define SIZEOF_LONG 4

/* Define if you have the fchmod function.  */
/* #undef HAVE_FCHMOD */

/* Define if you have the fsync function.  */
/* #undef HAVE_FSYNC */

/* Define if you have the ftime function.  */
/* #undef HAVE_FTIME */

/* Define if you have the ftruncate function.  */
/* #undef HAVE_FTRUNCATE */

/* Define if you have the getpagesize function.  */
/* #undef HAVE_GETPAGESIZE */

/* Define if you have the krb_get_err_text function.  */
/* #undef HAVE_KRB_GET_ERR_TEXT */

/* Define if you have the mkdir function */
#define HAVE_MKDIR 1

/* Define if you have the rmdir function */
#define HAVE_RMDIR 1

/* Define if you have the rename function */
#define HAVE_RENAME 1

/* Define if you have the strdup function */
#define HAVE_STRDUP 1

/* Define if you have the mkfifo function.  */
/* #undef HAVE_MKFIFO */

/* Define if you have the putenv function.  */
/* #undef HAVE_PUTENV */

/* Define if you have the setvbuf function.  */
/* #undef HAVE_SETVBUF */

/* Define if you have the timezone function.  */
/* #undef HAVE_TIMEZONE */

/* Define if you have the vfork function.  */
#define HAVE_VFORK

/* Define if you have the vprintf function.  */
/* #undef HAVE_VPRINTF */

/* Define if you have the <errno.h> header file.  */
/* #undef HAVE_ERRNO_H */

/* Define if you have the <fcntl.h> header file.  */
/* #undef HAVE_FCNTL_H */

/* Define if you have the <memory.h> header file.  */
/* #undef HAVE_MEMORY_H */

/* Define if you have the <ndbm.h> header file.  */
/* #undef HAVE_NDBM_H */

/* Define if you have the <string.h> header file.  */
#define HAVE_STRING_H 1

/* Define if you have the <sys/select.h> header file.  */
/* #undef HAVE_SYS_SELECT_H */

/* Define this if your <sys/socket.h> defines select() */
#define SYS_SOCKET_H_DEFINES_SELECT 1

/* Define if you have the <sys/timeb.h> header file.  */
#define HAVE_SYS_TIMEB_H 1
#define HAVE_TIMEB_H 1

/* Define if you have the <unistd.h> header file.  */
#define HAVE_UNISTD_H 1

/* Define if you have the <utime.h> header file.  */
/* #undef HAVE_UTIME_H */

/* Define if you have the nsl library (-lnsl).  */
/* #undef HAVE_LIBNSL */

/* Define if you have the socket library (-lsocket).  */
/* #undef HAVE_LIBSOCKET */

#define RSH_NOT_TRANSPARENT 1
#define START_SERVER vms_start_server
#define NO_SOCKET_TO_FD 1
#define START_SERVER_RETURNS_SOCKET 1
#define SEND_NEVER_PARTIAL 1
