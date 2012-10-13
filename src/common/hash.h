#ifndef __HASH_H
#define __HASH_H

#include <fwallsh/common/types.h>

/*!
 * HashTable element.
 * \inherits struct value_t
 */
struct hashtable_t
{
     struct value_t val;           /*!< Generic value. */

     size_t hash;                  /*!< Hashed key. */

     void *key;                    /*!< Unhashed key. */
     size_t size;                  /*!< Unashed key's size. */

     struct hashtable_t *head;     /*!< Head of the hashtable. */
     struct hashtable_t *prev;     /*!< Previous element. */
     struct hashtable_t *next;     /*!< Next element. */
};

/*!
 * Insert or Update a key/value pair in the HashTable.
 * If hashv is NULL, then the hashtable is created.
 *
 * \param hashv HashTable, or NULL.
 * \param key Key associated to the value (used to generate hashed identifier).
 * \param size Size of key.
 * \param type Value's type.
 * \param ... Value.
 * \return HashTable's head.
 */
struct hashtable_t *hashtable_update (struct hashtable_t *hashv, void * const key, size_t size, enum type_t type, ...);

/*!
 * Get value associated to a key.
 * If hashv is NULL, nothing is done.
 *
 * \param hashv HashTable, or NULL.
 * \param key Key associated to the value.
 * \param size Size of key.
 * \param ... Pointer to storage location for the value.
 * \return HashTable's head or NULL if hashv is NULL or if the key wasn't found.
 */
struct hashtable_t *hashtable_get (struct hashtable_t *hashv, void * const key, size_t size, ...);

/*!
 * Remove a value in the HashTable.
 * If hashv is NULL, nothing is done.
 *
 * \param hashv HashTable, or NULL.
 * \param key Key associated to the value.
 * \param size Size of key.
 * \return HashTable's head or NULL if the HashTable is empty after deletion or if hashv is NULL.
 */
struct hashtable_t *hashtable_remove (struct hashtable_t *hashv, void *const key, size_t size);

/*!
 * Traverse all the HashTable.
 * If hashv is NULL, nothing is done.
 *
 * \param hashv HashTable, or NULL.
 * \param callback Callback function called for each node, if it returns false, break the loop.
 * \param data Data to pass to the callback.
 */
void hashtable_foreach (struct hashtable_t *hashv, bool (*callback) (struct hashtable_t *, void *), void *data);

#endif /* __HASH_H */
