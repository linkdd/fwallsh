#ifndef __ERROR_H
#define __ERROR_H

#include <fwallsh/common/types.h>

/*! Error type */
struct error_t
{
     char *message; /*!< Error's message */
};

/*!
 * Define error.
 *
 * \param error Pointer to the error to set.
 * \param format Format string (see string_duplicate()).
 * \param ... Arguments for the format string.
 */
void error_set (struct error_t *error, char const * const format, ...);

/*!
 * Free memory used by an error type.
 *
 * \param error Pointer to the error to free.
 */
void error_free (struct error_t *error);

#endif /* __ERROR_H */
