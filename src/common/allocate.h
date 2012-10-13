#ifndef __ALLOCATE_H
#define __ALLOCATE_H

#include <fwallsh/common/types.h>

/*!
 * Allocate memory.
 * If the allocation failed, the program exit
 * with code EXIT_FAILURE. It never returns a
 * NULL pointer. After the allocation, the memory
 * is initialized to 0.
 *
 * This function store the size before the returned
 * pointer, so the real allocated memory is :
 *
 *   sz + sizeof (size_t)
 *
 * \param sz Size (in bytes) of memory to allocate.
 * \return A pointer to the newly allocated memory.
 */

void *allocate (size_t sz);

/*!
 * Free allocated memory.
 * If the ptr is NULL, nothing is done.
 *
 * This function overwrite the memory before releasing
 * it. The pointer has to be allocated with allocate().
 *
 * \param ptr Pointer to the memory to free.
 */

void deallocate (void *ptr);

/*!
 * Reallocate already allocated memory.
 * If ptr is NULL, it has the same effect as
 * allocate(). If sz is 0, it has the same effect
 * as deallocate(). If sz is greater than the current
 * allocated memory, then the new space isn't
 * initialized.
 *
 * \param ptr Pointer to allocated memory.
 * \param sz New size of the allocated memory.
 * \return A pointer to the newly allocated memory or NULL if sz was 0.
 */

void *reallocate (void *ptr, size_t sz);

#endif /* __ALLOCATE_H */
