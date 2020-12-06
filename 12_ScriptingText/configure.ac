#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([FULL-PACKAGE-NAME], [VERSION], [BUG-REPORT-ADDRESS])
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_SRCDIR([oracle.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([libintl.h locale.h string.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CHECK_FUNCS([setlocale])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

define(MYHEADER, [/\* COMMENT:: AC_PACKAGE_NAME, AC_PACKAGE_VERSION: `date` \*/])
define(SEDREGEX, [1s%/\* COMMENT:: .* \*/%MYHEADER%;t;1s%^%MYHEADER\n%])

sed -i "SEDREGEX" *.c *.h

