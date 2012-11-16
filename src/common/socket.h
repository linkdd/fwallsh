#ifndef __SOCKET_H
#define __SOCKET_H

#include <fwallsh/common/types.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

/*!
 * \brief UNIX Socket structure.
 *
 * Used to create server/client UNIX socket, it encapsulate
 * the whole management of sockets.
 */
struct socket_t
{
     int fd;                  /*!< Socket file descriptor. */
     struct sockaddr_un addr; /*!< UNIX socket address. */
     socklen_t addrlen;       /*!< Size of the socket address. */

     /*!
      * \fn bool socket_t::create (struct socket_t *s, char const * const path)
      * \brief Create the socket.
      *
      * This function calls socket() and then initialize socket_t::addr.
      *
      * \param s A socket structure.
      * \param path Path to the UNIX socket.
      * \return true on success, false on error and errno is set.
      */
     bool (*create) (struct socket_t *, char const * const);

     /*!
      * \fn bool socket_t::shutdown (struct socket_t *s)
      * \brief Close the socket.
      *
      * This function calls close(). No memory is freed.
      *
      * \param s A socket structure.
      * \return true on success, false on error and errno is set.
      */
     bool (*shutdown) (struct socket_t *);

     /*!
      * \fn bool socket_t::bind (struct socket_t *s)
      * \brief Assign an address to the socket.
      *
      * This function calls bind() and should be called
      * only by the daemon.
      *
      * \param s A socket structure.
      * \return true on success, false on error and errno is set.
      */
     bool (*bind) (struct socket_t *);

     /*!
      * \fn bool socket_t::listen (struct socket_t *s, int backlog)
      * \brief Make the socket passive.
      *
      * This function calls listen() and should be called
      * only by the daemon.
      *
      * \param s A socket structure.
      * \param backlog Maximum number of pending connections.
      * \return true on success, false on error and errno is set.
      */
     bool (*listen) (struct socket_t *, int);

     /*!
      * \fn bool socket_t::accept (struct socket_t *s, struct socket_t *peer)
      * \brief Accept an incomming connection.
      *
      * This function calls accept() and should be called
      * only by the daemon.
      *
      * \param s A socket structure.
      * \param peer The client socket structure.
      * \return true on success, false on error and errno is set.
      */
     bool (*accept) (struct socket_t *, struct socket_t *);

     /*!
      * \fn bool socket_t::connect (struct socket_t *s)
      * \brief Connect to a remote socket.
      *
      * This function calls connect() and should be called
      * only by the shell.
      *
      * \param s A socket structure.
      * \return true on success, false on error and errno is set.
      */
     bool (*connect) (struct socket_t *);

     /*!
      * \fn ssize_t socket_t::write (struct socket_t *s, void * const buf, size_t len, int flags)
      * \brief Write data to the socket.
      *
      * This function calls send().
      *
      * \param s A socket structure.
      * \param buf Data to send.
      * \param len Size of data to send.
      * \param flags See documentation for send().
      * \return Number of bytes written, or -1 on error with errno set.
      */
     ssize_t (*write) (struct socket_t *, void * const, size_t, int);

     /*!
      * \fn ssize_t socket_t::read (struct socket_t *s, void *buf, size_t len, int flags)
      * \brief Read data from the socket.
      *
      * This function calls recv().
      *
      * \param s A socket structure.
      * \param buf Buffer for received data.
      * \param len Size of the buffer.
      * \param flags See documentation for recv().
      * \return Number of read bytes, or -1 on error with errno set.
      */
     ssize_t (*read) (struct socket_t *, void *, size_t, int);
};

/*!
 * \brief Wrapper to fd_set structure.
 *
 * Allow to manage multiple socket I/O operation.
 */
struct socket_set_t
{
     struct socket_t *s[FD_SETSIZE];    /*!< Array of socket_t structures. */
     fd_set set;                        /*!< fd_set structure. */
};

/*!
 * \fn void socket_set_foreach_fn (struct socket_set_t *sset, struct socket_t *s, void *data)
 * \brief Called by socket_set_foreach().
 *
 * \param sset A socket_set_t structure.
 * \param s The current socket_t structure in the set.
 * \param data Data passed via socket_set_foreach().
 */
typedef void (*socket_set_foreach_fn) (struct socket_set_t *, struct socket_t *, void *);

/*!
 * \brief Initialize a socket structure.
 *
 * Defines the function pointers in the socket structure with
 * internal functions.
 *
 * \param s A socket structure.
 */
void socket_init (struct socket_t *s);

/*!
 * \brief Allow monitoring multiple sockets.
 *
 * This function calls select().
 *
 * \param fdmax Highest-numbered file descriptor in any of the three sets, plus 1.
 * \param readset Will be watched to see if bytes are available for reading on one or more sockets.
 * \param writeset Will be watched to see if one or more sockets are ready to write.
 * \param exceptset Will be watched for exceptions.
 * \param timeout Specifies the minimum interval that select() should block for a file descriptor to become ready.
 * \return Number of file descriptors contained in the three returned descriptor sets which may be zero if the timeout expires, or -1 on error with errno set.
 */
int socket_select (int fdmax,
                   struct socket_set_t *readset,
                   struct socket_set_t *writeset,
                   struct socket_set_t *exceptset,
                   struct timeval *timeout);

/*!
 * \brief Clears the socket set.
 *
 * This function calls FD_ZERO().
 *
 * \param sset A socket_set_t structure.
 */
void socket_set_zero (struct socket_set_t *sset);

/*!
 * \brief Add a socket to the set.
 *
 * This function calls FD_SET().
 *
 * \param sset A socket_set_t structure.
 * \param s The socket_t structure to add.
 */
void socket_set_add (struct socket_set_t *sset, struct socket_t *s);

/*!
 * \brief Remove a socket from the set.
 *
 * This function calls FD_CLR().
 *
 * \param sset A socket_set_t structure.
 * \param s The socket_t structure to remove.
 */
void socket_set_remove (struct socket_set_t *sset, struct socket_t *s);

/*!
 * \brief Check if there was I/O operations on a socket.
 *
 * This function calls FD_ISSET().
 *
 * \param sset A socket_set_t structure.
 * \param s A socket_t structure.
 * \return true if there was I/O operations, false otherwise.
 */
bool socket_set_isset (struct socket_set_t *sset, struct socket_t *s);

/*!
 * \brief Loop on each socket in the set.
 *
 * \param sset A socket_set_t structure.
 * \param caller Function to call for each socket (see socket_set_foreach_fn).
 * \param data Data to pass to the caller.
 */
void socket_set_foreach (struct socket_set_t *sset, socket_set_foreach_fn caller, void *data);

#endif /* __SOCKET_H */
