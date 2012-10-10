#include <fwallsh/common/common.h>

static size_t _hash (void * const data, size_t size)
{
     char const * const ptr = data;
     size_t hash = 5381;

     size_t i = 0;


     for (i = 0; i < size; ++i)
     {
          hash = ((hash << 5) + hash) + (size_t) (ptr[i]);
     }

     return hash;
}

struct hashtable_t *hashtable_update (struct hashtable_t *hashv, void * const key, size_t size, enum type_t type, ...)
{
     struct hashtable_t *tmp = NULL;

     size_t hash = _hash (key, size);
     va_list args;

     /* if hashv is NULL, create a new one */
     if (hashv == NULL)
     {
          tmp = allocate (sizeof (struct hashtable_t));

          tmp->head = tmp;
          tmp->prev = tmp;
          tmp->next = tmp;

          va_start (args, type);
          value_set ((struct value_t *) tmp, type, args);
          va_end (args);

          /* copy key */
          tmp->key  = allocate (size);
          tmp->size = size;
          memcpy (tmp->key, key, size);

          return tmp;
     }

     /* search if the key is already in the table */

     tmp = hashv->head;

     do
     {
          if (tmp->hash == hash)
          {
               va_start (args, type);
               value_set ((struct value_t *) tmp, type, args);
               va_end (args);

               /* copy key */
               tmp->key  = allocate (size);
               tmp->size = size;
               memcpy (tmp->key, key, size);

               tmp = NULL;

               break;
          }
     } while (tmp != hashv->head);

     /* if tmp is NULL, then the key was updated */
     if (tmp != NULL)
     {
          tmp = allocate (sizeof (struct hashtable_t));

          tmp->head = hashv->head;
          tmp->prev = hashv->prev;
          tmp->next = hashv->head;

          hashv->head->prev->next = tmp;
          hashv->head->prev       = tmp;

          va_start (args, type);
          value_set ((struct value_t *) tmp, type, args);
          va_end (args);

          /* copy key */
          tmp->key  = allocate (size);
          tmp->size = size;
          memcpy (tmp->key, key, size);

     }

     return hashv->head;
}

struct hashtable_t *hashtable_get (struct hashtable_t *hashv, void * const key, size_t size, ...)
{
     struct hashtable_t *tmp = NULL;

     size_t hash = _hash (key, size);

     if (hashv == NULL)
     {
          return NULL;
     }

     tmp = hashv->head;

     do
     {
          if (tmp->hash == hash)
          {
               va_list args;

               va_start (args, size);
               value_get ((struct value_t *) tmp, args);
               va_end (args);

               break;
          }

          tmp = tmp->next;
     } while (tmp != hashv->head);

     return hashv->head;
}

struct hashtable_t *hashtable_remove (struct hashtable_t *hashv, void * const key, size_t size)
{
     struct hashtable_t *tmp = NULL;

     size_t hash = _hash (key, size);

     if (hashv == NULL)
     {
          return NULL;
     }

     tmp = hashv->head;

     do
     {
          if (tmp->hash == hash)
          {
               /* free memory used */
               if (tmp->val.type == TYPE_STRING)
               {
                    deallocate (tmp->val.data.s);
               }

               deallocate (tmp->key);

               /* if this is the last entry in the hashtable */
               if (tmp->head == tmp)
               {
                    deallocate (tmp);
                    return NULL;
               }

               tmp->next->prev = tmp->prev;
               tmp->prev->next = tmp->next;
               deallocate (tmp);

               break;
          }
     } while (tmp != hashv->head);

     return hashv->head;
}

void hashtable_foreach (struct hashtable_t *hashv, bool (*callback) (struct hashtable_t *, void *), void *data)
{
     if (hashv != NULL)
     {
          struct hashtable_t *tmp = hashv->head;

          do
          {
               if (!callback (tmp, data))
               {
                    break;
               }

               tmp = tmp->next;
          } while (tmp != hashv->head);
     }
}
