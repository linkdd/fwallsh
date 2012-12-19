#include <fwallsh/daemon/local.h>

struct global_t global;

/*! main loop of the program. */
static int _main_loop (void)
{
     struct socket_set_t readset;

     if (!initialize_socket ())
     {
          return EXIT_FAILURE;
     }

     socket_set_zero (&readset);

     /* wait for clients */
     while (true)
     {
          readset = global.master;

          /* monitor many sockets */
          if (socket_select (global.fdmax + 1, &readset, NULL, NULL, NULL) < 0)
          {
               syslog (LOG_ERR, "socket select: %s", strerror (errno));
               continue;
          }

          /* loop on each socket to check which one is set */
          socket_set_foreach (&readset, handle_socket, NULL);
     }

     deallocate (global.listener);

     return EXIT_SUCCESS;
}

int main (int argc, char **argv)
{
     int retcode = EXIT_SUCCESS;
     int c = 0;

     char *pidfile  = NULL;
     bool daemonize = false;

     /* parse command line arguments */
     while ((c = getopt (argc, argv, "hvp:d")) != -1)
     {
          switch (c)
          {
               case 'h':
                    fprintf (stderr, "Usage: %s [-h] [-v] [-p pidfile] [-d]\n", PACKAGE);
                    break;

               case 'v':
                    fprintf (stderr, "%s, by :\n", PACKAGE_STRING);
                    break;

               case 'p':
                    pidfile = string_duplicate ("%s", optarg);
                    break;

               case 'd':
                    daemonize = true;
                    break;

               case '?':
                    fprintf (stderr, "Usage: %s [-h] [-v] [-p pidfile] [-d]\n", PACKAGE);
                    exit (EXIT_FAILURE);
          }
     }

     if (daemonize)
     {
          int ret = 0;

          /* if the pidfile is NULL, set a default value */
          if (pidfile == NULL)
          {
               pidfile = string_duplicate ("/var/run/%s.pid", PACKAGE);
          }

          /* daemonize the process */
          ret = fwallsh_daemonize ();

          if (ret == -1)
          {
               deallocate (pidfile);
               exit (EXIT_FAILURE);
          }
          /* if we are in the daemonized process */
          else if (ret == 0)
          {
               /* register pidfile */

               pid_t pid = getpid ();
               FILE *f   = fopen (pidfile, "w");

               if (f != NULL)
               {
                    fprintf (f, "%d", pid);
                    fclose (f);
               }

               /* run main loop */

               retcode = _main_loop ();

               /* remove pidfile */

               unlink (pidfile);
               deallocate (pidfile);
          }
     }
     else
     {
          /* we don't daemonize the current process, so we don't need pidfile */
          if (pidfile != NULL)
          {
               deallocate (pidfile);
          }

          /* Open log */

          openlog ("fwallsh:", LOG_CONS | LOG_PID, LOG_DAEMON);

          /* run main loop */

          retcode = _main_loop ();
     }

     return retcode;
}
