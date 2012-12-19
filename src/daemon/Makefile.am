sbin_PROGRAMS = fwallshd

fwallshd_SOURCES = main.c local.h daemon.c daemon.h module.c module.h socket.c socket.h ssl.c ssl.h
fwallshd_CFLAGS  = -I$(srcdir)/../fwallsh $(OpenSSL_CFLAGS)
fwallshd_LDADD   = ../common/libfwallsh-common.a $(sqlite3_LIBS) $(dl_LIBS) $(OpenSSL_LIBS)
