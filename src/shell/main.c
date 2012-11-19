#include <fwallsh/shell/local.h>

struct global_t global;

/*! \brief SSL Error Handler */
static int _ssl_error_handler (char const * const error, size_t len, void *data)
{
     int ret = 0;

     if (data != NULL)
     {
          char *preffix = (char *) data;

          ret = fprintf (stderr, "fwallsh: SSL: %s: %s\n", preffix, error);
     }
     else
     {
          ret = fprintf (stderr, "fwallsh: SSL: %s\n", error);
     }

     return ret;
}

/*!
 * \brief Initialize SSL.
 *
 * \return true on success, false on error.
 */
bool init_ssl (void)
{
     SSL_METHOD const *method = NULL;

     /* initialize OpenSSL library */
     SSL_library_init ();
     SSL_load_error_strings ();

     ssl_set_error_handler (_ssl_error_handler);

     method = SSLv3_method ();

     global.ssl.ctx = SSL_CTX_new (method);

     if (global.ssl.ctx == NULL)
     {
          ssl_print_errors ("SSL_CTX_new()");
          return false;
     }

     /* load certificate public key */
     if (SSL_CTX_use_certificate_file (global.ssl.ctx, global.ssl.cert, SSL_FILETYPE_PEM) < 0)
     {
          ssl_print_errors ("SSL_CTX_use_certificate_file()");
          SSL_CTX_free (global.ssl.ctx);
          return false;
     }

     /* load private key */
     if (SSL_CTX_use_PrivateKey_file (global.ssl.ctx, global.ssl.key, SSL_FILETYPE_PEM) < 0)
     {
          ssl_print_errors ("SSL_CTX_use_PrivateKey_file()");
          SSL_CTX_free (global.ssl.ctx);
          return false;
     }

     /* check if the keys pair match */
     if (!SSL_CTX_check_private_key (global.ssl.ctx))
     {
          fprintf (stderr, "fwallsh: SSL: Private key doesn't match the certificate public key.\n");
          SSL_CTX_free (global.ssl.ctx);
          return false;
     }

     /* load CA certificate for client authentication */
     if (!SSL_CTX_load_verify_locations (global.ssl.ctx, global.ssl.ca, NULL))
     {
          ssl_print_errors ("SSL_CTX_load_verify_locations()");
          SSL_CTX_free (global.ssl.ctx);
          return false;
     }

     SSL_CTX_set_verify (global.ssl.ctx, SSL_VERIFY_PEER, NULL);
     SSL_CTX_set_verify_depth (global.ssl.ctx, 1);

     return true;
}

/*!
 * \brief Initialize socket.
 * It creates and connect to the UNIX socket.
 *
 * \return true on success, false on error.
 */
bool init_socket (void)
{
     struct socket_t *s = (struct socket_t *) global.sock;

     socket_ssl_init (global.sock);

     /* create the socket */
     if (!s->create (s, "/tmp/fwallsh.sock"))
     {
          perror ("fwallsh: init[socket-create]");
          return false;
     }

     /* encapsulate the socket into SSL */
     if (!global.sock->new (global.sock, global.ssl.ctx))
     {
          ssl_print_errors ("s->new()");
          s->shutdown (s);
          return false;
     }

     /* connect to the socket */
     if (!s->connect (s))
     {
          ssl_print_errors ("s->connect()");
          s->shutdown (s);
          return false;
     }

     return true;
}

/*!
 * \brief Read line from input.
 * Using readline(), reads a line from input.
 * It loops until a non-empty line is read, and
 * adds it to the history.
 *
 * \param prt Prompt to print when asking the line.
 * \return The input line. Must be freed with free() and not deallocate().
 */
char *prompt (char const * const prt)
{
     char *line = NULL;

     while (line == NULL)
     {
          line = readline (prt);
     }

     add_history (line);

     return line;
}

/*!
 * \brief Main loop.
 *
 */
void main_loop (void)
{
     struct socket_t *s = (struct socket_t *) global.sock;

     char buffer[BUFSIZ] = {0};
     bool done = false;

     while (!done)
     {
          char *line = prompt ("> ");
          ssize_t n = 0;

          /* send command */
          if ((n = s->write (s, line, strlen (line), 0)) < 0)
          {
               ssl_print_errors ("s->write()");
          }

          /* free command */
          free (line);

          /* receive answer */
          if (n > 0 && (n = s->read (s, buffer, BUFSIZ, 0)) < 0)
          {
               ssl_print_errors ("s->read()");
          }

          /* print answer */
          if (n > 0)
          {
               buffer[n] = 0;
               printf ("%s\n", buffer);

               if (!strcmp (buffer, "exit"))
               {
                    done = true;
               }
          }
     }

     s->shutdown (s);
}

int main (int argc, char **argv)
{
     global.sock = allocate (sizeof (struct socket_ssl_t));

     /* initialize SSL */
     if (!init_ssl ())
     {
          deallocate (global.sock);
          exit (EXIT_FAILURE);
     }

     /* initialize the socket */
     if (!init_socket ())
     {
          deallocate (global.sock);
          exit (EXIT_FAILURE);
     }

     /* run main loop */
     main_loop ();

     deallocate (global.sock);

     SSL_CTX_free (global.ssl.ctx);

     return EXIT_SUCCESS;
}
