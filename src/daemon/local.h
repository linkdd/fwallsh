#ifndef __LOCAL_H
#define __LOCAL_H

#include <fwallsh/common/common.h>

#include <fwallsh/daemon/daemon.h>
#include <fwallsh/daemon/module.h>
#include <fwallsh/daemon/socket.h>
#include <fwallsh/daemon/ssl.h>

/*! \brief Global structure */
struct global_t
{
     struct socket_t *listener;    /*!< Socket which listens for incomming connection. */
     struct socket_set_t master;   /*!< Set of sockets, for watching I/O operations. */
     int fdmax;                    /*!< Highest file descriptor number. */

     struct
     {
          SSL_CTX *ctx;           /*!< SSL context. */

          char *cert;             /*!< SSL certificate public key. */
          char *key;              /*!< SSL private key. */
          char *ca;               /*!< SSL CA certificate for client authentication. */
     } ssl;
};

extern struct global_t global;

#endif /* __LOCAL_H */
