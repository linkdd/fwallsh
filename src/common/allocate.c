#include <fwallsh/common/common.h>

/*!
 * Reimplementation of memset() without compiler optimizations.
 *
 * \param v Pointer to the memory to set.
 * \param c New value to set.
 * \param n Number of bytes to set.
 * \return Pointer to the memory.
 */
static void *_guaranteed_memset (void *v, int c, size_t n)
{
     volatile char *p = v; /* we don't want any compiler optimizations */

     for (; n > 0; --n)
     {
          *p = c;
          p++;
     }

     return v;
}

void *allocate (size_t sz)
{
     void *ptr = calloc (1, sz + sizeof (size_t));

     if (ptr == NULL)
     {
          errx (EXIT_FAILURE, "Can't allocate %lu bytes.", (unsigned long) sz);
     }

     /* store allocated size */
     *((size_t *) ptr) = sz;

     /* and return the beginning of memory */
     return ptr + sizeof (size_t);
}

void deallocate (void *ptr)
{
     void *real_ptr = NULL;
     size_t sz      = 0;

     /* if ptr is NULL, does nothing */
     if (ptr == NULL)
     {
          return;
     }

     /* get back the real pointer */
     real_ptr = ptr - sizeof (size_t);
     sz       = *((size_t *) real_ptr);

     /* overwrite pointer so it's not garbage collected. */
     (void) _guaranteed_memset (real_ptr, 0, sz + sizeof (size_t));

     free (real_ptr);
}

void *reallocate (void *ptr, size_t sz)
{
     if (sz == 0)
     {
          deallocate (ptr);
          return NULL;
     }
     else if (ptr == NULL)
     {
          return allocate (sz);
     }
     else
     {
          void *nptr = realloc (ptr - sizeof (size_t), sz + sizeof (size_t));

          if (nptr == NULL)
          {
               errx (EXIT_FAILURE, "Can't allocate %lu bytes.", (unsigned long) sz);
          }

          /* update the pointer's size */
          *((size_t *) nptr) = sz;

          return nptr + sizeof (size_t);
     }
}
