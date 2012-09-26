#include <fwallsh/common/common.h>

void *allocate (size_t sz)
{
     void *ptr = calloc (1, sz);

     if (ptr == NULL)
     {
          errx (EXIT_FAILURE, "Can't allocate %u bytes.", sz);
     }

     return ptr;
}

void deallocate (void *ptr)
{
     free (ptr);
}

void *reallocate (void *ptr, size_t sz)
{
     void *nptr = realloc (ptr, sz);

     if (nptr == NULL)
     {
          errx (EXIT_FAILURE, "Can't allocate %u bytes.", sz);
     }

     return nptr;
}
