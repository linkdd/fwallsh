#ifndef __CONFIG_H
#define __CONFIG_H

#include <fwallsh/common/types.h>
#include <fwallsh/common/error.h>
#include <fwallsh/common/hash.h>

#include <sqlite3.h>

/*! Configuration object */
struct config_t
{
     char *path;                   /*!< Path to the configuration. */
     sqlite3 *db;                  /*!< SQLite3 handler. */

     struct hashtable_t *hashv;    /*!< Hash Table. */
};

/*!
 * Read configuration from BerkeleyDB.
 *
 * \param confpath Path to the configuration.
 * \param error Pointer to errors location, or NULL.
 * \return A new configuration object, or NULL if something went wrong.
 */
struct config_t *config_load (char const * const confpath, struct error_t *error);

void config_save (struct config_t *conf, struct error_t *error);


#endif /* __CONFIG_H */
