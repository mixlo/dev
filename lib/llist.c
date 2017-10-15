#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <llist.h>


typedef struct node node_t;
struct node
{
    void *data;
    struct node *next;
};

struct llist
{
    node_t *first;
    node_t *last;
    int length;
    void (*free_fun)(void *);
};


// PRIVATE DECLARATIONS

static node_t *node_new(void *data);
static bool node_insert(node_t **node_ptr, void *data);
static void *node_remove(node_t **node_ptr, void (*free_fun)(void *));


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
    return llist_insert(list, data, 0);
}

bool llist_append(llist_t *list, void *data)
{
    return llist_insert(list, data, list->length);
}

bool llist_insert(llist_t *list, void *data, int index)
{
    assert(list != NULL && "list is null");
    assert(index >= 0 && index <= list->length && "index is out of bounds");

    node_t **iterator = &(list->first);

    for (int i = 0; i < index; i++)
	iterator = &((*iterator)->next);
    
    bool success = node_insert(iterator, data);

    if (index == list->length++)
	list->last = *iterator;

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
