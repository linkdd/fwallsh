#ifndef __LOCAL_H
#define __LOCAL_H

#include <fwallsh/common/common.h>

#include <stdio.h>

#include <readline/readline.h>
#include <readline/history.h>

struct global_t
{
     struct socket_ssl_t *sock;

     struct
     {
          SSL_CTX *ctx;
          char *cert;
          char *key;
          char *ca;
     } ssl;
};

extern struct global_t global;

#endif /* __LOCAL_H */
