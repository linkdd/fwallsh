#include <fwallsh/shell/local.h>

/*!
 * \brief Initialize socket.
 * It creates and connect to the UNIX socket.
 *
 * \param s A socket structure.
 * \return true on success, false on error.
 */
bool init_socket (struct socket_t *s)
{
     socket_init (s);

     /* create the socket */
     if (!s->create (s, "/tmp/fwallsh.sock"))
     {
          perror ("fwallsh: init[socket-create]");
          return false;
     }

     /* connect to the socket */
     if (!s->connect (s))
     {
          perror ("fwallsh: init[socket-connect]");
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
 * \param s A socket structure.
 */
void main_loop (struct socket_t *s)
{
     char buffer[BUFSIZ] = {0};
     bool done = false;

     while (!done)
     {
          char *line = prompt ("> ");
          ssize_t n = 0;

          /* send command */
          if ((n = s->write (s, line, strlen (line), 0)) < 0)
          {
               perror ("fwallsh: main[socket-write]");
          }

          /* receive answer */
          if (n > 0 && (n = s->read (s, buffer, BUFSIZ, 0)) < 0)
          {
               perror ("fwallsh: main[socket-read]");
          }

          /* print answer */
          if (n > 0)
          {
               buffer[n] = 0;
               printf ("%s", buffer);
          }
     }

     s->shutdown (s);
}

int main (int argc, char **argv)
{
     struct socket_t *s = allocate (sizeof (struct socket_t));

     /* initialize the socket */
     if (!init_socket (s))
     {
          deallocate (s);
          exit (EXIT_FAILURE);
     }

     /* run main loop */
     main_loop (s);

     deallocate (s);

     return EXIT_SUCCESS;
}
