#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <llist.h>


typedef struct node node_t;
struct llist
{
    node_t *first;
    node_t *last;
    int length;
    void (*free_fun)(void *);
};

struct node
{
    void *data;
    node_t *next;
};

struct llist_iter {
    node_t *node;
};


// PRIVATE DECLARATIONS

static node_t *node_new(void *data);
static bool node_insert(node_t **node_ptr, void *data);
static void *node_remove(node_t **node_ptr, void (*free_fun)(void *));
static void *iter_new(llist_t *list);


// PUBLIC FUNCTIONS

llist_t *llist_new(void (*free_fun)(void *))
{
    llist_t *new_list = malloc(sizeof *new_list);

    if (new_list != NULL)
    {
	new_list->first = NULL;
	new_list->last = NULL;
	new_list->length = 0;
	new_list->free_fun = free_fun;
    }

    return new_list;
}

bool llist_empty(llist_t *list)
{
    assert(list != NULL && "list is NULL");

    return list->length == 0;
}

int llist_length(llist_t *list)
{
    assert(list != NULL && "list is NULL");
    
    return list->length;
}

void *llist_first(llist_t *list)
{
    assert(list != NULL && "list is null");

    return list->first != NULL ? list->first->data : NULL;
}

void *llist_last(llist_t *list)
{
    assert(list != NULL && "list is null");

    return list->last != NULL ? list->last->data : NULL;
}

bool llist_prepend(llist_t *list, void *data)
{
    assert(list != NULL && "list is null");

    node_t *new_node = node_new(data);

    if (new_node == NULL)
	return false;

    if (list->length == 0)
	list->last = new_node;
    else
	new_node->next = list->first;
    
    list->first = new_node;
    list->length++;
    
    return true;
}

bool llist_append(llist_t *list, void *data)
{
    assert(list != NULL && "list is null");

    node_t *new_node = node_new(data);

    if (new_node == NULL)
	return false;

    if (list->length == 0)
	list->first = new_node;
    else
	list->last->next = new_node;

    list->last = new_node;
    list->length++;
    
    return true;
}

bool llist_insert(llist_t *list, void *data, int index)
{
    assert(list != NULL && "list is null");
    assert(index >= 0 && index <= list->length && "index is out of bounds");

    if (index == 0)
	return llist_prepend(list, data);
    
    if (index == list->length)
	return llist_append(list, data);

    node_t **iterator = &(list->first);

    for (int i = 0; i < index; i++)
	iterator = &((*iterator)->next);
    
    bool success = node_insert(iterator, data);

    if (success)
	list->length++;

    return success;
}

void *llist_remove(llist_t *list, int index)
{
    assert(list != NULL && "list is null");
    assert(index >= 0 && index < list->length && "index is out of bounds");

    node_t *iterator = list->first;
    node_t *previous = NULL;

    for (int i = 0; i < index; i++)
    {
	previous = iterator;
	iterator = iterator->next;
    }
    
    void *d = node_remove(previous ? &(previous->next) : &(list->first), list->free_fun);

    if (index == --list->length)
	list->last = previous;

    return d;
}

int llist_index_of(llist_t *list, bool (*eq_fun)(const void *, void *), void *cmp_val)
{
    assert(list != NULL && "list is null");
    assert(eq_fun != NULL && "equals function is null");

    node_t *iterator = list->first;

    for (int i = 0; i < list->length; i++, iterator = iterator->next)
	if (eq_fun(iterator->data, cmp_val))
	    return i;
    
    return -1;
}

void *llist_get(llist_t *list, int index)
{
    assert(list != NULL && "list is null");
    assert(index >= 0 && index < list->length && "index is out of bounds");

    node_t *iterator = list->first;
    
    for (int i = 0; i < index; i++)
	iterator = iterator->next;

    return iterator->data;
}

void *llist_search(llist_t *list, bool (*eq_fun)(const void *, void *), void *cmp_val)
{
    assert(list != NULL && "list is null");
    assert(eq_fun != NULL && "equals function is null");

    for (node_t *iterator = list->first; iterator != NULL; iterator = iterator->next)
	if (eq_fun(iterator->data, cmp_val))
	    return iterator->data;

    return NULL;
}

llist_iter_t *llist_get_iter(llist_t *list)
{
    assert(list != NULL && "list is null");

    return iter_new(list);
}

void *llist_iter_next(llist_iter_t *iter)
{
    assert(iter != NULL && "iterator is null");

    if (iter->node != NULL)
    {
	void *data = iter->node->data;
	iter->node = iter->node->next;
	return data;
    }

    return NULL;
}

void llist_iter_free(llist_iter_t *iter)
{
    assert(iter != NULL && "iterator is null");

    free(iter);
}

void llist_free(llist_t *list)
{
    assert(list != NULL && "list is null");

    node_t *iterator = list->first;

    while (iterator != NULL)
    {
	node_t *n = iterator;
	iterator = iterator->next;
	
	if (list->free_fun != NULL)
	    list->free_fun(n->data);

	free(n);
    }

    free(list);
}


// PRIVATE FUNCTIONS

static node_t *node_new(void *data)
{
    node_t *new_node = malloc(sizeof *new_node);

    if (new_node != NULL)
    {
	new_node->data = data;
	new_node->next = NULL;
    }

    return new_node;
}

static bool node_insert(node_t **node_ptr, void *data)
{
    node_t *new_node = node_new(data);

    if (new_node != NULL)
    {
	new_node->next = *node_ptr;
	*node_ptr = new_node;
    }

    return new_node != NULL;
}

static void *node_remove(node_t **node_ptr, void (*free_fun)(void *))
{
    void *data = NULL;
    node_t *n = *node_ptr;
    *node_ptr = (*node_ptr)->next;

    if (free_fun != NULL)
	free_fun(n->data);
    else
	data = n->data;

    free(n);

    return data;
}

static void *iter_new(llist_t *list)
{
    llist_iter_t *iter = malloc(sizeof *iter);

    if (iter != NULL)
    {
	iter->node = list->first;
    }

    return iter;
}
