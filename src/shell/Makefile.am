bin_PROGRAMS = fwallsh

fwallsh_SOURCES = main.c local.h
fwallsh_CFLAGS  = -I$(srcdir)/../fwallsh
fwallsh_LDADD   = ../common/libfwallsh-common.a -ldl $(sqlite3_LIBS) -lreadline
