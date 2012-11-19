#include <fwallsh/daemon/local.h>

/*! \brief SSL Error Handler */
static int _ssl_error_handler (char const * const error, size_t len, void *data)
{
     if (data != NULL)
     {
          char *preffix = (char *) data;

          syslog (LOG_ERR, "SSL: %s: %s", preffix, error);
     }
     else
     {
          syslog (LOG_ERR, "SSL: %s", error);
     }

     return len;
}

bool initialize_ssl (void)
{
     SSL_METHOD const *method = NULL;

     /* initialize OpenSSL library */
     SSL_library_init ();
     SSL_load_error_strings ();

     ssl_set_error_handler (_ssl_error_handler);

     /* allocate SSL context */
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
          syslog (LOG_ERR, "SSL: Private key doesn't match the certificate public key.");
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

     /* define verify callback and verify depth */
     SSL_CTX_set_verify (global.ssl.ctx, SSL_VERIFY_PEER, NULL);
     SSL_CTX_set_verify_depth (global.ssl.ctx, 1);

     return true;
}

