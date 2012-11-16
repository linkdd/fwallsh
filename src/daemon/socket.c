#include <fwallsh/daemon/local.h>

bool initialize_socket (void)
{
     struct socket_t *s = allocate (sizeof (struct socket_t));

     /* first, initialize the socket */
     socket_init (s);

     unlink ("/tmp/fwallsh.sock");

     if (!s->create (s, "/tmp/fwallsh.sock"))
     {
          syslog (LOG_ERR, "s->create('%s'): %s", "/tmp/fwallsh.sock", strerror (errno));
          deallocate (s);
          return false;
     }

     /* bind to the socket */
     if (!s->bind (s))
     {
          syslog (LOG_ERR, "s->bind(): %s", strerror (errno));
          s->shutdown (s);
          deallocate (s);
          return false;
     }

     /* listen incomming connections */
     if (!s->listen (s, 5))
     {
          syslog (LOG_ERR, "s->listen(): %s", strerror (errno));
          s->shutdown (s);
          deallocate (s);
          return false;
     }

     /* initialize the sockets set */

     socket_set_zero (&(global.master));

     socket_set_add (&(global.master), s);
     global.fdmax = s->fd;

     global.listener = s;

     return true;
}

void handle_client_socket (struct socket_set_t *sset, struct socket_t *s, void *user_data)
{
     char buffer[BUFSIZ] = {0};
     ssize_t n = 0;

     if ((n = s->read (s, buffer, BUFSIZ, 0)) < 0)
     {
          syslog (LOG_ERR, "s->read(): %s", strerror (errno));
          return;
     }
     else if (n == 0)
     {
          socket_set_remove (&global.master, s);
          deallocate (s);
          return;
     }

     buffer[n] = 0;

     if ((n = s->write (s, buffer, strlen (buffer), 0)) < 0)
     {
          syslog (LOG_ERR, "s->write(): %s", strerror (errno));
     }
}

void handle_socket (struct socket_set_t *sset, struct socket_t *s, void *user_data)
{
     /* check if the socket is set */
     if (socket_set_isset (sset, s))
     {
          /* if the selected socket is the listener */
          if (s == global.listener)
          {
               struct socket_t *cs = allocate (sizeof (struct socket_t));

               /* initialize client socket */
               socket_init (cs);

               /* accept the incoming connection */
               if (!s->accept (s, cs))
               {
                    syslog (LOG_ERR, "s->accept(): %s", strerror (errno));
                    deallocate (cs);
                    return;
               }

               /* add the client socket to the master set */
               socket_set_add (&global.master, cs);

               if (cs->fd > global.fdmax)
               {
                    global.fdmax = cs->fd;
               }
          }
          /* the selected socket is a client socket */
          else
          {
               handle_client_socket (sset, s, user_data);
          }
     }
}


