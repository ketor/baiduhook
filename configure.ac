#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(bhook, 1.0.0, hyxbiao@gmail.com)
#AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_HEADERS([build-aux/config.h])
#AC_CONFIG_SRCDIR([src/AsmHelper.cpp])
AC_CONFIG_FILES([Makefile
				 preload/disk/Makefile
				 preload/memory/Makefile
				 preload/net/Makefile])
AC_CONFIG_FILES([scripts/bhook], [chmod +x scripts/bhook])
AC_CONFIG_FILES([test/Makefile.env])
AC_CONFIG_FILES([test/RunEnv.sh])

AM_INIT_AUTOMAKE(foreign)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
#AC_PROG_LIBTOOL
AC_PROG_RANLIB

AC_ARG_WITH([binutils],
            [AS_HELP_STRING([--with-binutils],
               [use binutils lib(default is no)])],
            [with_binutils=$withval],
            [with_binutils=no])

AS_IF([test "x$with_binutils" != "xno"],
	  [
	   CPPFLAGS="-I$with_binutils/include $CPPFLAGS"
	   LDFLAGS="-L$with_binutils/lib -L$with_binutils/lib64 $LDFLAGS"
	   BINUTILS_PATH="$with_binutils"
	   ])

AC_SUBST(BINUTILS_PATH)
# Checks for libraries.
AC_CHECK_LIB([pthread], [pthread_create])
AC_CHECK_LIB([bfd], [bfd_demangle], [have_demangle=yes], [], [-lopcodes -liberty -lz])
#AC_SEARCH_LIBS([bfd_demangle], [bfd], [have_demangle=yes])
AS_IF([test "x$have_demangle" == "xyes"],  
	  [
	   LDFLAGS="-DHAVE_BFD_DEMANGLE $LDFLAGS"
	   ])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([fcntl.h malloc.h stdlib.h string.h unistd.h])
AC_CHECK_HEADERS([dis-asm.h], [have_disasm=yes])
AS_IF([test "x$have_disasm" != "xyes"],  
	  [AC_MSG_FAILURE([missing dis-asm.h])])

AC_CHECK_HEADERS([bfd.h], [have_bfd=yes])
AS_IF([test "x$have_bfd" != "xyes"],  
	  [AC_MSG_FAILURE([missing bfd.h])])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_MODE_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset mkfifo])

AC_OUTPUT
