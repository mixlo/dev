#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <bst.h>


typedef struct node node_t;
struct node
{
    void *key;
    void *value;
    int balance;
    struct node *left;
    struct node *right;
};

// typedeffed to bst_t in "bst.h"
// lazy height
// height[0] = is_height_up_to_date
// height[1] = actual_height
struct bst
{
    node_t *root;
    TYPE type;
    int size;
    int height[2];
};


// PRIVATE DECLARATIONS

static int bst_height_aux(node_t *n);
static void bst_apply_aux(node_t *n, void (*f)(void *, void *));
static void bst_free_aux(node_t *n, void (*f)(void *));

static node_t *node_new(TYPE t, void *key, void *value);
static void balance(node_t **node_ptr, node_t **parent_ptr, node_t **grand_parent_ptr);
static void left_rotate(node_t **parent_ptr);
static void right_rotate(node_t **parent_ptr);

static int compare(TYPE t, void *v1, void *v2);
static void *duplicate(TYPE t, void *v);
static char *strdup(const char *s);
static int max(int a, int b);


// PUBLIC

int bst_size(bst_t *bst)
{
    if (bst == NULL)
	return -1;
    
    return bst->size;
}

int bst_height(bst_t *bst)
{
    if (bst == NULL)
	return -1;
    
    if (bst->height[0])
	return bst->height[1];

    int height = bst_height_aux(bst->root);
    
    bst->height[0] = 1;
    bst->height[1] = height;
    
    return height;
}

bst_t *bst_new(TYPE t)
{
    bst_t *new_bst = (bst_t *)malloc(sizeof(bst_t));

    if (new_bst != NULL)
    {
	new_bst->root = NULL;
	new_bst->type = t;
	new_bst->size = 0;
	new_bst->height[0] = 0;
	new_bst->height[1] = 0;
    }

    return new_bst;
}

int bst_insert_aux(node_t **np, TYPE t, void *k, void *v)
{
    if (*np == NULL)
    {
	node_t *new_node = node_new(t, k, v);

	if (new_node != NULL)
	{
	    *np = new_node;
	    //bst->size++;
	    //bst->height[0] = 0;
	    
	    balance(iterator, parent, grand_parent);
	}

	return new_node != NULL ? 1 : 0;
    }

    int c = compare(t, k, (*np)->key);
    assert(c != 0 && "keys must be unique");
    node_t **next = c < 0 ? &((*np)->left) : &((*np)->right);
    int pre_bal = *next != NULL ? (*next)->balance : 0;
    int result = bst_insert_aux(next, t, k, v);
    int post_bal = *next != NULL ? (*next)->balance : 0;

    // can't calculate balance difference based on left and right subtree
    // height since that would mean having to calculate height of one of them.
    // instead compare the previous balance value with the new one
    // find a good case for when the new node occurred
    if (result && pre_bal != post_bal)
    {
	
	(*np)->balance += c < 0 ? -1 : 1;
    }
    
    return result;
}

int bst_insert(bst_t *bst, void *key, void *value)
{
    assert(bst != NULL && "bst is null");
    assert(key != NULL && "key is null");

    node_t **iterator = &(bst->root);
    node_t **parent = NULL;
    node_t **grand_parent = NULL;
    
    while (*iterator != NULL)
    {
	int c = compare(bst->type, key, (*iterator)->key);
	
	assert(c != 0 && "keys must be unique");

	grand_parent = parent;
	parent = iterator;
	iterator = c < 0 ? &((*iterator)->left) : &((*iterator)->right);
    }

    node_t *new_node = node_new(bst->type, key, value);

    if (new_node != NULL)
    {
	*iterator = new_node;
	bst->size++;
	bst->height[0] = 0;
	
	balance(iterator, parent, grand_parent);
    }
    
    return new_node != NULL ? 1 : 0;
}

void bst_remove(bst_t *bst, void *key)
{
    // TBI
    // Re-balance
    // Decrease size
    // Reset height
    // Set node balances
}

void *bst_search(bst_t *bst, void *key)
{
    assert(bst != NULL && "bst is null");
    assert(key != NULL && "key is null");

    node_t *iterator = bst->root;

    while (iterator != NULL)
    {
	int c = compare(bst->type, key, iterator->key);
	
	if (c == 0)
	    return iterator->value;

	iterator = c < 0 ? iterator->left : iterator->right;
    }

    return NULL;
}

void bst_apply(bst_t *bst, void (*f)(void *, void *))
{
    assert(bst != NULL && "bst is null");
    assert(f != NULL && "function is null");

    bst_apply_aux(bst->root, f);
}

void bst_free(bst_t *bst, void (*value_free_fn)(void *))
{
    bst_free_aux(bst->root, value_free_fn);
    free(bst);
}


// PRIVATE
// No need for assertions for programmer errors in private functions

int bst_height_aux(node_t *node)
{
    return node == NULL
	? 0 : 1 + max(bst_height_aux(node->left), bst_height_aux(node->right));
}

static void bst_apply_aux(node_t *n, void (*f)(void *, void *))
{
    if (n != NULL)
    {
	bst_apply_aux(n->left, f);
	f(n->key, n->value);
	bst_apply_aux(n->right, f);
    }
}

static void bst_free_aux(node_t *n, void (*f)(void *))
{
    if (n != NULL)
    {
	bst_free_aux(n->left, f);
	bst_free_aux(n->right, f);
	// Might need to revise this w.r.t. future expansion of TYPE
	free(n->key);
	
	if (f != NULL)
	    f(n->value);

	free(n);
    }
}

static node_t *node_new(TYPE t, void *key, void *value)
{
    void *key_dup = duplicate(t, key);

    if (key_dup == NULL)
	return NULL;
    
    node_t *node = (node_t *)malloc(sizeof(node_t));

    if (node != NULL)
    {
	node->key = key_dup;
	node->value = value;
	node->balance = 0;
	node->left = NULL;
	node->right = NULL;
    }

    return node;
}

static void balance(node_t **node_ptr, node_t **parent_ptr, node_t **grand_parent_ptr)
{
    // If node has depth < 3, no need to balance
    if (grand_parent_ptr == NULL || parent_ptr == NULL)
	return;
    
    node_t *node = *node_ptr;
    node_t *parent = *parent_ptr;
    node_t *grand_parent = *grand_parent_ptr;
    
    // If node is the only leaf (1/4) of the current subtree, balance
    if ((grand_parent->left == NULL || grand_parent->right == NULL) &&
	(parent->left == NULL || parent->right == NULL))
    {
	if (grand_parent->left == parent)
	{
	    if (parent->right == node)
		left_rotate(parent_ptr);
	    
	    right_rotate(grand_parent_ptr);
	}
	
	if (grand_parent->right == parent)
	{
	    if (parent->left == node)
		right_rotate(parent_ptr);

	    left_rotate(grand_parent_ptr);
	}
    }
}

static void left_rotate(node_t **parent_ptr)
{
    node_t *root = *parent_ptr;
    node_t *pivot = root->right;

    root->right = pivot->left;
    pivot->left = root;
    *parent_ptr = pivot;
}

static void right_rotate(node_t **parent_ptr)
{
    node_t *root = *parent_ptr;
    node_t *pivot = root->left;

    root->left = pivot->right;
    pivot->right = root;
    *parent_ptr = pivot;
}

static int compare(TYPE t, void *v1, void *v2)
{
    int c = 0;
    
    switch (t)
    {
        case INT:
	    c = *((int *)v1) - *((int *)v2);
	    break;
        case STRING:
	    c = strcmp((char *)v1, (char *)v2);
	    break;
    }

    return c;
}

static void *duplicate(TYPE t, void *v)
{
    void *new_v = NULL;
    
    switch (t)
    {
        case INT:
	    new_v = malloc(sizeof(int));
	    *((int *)new_v) = *((int *)v);
	    break;
        case STRING:
	    new_v = (void *)strdup((char *)v);
	    break;
    }

    return new_v;
}

static char *strdup(const char *s)
{
    char *d = (char *)malloc(sizeof(char) * strlen(s) + 1);

    if (d != NULL)
    {
	strcpy(d, s);
    }

    return d;
}

int max(int a, int b)
{
    return a > b ? a : b;
}
