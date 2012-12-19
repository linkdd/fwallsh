#!/bin/sh

bootstrap()
{
     autoreconf --install
}

clean()
{
     find -name "Makefile.in" -printf "bootstrap.sh: removing \`%p'\n" -delete

     for f in aclocal.m4 autom4te.cache compile configure depcomp install-sh missing fwallsh-config.h.in
     do
          echo "bootstrap.sh: removing \`$f'"
          rm -rf $f > /dev/null
     done
}

if [ "$1" = "--clean" ]
then
     clean
else
     bootstrap
fi
