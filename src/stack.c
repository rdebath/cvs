/*
 * Copyright (c) 2004, Free Software Foundation,
 *                     Derek Price,
 *                     & Ximbiot <http://ximbiot.com>.
 * 
 * You may distribute under the terms of the GNU General Public License as
 * specified in the README file that comes with the CVS source distribution.
 *
 * This module uses the hash.c module to implement a stack.
 */

#include "cvs.h"
#include <assert.h>



static void
_push (List *stack, void *elem, int isstring)
{
    Node *p = getnode();

    if (isstring)
	p->key = elem;
    else
	p->data = elem;

    addnode(stack, p);
}



void
push (List *stack, void *elem)
{
    _push (stack, elem, 0);
}



void
push_string (List *stack, char *elem)
{
    _push (stack, elem, 1);
}



void *
_pop (List *stack, int isstring)
{
    void *elem;

    if (isempty (stack)) return NULL;

    if (isstring)
    {
	elem = stack->list->prev->key;
	stack->list->prev->key = NULL;
    }
    else
    {
	elem = stack->list->prev->data;
	stack->list->prev->data = NULL;
    }

    delnode (stack->list->prev);
    return elem;
}



void *
pop (List *stack)
{
    return _pop (stack, 0);
}



char *
pop_string (List *stack)
{
    return _pop (stack, 1);
}



static void
_unshift (List *stack, void *elem, int isstring)
{
    Node *p = getnode();

    if (isstring)
	p->key = elem;
    else
	p->data = elem;

    addnode_at_front(stack, p);
}



void
unshift (List *stack, void *elem)
{
    _unshift (stack, elem, 0);
}



void
unshift_string (List *stack, char *elem)
{
    _unshift (stack, elem, 1);
}



static void *
_shift (List *stack, int isstring)
{
    void *elem;

    if (isempty (stack)) return NULL;

    if (isstring)
    {
	elem = stack->list->next->key;
	stack->list->next->key = NULL;
    }
    else
    {
	elem = stack->list->next->data;
	stack->list->next->data = NULL;
    }
    delnode (stack->list->next);
    return elem;
}



void *
shift (List *stack)
{
    return _shift (stack, 0);
}



char *
shift_string (List *stack)
{
    return _shift (stack, 1);
}



int
isempty (List *stack)
{
    if (stack->list == stack->list->next)
	return 1;
    return 0;
}
