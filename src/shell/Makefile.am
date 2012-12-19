bin_PROGRAMS = fwallsh

fwallsh_SOURCES = main.c local.h
fwallsh_CFLAGS  = -I$(srcdir)/../fwallsh $(sqlite3_CFLAGS) $(OpenSSL_CFLAGS)
fwallsh_LDADD   = ../common/libfwallsh-common.a $(sqlite3_LIBS) $(readline_LIBS) $(OpenSSL_LIBS)
