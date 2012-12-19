AC_INIT([fwallsh], [0.1], [david.jose.delassus@gmail.com])
AM_INIT_AUTOMAKE([foreign -Wall])

CFLAGS+="-Wall -Werror"

AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_RANLIB

PKG_CHECK_MODULES(sqlite3, [sqlite3])

LIBS_OLD=$LIBS

AC_CHECK_LIB([dl], [dlopen],
          [
               dl_LIBS="-ldl"
               AC_ARG_VAR(dl_LIBS, "dynamic loading libraries")
          ],
          [AC_MSG_ERROR("Can't find dynamic loading library.")])

AC_CHECK_LIB([readline], [readline],
          [
               readline_LIBS="-lreadline"
               AC_ARG_VAR(readline_LIBS, "readline libraries")
          ],
          [AC_MSG_ERROR("Can't find readline library.")])

LIBS=$LIBS_OLD

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
          src/shell/Makefile
])

AC_OUTPUT
