#include <fwallsh/daemon/local.h>

int fwallsh_daemonize (void)
{
     pid_t pid, sid;

     /* Fork the process */
     pid = fork ();
     if (pid < 0)
     {
          return -1;
     }

     /* Exit parent process */
     if (pid > 0)
     {
          return 1;
     }

     /* Change the file mode mask */
     umask (0);

     /* Open log */

     openlog ("fwallsh:", LOG_CONS | LOG_PID, LOG_DAEMON);

     /* Create a new SID */

     sid = setsid ();
     if (sid < 0)
     {
          syslog (LOG_ERR, "%s", strerror (errno));
          return -1;
     }

     /* Change the current working directory */

     if ((chdir ("/")) < 0)
     {
          syslog (LOG_ERR, "%s", strerror (errno));
          return -1;
     }

     /* Close standard file descriptors */

     close (STDIN_FILENO);
     close (STDOUT_FILENO);
     close (STDERR_FILENO);

     return 0;
}

