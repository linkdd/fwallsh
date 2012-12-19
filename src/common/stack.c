#include <fwallsh/common/common.h>

struct stack_t *stack_push (struct stack_t *top, enum type_t type, ...)
{
     struct stack_t *el = NULL;

     if (top != NULL)
     {
          va_list args;

          /* allocate the new element */
          el       = allocate (sizeof (struct stack_t));
          el->prev = top;

          va_start (args, type);
          value_set ((struct value_t *) el, type, args);
          va_end (args);
     }

     return el;
}

struct stack_t *stack_pop (struct stack_t *top, ...)
{
     struct stack_t *new_top = NULL;

     if (top != NULL)
     {
          va_list args;

          /* pop remove the top element, so define the new top
           * to the previous element.
           */
          new_top = top->prev;

          va_start (args, top);
          value_get ((struct value_t *) top, args);
          va_end (args);

          /* now free memory */

          if (top->val.type == TYPE_STRING)
          {
               deallocate (top->val.data.s);
          }

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

          va_start (args, idx);
          value_get ((struct value_t *) tmp, args);
          va_end (args);
     }

     return top;
}

size_t stack_length (struct stack_t *top)
{
     struct stack_t *tmp = NULL;
     size_t len          = 0;

     for (tmp = top; tmp != NULL; tmp = tmp->prev)
     {
          ++len;
     }

     return len;
}
