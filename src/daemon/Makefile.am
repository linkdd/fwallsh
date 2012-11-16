sbin_PROGRAMS = fwallshd

fwallshd_SOURCES = main.c local.h daemon.c daemon.h module.c module.h socket.c socket.h
fwallshd_CFLAGS  = -I$(srcdir)/../fwallsh
fwallshd_LDADD   = ../common/libfwallsh-common.a -ldl $(sqlite3_LIBS)
