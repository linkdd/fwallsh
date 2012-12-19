#ifndef __LOCAL_H
#define __LOCAL_H

#include <fwallsh/common/common.h>

#include <fwallsh/daemon/daemon.h>
#include <fwallsh/daemon/module.h>
#include <fwallsh/daemon/socket.h>

/*! \brief Global structure */
struct global_t
{
     struct socket_t *listener;    /*!< Socket which listens for incomming connection. */
     struct socket_set_t master;   /*!< Set of sockets, for watching I/O operations. */
     int fdmax;                    /*!< Highest file descriptor number. */
};

extern struct global_t global;

#endif /* __LOCAL_H */
