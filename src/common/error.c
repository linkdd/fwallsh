#include <fwallsh/common/common.h>

void error_set (struct error_t *error, char const * const format, ...)
{
     if (error != NULL)
     {
          va_list args;

          va_start (args, format);
          error->message = string_duplicate_va (format, args);
          va_end (args);
     }
}

void error_free (struct error_t *error)
{
     if (error != NULL)
     {
          deallocate (error->message);
     }
}
