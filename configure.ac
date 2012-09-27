AC_INIT([fwallsh], [0.1], [david.jose.delassus@gmail.com])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])

AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_RANLIB

AC_CONFIG_HEADERS([fwallsh-config.h])
AC_CHECK_HEADER([stdio.h])
AC_CHECK_HEADER([stdlib.h])
AC_CHECK_HEADER([stdarg.h])
AC_CHECK_HEADER([stdbool.h])
AC_CHECK_HEADER([unistd.h])
AC_CHECK_HEADER([string.h])
AC_CHECK_HEADER([sys/stat.h])
AC_CHECK_HEADER([sys/types.h])
AC_CHECK_HEADER([errno.h])
AC_CHECK_HEADER([err.h])
AC_CHECK_HEADER([dlfcn.h])
AC_CHECK_HEADER([syslog.h])

AC_CONFIG_FILES([
          Makefile
          src/common/Makefile
          src/daemon/Makefile
])

AC_OUTPUT
