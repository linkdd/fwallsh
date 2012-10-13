#ifndef __TYPES_H
#define __TYPES_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>

#include <fwallsh/common/string-util.h>

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
     TYPE_POINTER,  /*!< should be a void* (or any other pointers) */
     TYPE_NULL      /*!< NULL type */
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

/*!
 * Unpack generic value.
 *
 * \param val Pointer to the value to unpack.
 * \param arg Arguments list which contains the pointer to the value destination.
 */
static inline void value_get (struct value_t *val, va_list arg)
{
     union
     {
          int *i;
          double *f;
          bool *b;
          char **s;
          void **p;
     } el;

     /* decompile generic value and put data into the pointer's
      * location.
      */

     switch (val->type)
     {
          case TYPE_INT:
               el.i = va_arg (arg, int *);

               if (el.i != NULL)
               {
                    *(el.i) = val->data.i;
               }

               break;

          case TYPE_DOUBLE:
               el.f = va_arg (arg, double *);

               if (el.f != NULL)
               {
                    *(el.f) = val->data.f;
               }

               break;

          case TYPE_BOOLEAN:
               el.b = va_arg (arg, bool *);

               if (el.b != NULL)
               {
                    *(el.b) = val->data.b;
               }

               break;

          case TYPE_STRING:
               el.s = va_arg (arg, char **);

               if (el.s != NULL)
               {
                    *(el.s) = string_duplicate ("%s", val->data.s);
               }

               break;

          case TYPE_POINTER:
               el.p = va_arg (arg, void **);

               if (el.p != NULL)
               {
                    *(el.p) = val->data.p;
               }

               break;

          default:
               break;
     }
}

/*!
 * Build generic value.
 *
 * \param val Pointer to the value to build.
 * \param type Value's type.
 * \param arg Argument list containing the value.
 */
static inline void value_set (struct value_t *val, enum type_t type, va_list arg)
{
     /* build generic value */

     val->type = type;

     switch (type)
     {
          case TYPE_INT:
               val->data.i = va_arg (arg, int);
               break;

          case TYPE_DOUBLE:
               val->data.f = va_arg (arg, double);
               break;

          case TYPE_BOOLEAN:
               val->data.b = va_arg (arg, int);
               break;

          case TYPE_STRING:
               val->data.s = string_duplicate ("%s", va_arg (arg, char *));
               break;

          case TYPE_POINTER:
               val->data.p = va_arg (arg, void *);
               break;

          default:
               break;
     }
}

#endif /* __TYPES_H */
