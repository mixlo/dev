#ifndef __BST_H__
#define __BST_H__

/**
 * @file bst.h
 * @author Björn Berggren
 * @date 2015-10-21
 * @brief An implementation of a generic binary search tree.
 *
 * TBD, longer description
 */

enum type {
  INT,
  STRING
};

typedef struct bst bst_t;
typedef enum type TYPE;

int bst_size(bst_t *bst);

int bst_height(bst_t *bst);

/**
 * Creates a new binary search tree with the specified type.
 *
 * @param t The type of the binary search tree.
 * @return A pointer to a new binary search tree, or NULL if memory could not 
 * be allocated.
 */
bst_t *bst_new(TYPE t);

/**
 * Inserts a new node into the binary search tree.
 *
 * @param bst A pointer to the binary search tree in which the node should 
 * be inserted.
 * @param key The key of the node to insert.
 * @param value The value of the node to insert.
 * @return 1 if the node was inserted successfully, otherwise 0.
 */
int bst_insert(bst_t *bst, void *key, void *value);

/**
 * Removes a node from the binary search tree.
 *
 * @param bst A pointer to the binary search tree from which the node should 
 * be removed.
 * @param key The key of the node to remove.
 */
void bst_remove(bst_t *bst, void *key);

/**
 * Searches a binary search tree for a node with the given key and returns 
 * its value.
 *
 * @param bst A pointer to the binary search tree that should be searched.
 * @param key The key of the node to search for.
 * @return The value of the node with the given key if it exists, 
 * otherwise NULL.
 */
void *bst_search(bst_t *bst, void *key);

/**
 * Applies a function on all keys in the binary search tree.
 *
 * @param bst The binary search tree on which to apply the function.
 * @param f The function to apply on all keys in the tree.
 */
void bst_apply(bst_t *bst, void (*f)(void *, void *));

void bst_free(bst_t *bst, void (*value_free_fn)(void *));

#endif
