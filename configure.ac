#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([dpt], [1.0.0], [toshiaki.takada@gmail.com])
AM_INIT_AUTOMAKE
#AC_CONFIG_SRCDIR([StatsStore.hpp])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lldns':
AC_CHECK_LIB([ldns], [main])
# FIXME: Replace `main' with a function in `-lmysqlpp':
AC_CHECK_LIB([mysqlpp], [main])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([pow sqrt])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
