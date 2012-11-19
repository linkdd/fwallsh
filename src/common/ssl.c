#include <fwallsh/common/common.h>

static int (*ssl_error_handler) (char const * const, size_t, void *) = NULL;

/*! Internal method for socket_ssl_t::new() */
static bool _socket_ssl_new (struct socket_ssl_t *s, SSL_CTX *ctx)
{
     struct socket_t *sock = (struct socket_t *) sock;

     /* create SSL structure */
     s->ssl = SSL_new (ctx);

     if (s->ssl == NULL)
     {
          return false;
     }

     /* Assign the socket to the SSL structure */
     SSL_set_fd (s->ssl, sock->fd);

     return true;
}

/*! Internal method for socket_ssl_t::handshake() */
static bool _socket_ssl_handshake (struct socket_ssl_t *s)
{
     return SSL_accept (s->ssl);
}

/*! Internal method for socket_t::shutdown() */
static bool _socket_ssl_shutdown (struct socket_t *s)
{
     struct socket_ssl_t *ssl = (struct socket_ssl_t *) s;
     bool success = true;

     if (ssl->ssl != NULL)
     {
          /* close SSL connection */
          if (SSL_shutdown (ssl->ssl) < 0)
          {
               success = false;
          }

          /* free memory used by SSL structure */
          SSL_free (ssl->ssl);
     }

     /* close socket */
     if (close (s->fd) < 0)
     {
          success = false;
     }

     return success;
}

/*! Internal method for socket_t::connect() */
static bool _socket_ssl_connect (struct socket_t *s)
{
     struct socket_ssl_t *ssl = (struct socket_ssl_t *) s;

     return SSL_connect (ssl->ssl);
}

/*! Internal method for socket_t::write() */
static ssize_t _socket_ssl_write (struct socket_t *s, void * const buf, size_t len, int flags)
{
     struct socket_ssl_t *ssl = (struct socket_ssl_t *) s;
     return SSL_write (ssl->ssl, buf, len);
}

/*! Internal method for socket_t::read() */
static ssize_t _socket_ssl_read (struct socket_t *s, void * const buf, size_t len, int flags)
{
     struct socket_ssl_t *ssl = (struct socket_ssl_t *) s;
     return SSL_read (ssl->ssl, buf, len);
}

void socket_ssl_init (struct socket_ssl_t *ssl)
{
     struct socket_t *s = (struct socket_t *) ssl;

     if (ssl != NULL)
     {
          s->shutdown    = _socket_ssl_shutdown;
          s->connect     = _socket_ssl_connect;
          s->write       = _socket_ssl_write;
          s->read        = _socket_ssl_read;

          ssl->new       = _socket_ssl_new;
          ssl->handshake = _socket_ssl_handshake;
     }
}

void ssl_print_errors (char *preffix)
{
     if (ssl_error_handler != NULL)
     {
          ERR_print_errors_cb (ssl_error_handler, (void *) preffix);
     }
}

void ssl_set_error_handler (int (*cb) (char const * const, size_t, void *))
{
     ssl_error_handler = cb;
}

