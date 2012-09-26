#include <fwallsh/common/common.h>

static int _vasprintf (char **ret, char const * const format, va_list args)
{
     va_list ap;

     char *str = NULL;
     char c    = 0;
     int sz    = 0;

     if (ret == NULL)
     {
          return -1;
     }

     va_copy (ap, args);

     /* vsnprintf() returns the number of bytes it should have read */

     sz = vsnprintf (&c, 1, format, args);

     /* allocate enough memory for the copy */

     str = allocate (sz + 1);

     /* copy string */

     sz = vsprintf (str, format, ap);
     *ret = str;

     return sz;
}

char *string_duplicate_va (char const * const format, va_list args)
{
     char *str = NULL;
     int ret   = 0;

     ret = _vasprintf (&str, format, args);

     if (ret < 0)
     {
          return NULL;
     }

     return str;
}

char *string_duplicate (char const * const format, ...)
{
     va_list args;
     char *str = NULL;

     va_start (args, format);
     str = string_duplicate_va (format, args);
     va_end (args);

     return str;
}

char *string_duplicate_n (size_t n, char const * const src)
{
     char *dst = NULL;

     if (src != NULL)
     {
          int i = 0, len = strlen (src);

          dst = allocate (n);

          for (i = 0; i < len && i < n; ++i)
          {
               dst[i] = src[i];
          }
     }

     return dst;
}

char *string_concatenate (char const * const str0, char const * const str1)
{
     return string_duplicate ("%s%s", str0, str1);
}

bool string_equal (char const * const str0, char const * const str1)
{
     return (strcmp (str0, str1) == 0);
}

char **string_parse_shell_argv (char const * const str, int *argc)
{
     char **argv = NULL;
     int nargs   = 0;

     int i     = 0;
     int len   = strlen (str);
     int first = 0;

     char quote = 0;
     bool word  = false;

     for (i = 0; i < len; ++i)
     {
          /* if we are not inside a quoted text */
          if (!quote)
          {
               /* check if the current character is a space */
               if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\r')
               {
                    /* if we are inside a word */
                    if (word)
                    {
                         /* allocate a new string in the array, and copy the word into it */
                         nargs++;
                         argv            = reallocate (argv, nargs * sizeof (char *));
                         argv[nargs - 1] = string_duplicate_n (i - first, &(str[first]));

                         /* we are not anymore inside a word */
                         word = false;
                    }

                    first = i + 1;
               }
               /* check if the current character is a quote */
               else if (str[i] == '\'' || str[i] == '"')
               {
                    /* then we are in a quoted text */
                    quote = str[i];
                    first = i + 1;
               }
               /* everything else ? we are in a word */
               else
               {
                    word = true;
               }
          }
          /* if we encountered the end of quoted text */
          else if (quote == str[i])
          {
               /* allocate a new string in the array, and copy the quoted text into it */
               nargs++;
               argv            = reallocate (argv, nargs * sizeof (char *));
               argv[nargs - 1] = string_duplicate_n (i - first, &(str[first]));

               quote = 0;
               first = i + 1;
          }
     }

     if (argc != NULL)
     {
          *argc = nargs;
     }

     return argv;
}
