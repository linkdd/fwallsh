#ifndef __DAEMON_SOCKET_H
#define __DAEMON_SOCKET_H

#include <fwallsh/common/common.h>

/*!
 * \brief Create UNIX socket.
 *
 * \return true on success, false on error.
 */
bool initialize_socket (void);

/*!
 * \brief Handle client socket.
 *
 * \param sset The socket set where s is contained.
 * \param s The client socket.
 * \param user_data Data passed through socket_set_foreach().
 */
void handle_client_socket (struct socket_set_t *sset, struct socket_t *s, void *user_data);

/*!
 * \brief Handle socket.
 *
 * \param sset The socket set where s is contained.
 * \param s The socket.
 * \param user_data Data passed through socket_set_foreach().
 */
void handle_socket (struct socket_set_t *sset, struct socket_t *s, void *user_data);

#endif /* __DAEMON_SOCKET_H */
