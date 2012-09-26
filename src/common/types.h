#ifndef __TYPES_H
#define __TYPES_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>

#ifndef va_copy
#    define va_copy(d,s)      __va_copy(d,s)
#endif

/*! C types identificators */
enum type_t
{
     TYPE_INT = 0,  /*!< should be a char, a short, an int or a long */
     TYPE_DOUBLE,   /*!< should be a double or a float */
     TYPE_BOOLEAN,  /*!< should be a bool, a char, or an int */
     TYPE_STRING,   /*!< should be a char* */
     TYPE_POINTER   /*!< should be a void* (or any other pointers) */
};

/*! Generic value */
struct value_t
{
     enum type_t type;   /*!< Value type */

     union
     {
          int i;
          double f;
          bool b;
          char *s;
          void *p;
     } data;             /*!< Value data */
};

#endif /* __TYPES_H */
