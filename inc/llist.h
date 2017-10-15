/**
 * @file llist.h
 * @author Björn Berggren
 * @date 2016-02-13
 * @copyright The MIT License
 *
 * @brief An implementation of a generic linked list.
 *
 * This is the interface of a generic linked list data structure.
 */

#ifndef __LLIST_H__
#define __LLIST_H__

#include <stdbool.h>

/**
 * @brief The declaration of the linked list data type.
 *
 * A call to llist_new() will return a pointer to a new, empty @c llist_t. 
 * Each function in this interface requires a @c llist_t argument to perform 
 * the specific operation on.
 */
typedef struct llist llist_t;

/**
 * @brief Creates a new, empty linked list.
 * 
 * If the @c free_fun argument is not @c NULL, it will be used during calls 
 * to the llist_remove() and llist_free() functions.
 * 
 * @param free_fun A pointer to a function that, if not @c NULL, will be run 
 * during a call to the llist_remove() and llist_free() functions. The function 
 * will receive the data of the to be removed element, and its purpose is 
 * to make it easier to simultaneously free both the memory taken up by an 
 * element and the memory taken up by the element data.
 * @return A reference to the newly created empty list.
 */
llist_t *llist_new(void (*free_fun)(void *));

/**
 * @brief Checks if the length of a list is zero (i.e. it contains no elements).
 *
 * @param list The list to check.
 * @return @c true if the length of the list is zero, otherwise @c false.
 */
bool llist_empty(llist_t *list);

/**
 * @brief Returns the length of the list (i.e. the number of elements contained 
 * in it).
 *
 * @param list The list whose length to determine.
 * @return The length of the list.
 */
int llist_length(llist_t *list);

/**
 * @brief Returns the data of the first element of a list.
 *
 * @param list The list from which to retrieve data.
 * @return The data of the first element of the list.
 */
void *llist_first(llist_t *list);

/**
 * @brief Returns the data of the last element of a list.
 *
 * @param list The list from which to retrieve data.
 * @return The data of the last element of the list.
 */
void *llist_last(llist_t *list);

/**
 * @brief Inserts an element at the first position of a list.
 *
 * @param list The list into which to insert the element.
 * @param data The data to store in the element.
 * @return @c true if the element was successfully inserted, otherwise @c false.
 */
bool llist_prepend(llist_t *list, void *data);

/**
 * @brief Inserts an element at the last position of a list.
 *
 * @param list The list into which to insert the element.
 * @param data The data to store in the element.
 * @return @c true if the element was successfully inserted, otherwise @c false.
 */
bool llist_append(llist_t *list, void *data);

/**
 * @brief Inserts an element at a given index of a list.
 * 
 * Permitted index interval is <CODE>0 <= index <= llist_length(list)</CODE>.
 *
 * @param list The list into which to insert the element.
 * @param data The data to store in the element.
 * @param index The index of the position in the list where the element will be inserted.
 * @return @c true if the element was successfully inserted, otherwise @c false.
 */
bool llist_insert(llist_t *list, void *data, int index);

/**
 * @brief Removes an element from a list at a given position.
 *
 * If a free-function was provided to llist_new() when the list was created, 
 * the stored data of the element will be passed to that function and this 
 * function will return @c NULL, otherwise the stored data is returned.
 *
 * Permitted index interval is <CODE>0 <= index < llist_length(list)</CODE>.
 *
 * @param list The list from which to remove the element.
 * @param index The index of the position of the element to remove.
 * @return @c NULL if a free-function was provided at list creation, otherwise 
 * the stored data.
 */
void *llist_remove(llist_t *list, int index);

/**
 * @brief Gets the index of the first element indicated equal to @c cmp_val 
 * by @c eq_fun.
 *
 * @param list The list to search.
 * @param eq_fun A pointer to an equals-function accepting an element an a 
 * value to compare, returning @c true if the two are equal.
 * The first parameter represents the iterated elements of the list,
 * the second parameter represents the value to compare to when searching.
 * @param cmp_val The value to which the iterated elements of the list will be 
 * compared during the search. This value will be input as the second argument 
 * to @c eq_fun when searching.
 * @return The index of the first element indicated equal to @c cmp_val 
 * by @c eq_fun.If no equal element is found, -1 is returned.
 */
int llist_index_of(llist_t *list, bool (*eq_fun)(const void *, void *), void *cmp_val);

/**
 * @brief Gets the stored data of an element at a given position.
 *
 * Permitted index interval is <CODE>0 <= index < llist_length(list)</CODE>.
 *
 * @param list The list to search.
 * @param index The index of the position of the element to get the data from.
 * @return The data of the element at position @c index in @c list.
 */
void *llist_get(llist_t *list, int index);

/**
 * @brief Searches a list for an element, using the compare function and value 
 * provided.
 *
 * @param list The list on which to perform the search.
 * @param eq_fun A pointer to a compare function accepting an element and a 
 * value to compare, returning @c true if the two are equal.
 * The first parameter represents the iterated elements of the list, 
 * the second parameter represents the value to compare to when searching.
 * @param cmp_val The value to which the iterated elements of the list will be 
 * compared during the search. This value will be input as the second argument 
 * to @c eq_fun when searching.
 * @return The data of the first element indicated equal to @c cmp_val 
 * by @c eq_fun. If no equal element is found, @c NULL is returned.
 */
void *llist_search(llist_t *list, bool (*eq_fun)(const void *, void *), void *cmp_val);

/**
 * @brief Deletes the entire list from memory. 
 *
 * If a free-function was provided to llist_new() when the list was created, 
 * the stored data of each element will be passed to that function during 
 * the execution of this function.
 *
 * @param list The list to delete.
 */
void llist_free(llist_t *list);

#endif
