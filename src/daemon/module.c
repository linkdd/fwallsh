#include <fwallsh/common/common.h>
#include <fwallsh/daemon/module.h>

struct api_t *module_load_api (char const * const name)
{
     struct api_t *api = NULL;
     void *handler     = NULL;
     char *tmp         = NULL;

     /* open module */

     handler = dlopen (name, RTLD_GLOBAL); /* TODO: search path */

     if (handler == NULL)
     {
          warnx ("Can't open module '%s': %s", name, dlerror ());

          return NULL;
     }

     /* load API */

     tmp = string_concatenate (name, "_api");

     api = dlsym (handler, tmp);

     deallocate (tmp);

     if ((tmp = dlerror ()) != NULL)
     {
          warnx ("Can't read API of module '%s': %s", name, tmp);

          return NULL;
     }

     /* close handler */

     dlclose (handler);

     return api;
}
