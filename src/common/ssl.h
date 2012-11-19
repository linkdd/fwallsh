#ifndef __SSL_H
#define __SSL_H

#include <fwallsh/common/types.h>

#include <fwallsh/common/socket.h>

#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

/*!
 * \brief UNIX Socket SSL structure.
 *
 * Used to create an SSL encapsulated socket.
 */
struct socket_ssl_t
{
     struct socket_t sock;    /*!< Real UNIX Socket. */

     SSL *ssl;                /*!< SSL socket. */

     /*!
      * \fn bool socket_ssl_t::new (struct socket_ssl_t *s, SSL_CTX *ctx)
      * \brief Create SSL structure.
      *
      * \param s A SSL socket structure.
      * \param ctx A SSL context structure.
      * \return true on success, false on error.
      */
     bool (*new) (struct socket_ssl_t *, SSL_CTX *);

     /*!
      * \fn bool socket_ssl_t::handshake (struct socket_ssl_t *s)
      * \brief Perform a SSL handshake.
      *
      * \param s A SSL socket structure.
      * \return true on success, false on error.
      */
     bool (*handshake) (struct socket_ssl_t *);
};

/*!
 * \brief Initialize SSL socket structure.
 *
 * \param s A socket_ssl_t structure to initialize.
 */
void socket_ssl_init (struct socket_ssl_t *s);

/*!
 * \brief Print SSL errors.
 *
 * This function calls ERR_print_errors_cb().
 *
 * \param preffix Message to print before the error (or NULL).
 */
void ssl_print_errors (char *preffix);

/*!
 * \brief Define function to call for error printing.
 *
 * \param cb Callback.
 */
void ssl_set_error_handler (int (*cb) (char const * const, size_t, void *));

#endif /* __SSL_H */
