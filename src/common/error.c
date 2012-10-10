#include <fwallsh/common/common.h>

void error_free (struct error_t *error)
{
     if (error != NULL)
     {
          deallocate (error->message);
     }
}
