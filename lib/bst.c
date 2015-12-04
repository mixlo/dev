#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <bst.h>


struct node
{
    void *key;
    void *value;
    struct node *left;
    struct node *right;
};

typedef struct node node_t;

// typedeffed to bst_t in "bst.h"
struct bst
{
    TYPE type;
    node_t *root;
    int size;
};


// PRIVATE DECLARATIONS

static node_t *node_new(TYPE t, void *key, void *value);
static void balance(node_t **node_ptr, node_t **parent_ptr, node_t **grand_parent_ptr);
static void left_rotate(node_t **parent_ptr);
static void right_rotate(node_t **parent_ptr);
static void bst_apply_aux(node_t *n, void (*f)(void *, void *));
static int compare(TYPE t, void *v1, void *v2);
static void *duplicate(TYPE t, void *v);
static char *strdup(const char *s);


// PUBLIC

int bst_size(bst_t *bst)
{
    return bst->size;
}

int max(int a, int b)
{
    return a > b ? a : b;
}

int bst_height_aux(node_t *node)
{
    return node == NULL
	? 0
	: 1 + max(bst_height_aux(node->left), bst_height_aux(node->right));
}

int bst_height(bst_t *bst)
{
    return bst == NULL ? 0 : bst_height_aux(bst->root);
}

bst_t *bst_new(TYPE t)
{
    bst_t *new_bst = (bst_t *)malloc(sizeof(bst_t));

    if (new_bst != NULL)
    {
	new_bst->type = t;
	new_bst->root = NULL;
	new_bst->size = 0;
    }

    return new_bst;
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
	
	balance(iterator, parent, grand_parent);
    }
    
    return new_node != NULL ? 1 : 0;
}

void bst_remove(bst_t *bst, void *key)
{
    // TBI
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


// PRIVATE
// No need for assertions for programmer errors in private functions

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

//static void left_rotate(node_t *node)
//{
//    // Swap node and right subtree, parent now points to right subtree
//    node_t copy = *node;
//    *node = *(node->right);
//    *(node->right) = copy;
//
//    // Node's right subtree will be right's left subtree after rotation
//    node->right = node->right->left;
//    node->right->left = NULL;
//}
//
//static void right_rotate(node_t *node)
//{
//    // Swap node and left subtree, parent now points to left subtree
//    node_t copy = *node;
//    *node = *(node->left);
//    *(node->left) = copy;
//
//    // Node's left subtree will be left's right subtree after rotation
//    node->left = node->left->right;
//    node->left->right = NULL;
//}

static void bst_apply_aux(node_t *n, void (*f)(void *, void *))
{
    if (n != NULL)
    {
	bst_apply_aux(n->left, f);
	f(n->key, n->value);
	bst_apply_aux(n->right, f);
    }
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
