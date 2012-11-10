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
      * \fn struct socket_t * socket_t::accept (struct socket_t *s)
      * \brief Accept an incomming connection.
      *
      * This function calls accept() and should be called
      * only by the daemon.
      *
      * \param s A socket structure.
      * \return the client socket structure on success, NULL on error and errno is set.
      */
     struct socket_t * (*accept) (struct socket_t *);

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
 * \brief Initialize a socket structure.
 *
 * Defines the function pointers in the socket structure with
 * internal functions.
 *
 * \param s A socket structure.
 */
void socket_init (struct socket_t *s);

#endif /* __SOCKET_H */
