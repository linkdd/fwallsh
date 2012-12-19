#ifndef __MODULE_H
#define __MODULE_H

#include <dlfcn.h>

#include <fwallsh/common/stack.h>

/*! Defines module API */
struct api_t
{
     char const * const pattern;         /*!< Command in shell */
     void (*handler) (struct stack_t *); /*!< Associated handler */
};

/*!
 * Load the module API.
 *
 * \param name Module's name.
 * \return Array of handlers or NULL if we can't open the module or if we can't get its API.
 */
struct api_t *module_load_api (char const * const name);

#endif /* __MODULE_H */
