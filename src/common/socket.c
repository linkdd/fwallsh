#include <fwallsh/common/common.h>

/*! Internal function for socket_t::create(). */
static bool _socket_create (struct socket_t *s, char const * const path)
{
     s->fd = socket (AF_UNIX, SOCK_STREAM, 0);

     if (s->fd == -1)
     {
          return false;
     }

     s->addr.sun_family = AF_UNIX;
     strcpy (s->addr.sun_path, path);
     s->addrlen = strlen (s->addr.sun_path) + sizeof (s->addr.sun_family);

     return true;
}

/*! Internal function for socket_t::shutdown(). */
static bool _socket_shutdown (struct socket_t *s)
{
     return (close (s->fd) == 0);
}

/*! Internal function for socket_t::bind(). */
static bool _socket_bind (struct socket_t *s)
{
     return (bind (s->fd, (struct sockaddr *) &(s->addr), s->addrlen) == 0);
}

/*! Internal function for socket_t::listen(). */
static bool _socket_listen (struct socket_t *s, int backlog)
{
     return (listen (s->fd, backlog) == 0);
}

/*! Internal function for socket_t::accept(). */
static struct socket_t *_socket_accept (struct socket_t *s)
{
     struct socket_t *peer = malloc (sizeof (struct socket_t));

     if (!peer)
     {
          errno = ENOMEM;
          return NULL;
     }

     peer->fd = accept (s->fd, (struct sockaddr *) &(s->addr), &(s->addrlen));

     if (peer->fd == -1)
     {
          free (peer);
          return NULL;
     }

     socket_init (peer);

     return peer;
}

/*! Internal function for socket_t::connect(). */
static bool _socket_connect (struct socket_t *s)
{
     return (connect (s->fd, (struct sockaddr *) &(s->addr), s->addrlen) == 0);
}

/*! Internal function for socket_t::write(). */
static ssize_t _socket_write (struct socket_t *s, void * const buf, size_t len, int flags)
{
     return send (s->fd, buf, len, flags);
}

/*! Internal function for socket_t::read(). */
static ssize_t _socket_read (struct socket_t *s, void *buf, size_t len, int flags)
{
     return recv (s->fd, buf, len, flags);
}

void socket_init (struct socket_t *s)
{
     if (s != NULL)
     {
          s->create   = _socket_create;
          s->shutdown = _socket_shutdown;

          s->bind     = _socket_bind;
          s->listen   = _socket_listen;
          s->accept   = _socket_accept;

          s->connect  = _socket_connect;

          s->write    = _socket_write;
          s->read     = _socket_read;
     }
}

int socket_select (int fdmax,
                   struct socket_set_t *readset,
                   struct socket_set_t *writeset,
                   struct socket_set_t *exceptset,
                   struct timeval *timeout)
{
     fd_set *readfds   = (readset != NULL ? &(readset->set) : NULL);
     fd_set *writefds  = (writeset != NULL ? &(writeset->set) : NULL);
     fd_set *exceptfds = (exceptset != NULL ? &(exceptset->set) : NULL);

     return select (fdmax, readfds, writefds, exceptfds, timeout);
}

void socket_set_zero (struct socket_set_t *sset)
{
     int i = 0;

     for (i = 0; i < FD_SETSIZE; ++i)
     {
          sset->s[i] = NULL;
     }

     FD_ZERO (&(sset->set));

     sset->fdmax = 0;
}

void socket_set_add (struct socket_set_t *sset, struct socket_t *s)
{
     if (s != NULL)
     {
          sset->s[s->fd] = s;
          FD_SET (s->fd, &(sset->set));
     }
}

void socket_set_remove (struct socket_set_t *sset, struct socket_t *s)
{
     if (s != NULL)
     {
          sset->s[s->fd] = NULL;
          FD_CLR (s->fd, &(sset->set));
     }
}

bool socket_set_isset (struct socket_set_t *sset, struct socket_t *s)
{
     if (s == NULL)
     {
          return false;
     }

     return FD_ISSET (s->fd, &(sset->set));
}

void socket_set_foreach (struct socket_set_t *sset, socket_set_foreach_fn caller, void *data)
{
     if (caller != NULL)
     {
          int i = 0;

          for (i = 0; i < FD_SETSIZE; ++i)
          {
               if (sset->s[i] != NULL)
               {
                    caller (sset, sset->s[i], data);
               }
          }
     }
}
