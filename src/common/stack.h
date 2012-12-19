#ifndef __STACK_H
#define __STACK_H

#include <fwallsh/common/types.h>

/*!
 * Stack element.
 * \inherits struct value_t
 */
struct stack_t
{
     struct value_t val;      /*!< Generic value. */

     struct stack_t *prev;    /*!< Pointer to the previous element in the stack. */
};

/*!
 * Push a new element onto the stack.
 * If top is NULL, then the stack is created.
 *
 * \param top Pointer to the top of the stack, or NULL if the stack is empty.
 * \param type Type of the element to push.
 * \param ... Value of the element to push.
 * \return The new top of the stack.
 */

struct stack_t *stack_push (struct stack_t *top, enum type_t type, ...);

/*!
 * Pop the top element of the stack.
 * If top is NULL, then nothing is popped.
 *
 * \param top Pointer to the top of the stack, or NULL if the stack is empty.
 * \param ... Pointer to store the top value, or NULL if you don't want to get it back.
 * \return The new top of the stack.
 */

struct stack_t *stack_pop (struct stack_t *top, ...);

/*!
 * Peek an element in the stack.
 * If top is NULL, then nothing is peeked.
 *
 * \param top Pointer to the top of the stack, or NULL if the stack is empty.
 * \param idx Peek in the nth element from top of the stack.
 * \param ... Pointer to store the peeked value, or NULL if you don't want to get it back.
 * \return The top of the stack.
 */

struct stack_t *stack_peek (struct stack_t *top, size_t idx, ...);

/*!
 * Get number of elements in the stack.
 *
 * \param top Pointer to the top of the stack, or NULL if the stack is empty.
 * \return Number of elements in the stack.
 */

size_t stack_length (struct stack_t *top);

#endif /* __STACK_H */
