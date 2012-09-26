#ifndef __STRING_H
#define __STRING_H

#include <fwallsh/common/types.h>

#include <string.h>

/*!
 * Duplicate a formatted string.
 *
 * \param format Format string (see 'man 3 printf') to duplicate.
 * \param args List of arguments for the format string.
 * \return The duplicated formatted string.
 */

char *string_duplicate_va (char const * const format, va_list args);

/*!
 * Duplicate a formatted string.
 * This function get the arguments list from ... and just call
 * string_duplicate_va().
 *
 * \param format Format string (see 'man 3 printf') to duplicate.
 * \param ... Arguments for the format string.
 * \return The duplicated formatted string.
 */

char *string_duplicate (char const * const format, ...);

/*!
 * Duplicate n characters of a string.
 *
 * \param n Number of characters to copy.
 * \param src String to read characters from.
 * \return Newly duplicated string or NULL if src was NULL.
 */

char *string_duplicate_n (size_t n, char const * const src);

/*!
 * Concatenate two strings.
 *
 * \param str0 Initial string.
 * \param str1 String to append to str0.
 * \return Newly allocated string ocntaining str0 + str1.
 */

char *string_concatenate (char const * const str0, char const * const str1);

/*!
 * Compare two strings.
 * Just check if the return value of strcmp(str0, str1) is 0.
 *
 * \param str0 Initial string.
 * \param str1 String to compare with str0.
 * \return true if str0 == str1, false otherwise.
 */

bool string_equal (char const * const str0, char const * const str1);

/*!
 * Split a string into an array of string.
 * This function manage shell quotting.
 *
 * \param str String to parse.
 * \param argc Pointer to a storage location for the number of string in the returned array.
 * \return An array of string.
 */

char **string_parse_shell_argv (char const * const str, int *argc);

#endif /* __STRING_H */
