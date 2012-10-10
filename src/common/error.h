#ifndef __ERROR_H
#define __ERROR_H

#include <fwallsh/common/types.h>

/*! Error type */
struct error_t
{
     char *message; /*!< Error's message */
};

/*!
 * Free memory used by an error type.
 *
 * \param error Pointer to the error to free.
 */
void error_free (struct error_t *error);

#endif /* __ERROR_H */
