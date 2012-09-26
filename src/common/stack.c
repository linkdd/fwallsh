#include <fwallsh/common/common.h>

struct stack_t *stack_push (struct stack_t *top, enum type_t type, ...)
{
     struct stack_t *el = NULL;

     if (top != NULL)
     {
          va_list args;

          /* allocate the new element */
          el = allocate (sizeof (struct stack_t));
          el->prev = top;

          el->val.type = type;

          /* build the generic value */
          va_start (args, type);

          switch (type)
          {
               case TYPE_INT:
                    el->val.data.i = va_arg (args, int);
                    break;

               case TYPE_DOUBLE:
                    el->val.data.f = va_arg (args, double);
                    break;

               case TYPE_BOOLEAN:
                    el->val.data.b = va_arg (args, int);
                    break;

               case TYPE_STRING:
                    el->val.data.s = string_duplicate ("%s", va_arg (args, char *));
                    break;

               case TYPE_POINTER:
                    el->val.data.p = va_arg (args, void *);
                    break;
          }
     }

     return el;
}

struct stack_t *stack_pop (struct stack_t *top, ...)
{
     struct stack_t *new_top = NULL;

     if (top != NULL)
     {
          va_list args;
          union
          {
               int *i;
               double *f;
               bool *b;
               char **s;
               void **p;
          } el;

          /* pop remove the top element, so define the new top
           * to the previous element.
           */
          new_top = top->prev;

          va_start (args, top);

          /* decompile generic value and put data into the pointer's
           * location.
           */

          switch (top->val.type)
          {
               case TYPE_INT:
                    el.i = va_arg (args, int *);
                    if (el.i != NULL)
                    {
                         *(el.i) = top->val.data.i;
                    }

                    break;

               case TYPE_DOUBLE:
                    el.f = va_arg (args, double *);

                    if (el.f != NULL)
                    {
                         *(el.f) = top->val.data.f;
                    }

                    break;

               case TYPE_BOOLEAN:
                    el.b = va_arg (args, bool *);

                    if (el.b != NULL)
                    {
                         *(el.b) = top->val.data.b;
                    }

                    break;

               case TYPE_STRING:
                    el.s = va_arg (args, char **);

                    if (el.s != NULL)
                    {
                         *(el.s) = string_duplicate ("%s", top->val.data.s);
                         deallocate (top->val.data.s);
                    }

                    break;

               case TYPE_POINTER:
                    el.p = va_arg (args, void **);

                    if (el.p != NULL)
                    {
                         *(el.p) = top->val.data.p;
                    }

                    break;
          }

          va_end (args);

          /* now free memory */

          deallocate (top);
     }

     return new_top;
}

struct stack_t *stack_peek (struct stack_t *top, size_t idx, ...)
{
     struct stack_t *tmp = top;
     size_t i = 0;

     /* go to the selected element */
     for (i = 0; i < idx && tmp != NULL; tmp = tmp->prev);

     if (tmp != NULL)
     {
          va_list args;
          union
          {
               int *i;
               double *f;
               bool *b;
               char **s;
               void **p;
          } el;

          va_start (args, idx);

          /* decompile generic value */

          switch (tmp->val.type)
          {
               case TYPE_INT:
                    el.i = va_arg (args, int *);

                    if (el.i != NULL)
                    {
                         *(el.i) = tmp->val.data.i;
                    }

                    break;

               case TYPE_DOUBLE:
                    el.f = va_arg (args, double *);

                    if (el.f != NULL)
                    {
                         *(el.f) = tmp->val.data.f;
                    }

                    break;

               case TYPE_BOOLEAN:
                    el.b = va_arg (args, bool *);

                    if (el.b != NULL)
                    {
                         *(el.b) = tmp->val.data.b;
                    }

                    break;

               case TYPE_STRING:
                    el.s = va_arg (args, char **);

                    if (el.s != NULL)
                    {
                         *(el.s) = string_duplicate ("%s", tmp->val.data.s);
                    }

                    break;

               case TYPE_POINTER:
                    el.p = va_arg (args, void **);

                    if (el.p != NULL)
                    {
                         *(el.p) = tmp->val.data.p;
                    }

                    break;
          }

          va_end (args);
     }

     return top;
}

size_t stack_length (struct stack_t *top)
{
     struct stack_t *tmp = NULL;
     size_t len = 0;

     for (tmp = top; tmp != NULL; tmp = tmp->prev)
     {
          ++len;
     }

     return len;
}
