#include <fwallsh/common/common.h>

static char const * const CONFIG_SQL_CREATE_TABLE = "  \
     CREATE TABLE IF NOT EXISTS config (               \
          id INTEGER PRIMARY KEY AUTOINCREMENT,        \
          key VARCHAR NOT NULL UNIQUE,                 \
          value VARCHAR,                               \
          type INTEGER NOT NULL                        \
     );                                                \
";

static char const * const CONFIG_SQL_SELECT = "        \
     SELECT key, value, type FROM config;              \
";

static char const * const CONFIG_SQL_UPDATE = "        \
     REPLACE INTO config (key, value, type)            \
     VALUES (                                          \
          '%s',                                        \
          '%s',                                        \
          %d                                           \
     );                                                \
";

/*!
 * SQL Callback for the SELECT statement.
 *
 * \param data Configuration object.
 * \param argc Number of columns.
 * \param argv Values.
 * \param column Column names.
 * \return Always 0.
 */
static int _config_get_key_value (void *data, int argc, char **argv, char **column)
{
     struct config_t *conf = (struct config_t *) data;

     char *key = argv[0];
     char *val = argv[1];

     int type  = (int) strtol (argv[2], NULL, 10);

     switch (type)
     {
          case 0:
               conf->hashv = hashtable_update (conf->hashv, (void *) key, strlen (key), TYPE_INT, (int) strtol (val, NULL, 10));
               break;

          case 1:
               conf->hashv = hashtable_update (conf->hashv, (void *) key, strlen (key), TYPE_DOUBLE, (double) strtod (val, NULL));
               break;

          case 2:
               if (string_equal (val, "true"))
               {
                    conf->hashv = hashtable_update (conf->hashv, (void *) key, strlen (key), TYPE_BOOLEAN, true);
               }
               else
               {
                    conf->hashv = hashtable_update (conf->hashv, (void *) key, strlen (key), TYPE_BOOLEAN, false);
               }

               break;

          case 3:
               conf->hashv = hashtable_update (conf->hashv, (void *) key, strlen (key), TYPE_STRING, val);
               break;

          default:
               break;
     }

     return 0;
}

/*!
 * Callback for the foreach statement.
 *
 * \param element Current element in the hash table.
 * \param data Configuration object.
 * \return Always true.
 */
static bool _config_set_key_value (struct hashtable_t *element, void *data)
{
     struct config_t *conf = (struct config_t *) data;

     char *sql = NULL;
     char *key = NULL;
     char *val = NULL;
     int type  = TYPE_NULL;


     /* retrieve key/value pair */

     key = allocate (element->size);
     memcpy (key, element->key, element->size);

     type = element->val.type;

     switch (type)
     {
          case TYPE_INT:
               val = string_duplicate ("%d", element->val.data.i);
               break;

          case TYPE_DOUBLE:
               val = string_duplicate ("%lf", (double) element->val.data.f);
               break;

          case TYPE_BOOLEAN:
               val = string_duplicate ("%s", element->val.data.b ? "true" : "false");
               break;

          case TYPE_STRING:
               val = string_duplicate ("%s", element->val.data.s);
               break;

          default:
               break;
     }

     /* create SQL statement */
     sql = string_duplicate (CONFIG_SQL_UPDATE, key, val, type);

     deallocate (key);
     deallocate (val);

     sqlite3_exec (conf->db, sql, NULL, NULL, NULL); /* if this fail, what can we do ? */

     deallocate (sql);

     return true;
}

struct config_t *config_load (char const * const confpath, struct error_t *error)
{
     struct config_t *conf = allocate (sizeof (struct config_t));

     char *errmsg = NULL;

     conf->path = string_duplicate ("%s", confpath);

     /* open database */

     if (sqlite3_open (conf->path, &(conf->db)) != SQLITE_OK)
     {
          error_set (error, "config:%s: Can't open database: %s", conf->path, sqlite3_errmsg (conf->db));

          deallocate (conf->path);
          deallocate (conf);
          return NULL;
     }

     /* create table if needed */
     if (sqlite3_exec (conf->db, CONFIG_SQL_CREATE_TABLE, NULL, NULL, &errmsg) != SQLITE_OK)
     {
          error_set (error, "config:%s: Can't create table: %s", conf->path, errmsg);
          sqlite3_free (errmsg);

          sqlite3_close (conf->db);
          deallocate (conf->path);
          deallocate (conf);
          return NULL;
     }

     /* now, retrieve all key/value pairs */
     if (sqlite3_exec (conf->db, CONFIG_SQL_SELECT, _config_get_key_value, conf, &errmsg) != SQLITE_OK)
     {
          error_set (error, "config:%s: Can't select key/value paris: %s", conf->path, errmsg);
          sqlite3_free (errmsg);

          sqlite3_close (conf->db);
          deallocate (conf->path);
          deallocate (conf);
          return NULL;
     }

     return conf;
}

void config_save (struct config_t *conf, struct error_t *error)
{
     /* for each element in the hash table, call _config_set_key_value() */
     hashtable_foreach (conf->hashv, _config_set_key_value, conf);
}
