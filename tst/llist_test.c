#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>
#include <llist.h>

void test_llist_new();
void test_llist_empty();
void test_llist_length();
void test_llist_first();
void test_llist_last();
void test_llist_prepend();
void test_llist_append();
void test_llist_insert();
void test_llist_remove();
void test_llist_index_of();
void test_llist_get();
void test_llist_search();

typedef struct mock {
    char *foo;
    int bar;
} mock_t;

int main()
{
    CU_pSuite pSuite1 = NULL;
    CU_pSuite pSuite2 = NULL;
    
    // Initialise registry
    if (CU_initialize_registry() != CUE_SUCCESS)
	return CU_get_error();
    
    // Add suites to registry
    pSuite1 = CU_add_suite("LLIST Basic Functions Suite", NULL, NULL);
    pSuite2 = CU_add_suite("LLIST Advanced Functions Suite", NULL, NULL);
    
    if (pSuite1 == NULL ||
	pSuite2 == NULL)
    {
	CU_cleanup_registry();
	return CU_get_error();
    }

    // Add tests to suites
    if ((CU_add_test(pSuite1, "test of llist_new()",      test_llist_new)      == NULL) ||
	(CU_add_test(pSuite1, "test of llist_empty()",    test_llist_empty)    == NULL) ||
	(CU_add_test(pSuite1, "test of llist_length()",   test_llist_length)   == NULL) ||
	(CU_add_test(pSuite1, "test of llist_first()",    test_llist_first)    == NULL) ||
	(CU_add_test(pSuite1, "test of llist_last()",     test_llist_first)    == NULL) ||
	(CU_add_test(pSuite2, "test of llist_prepend()",  test_llist_prepend)  == NULL) ||
	(CU_add_test(pSuite2, "test of llist_append()",   test_llist_append)   == NULL) ||
	(CU_add_test(pSuite2, "test of llist_insert()",   test_llist_insert)   == NULL) ||
	(CU_add_test(pSuite2, "test of llist_remove()",   test_llist_remove)   == NULL) ||
	(CU_add_test(pSuite2, "test of llist_index_of()", test_llist_index_of) == NULL) ||
	(CU_add_test(pSuite2, "test of llist_get()",      test_llist_get)      == NULL) ||
	(CU_add_test(pSuite2, "test of llist_search()",   test_llist_search)   == NULL))
    {
	CU_cleanup_registry();
	return CU_get_error();
    }
    
    // Run tests
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    
    // Clean up registry
    CU_cleanup_registry();
    return CU_get_error();
}


char *strdup(const char *str)
{
    return strcpy(malloc(sizeof(char) * (strlen(str) + 1)), str);
}

mock_t *mock_new(char *foo, int bar)
{
    mock_t *m = malloc(sizeof *m);

    if (m != NULL)
    {
	m->foo = strdup(foo);
	m->bar = bar;
    }

    return m;
}

bool mock_eq(const void *m1, void *m2)
{
    mock_t *mock1 = (mock_t *)m1;
    mock_t *mock2 = (mock_t *)m2;
    
    return strcmp(mock1->foo, mock2->foo) == 0 && mock1->bar == mock2->bar;
}

void mock_free(void *m)
{
    mock_t *mock = (mock_t *)m;
    
    free(mock->foo);
    free(mock);
}


void test_llist_new()
{
    llist_t *l = llist_new(NULL);

    CU_ASSERT_TRUE(llist_empty(l));
    CU_ASSERT_PTR_NULL(llist_first(l));
    CU_ASSERT_PTR_NULL(llist_last(l));

    llist_free(l);
}

void test_llist_empty()
{
    llist_t *l = llist_new(NULL);

    CU_ASSERT_TRUE(llist_empty(l));
    llist_append(l, NULL);
    CU_ASSERT_FALSE(llist_empty(l));

    llist_free(l);
}

void test_llist_length()
{
    llist_t *l = llist_new(NULL);

    CU_ASSERT_EQUAL(llist_length(l), 0);
    llist_append(l, NULL);
    CU_ASSERT_EQUAL(llist_length(l), 1);

    llist_free(l);
}

void test_llist_first()
{
    llist_t *l = llist_new(mock_free);

    mock_t cmp1 = { .foo = "mock1", .bar = 1 };
    mock_t cmp2 = { .foo = "mock2", .bar = 2 };
    
    CU_ASSERT_PTR_NULL(llist_first(l));
    llist_prepend(l, mock_new("mock1", 1));
    CU_ASSERT_TRUE(mock_eq(llist_first(l), &cmp1));
    llist_prepend(l, mock_new("mock2", 2));
    CU_ASSERT_TRUE(mock_eq(llist_first(l), &cmp2));

    llist_free(l);
}

void test_llist_last()
{
    llist_t *l = llist_new(mock_free);

    mock_t cmp1 = { .foo = "mock1", .bar = 1 };
    mock_t cmp2 = { .foo = "mock2", .bar = 2 };
    
    CU_ASSERT_PTR_NULL(llist_last(l));
    llist_append(l, mock_new("mock1", 1));
    CU_ASSERT_TRUE(mock_eq(llist_last(l), &cmp1));
    llist_append(l, mock_new("mock2", 2));
    CU_ASSERT_TRUE(mock_eq(llist_last(l), &cmp2));

    llist_free(l);
}

void test_llist_prepend()
{
    llist_t *l = llist_new(mock_free);

    mock_t cmp1 = { .foo = "mock1", .bar = 1 };
    mock_t cmp2 = { .foo = "mock2", .bar = 2 };
    
    llist_prepend(l, mock_new("mock1", 1));
    CU_ASSERT_TRUE(mock_eq(llist_first(l), &cmp1));
    llist_prepend(l, mock_new("mock2", 2));
    CU_ASSERT_TRUE(mock_eq(llist_first(l), &cmp2));
    
    llist_free(l);
}

void test_llist_append()
{
    llist_t *l = llist_new(mock_free);

    mock_t cmp1 = { .foo = "mock1", .bar = 1 };
    mock_t cmp2 = { .foo = "mock2", .bar = 2 };
    
    llist_append(l, mock_new("mock1", 1));
    CU_ASSERT_TRUE(mock_eq(llist_last(l), &cmp1));
    llist_append(l, mock_new("mock2", 2));
    CU_ASSERT_TRUE(mock_eq(llist_last(l), &cmp2));

    llist_free(l);
}

void test_llist_insert()
{
    llist_t *l = llist_new(mock_free);

    mock_t cmp1 = { .foo = "mock1", .bar = 1 };
    mock_t cmp2 = { .foo = "mock2", .bar = 2 };
    mock_t cmp3 = { .foo = "mock3", .bar = 3 };
    
    llist_insert(l, mock_new("mock1", 1), 0);
    CU_ASSERT_TRUE(mock_eq(llist_first(l), &cmp1));
    llist_insert(l, mock_new("mock2", 2), 1);
    CU_ASSERT_TRUE(mock_eq(llist_last(l), &cmp2));
    llist_insert(l, mock_new("mock3", 3), 1);
    CU_ASSERT_TRUE(mock_eq(llist_get(l, 1), &cmp3));

    llist_free(l);
}

void test_llist_remove()
{
    llist_t *l = llist_new(mock_free);
    llist_t *l2 = llist_new(NULL);

    mock_t cmp1 = { .foo = "mock1", .bar = 1 };
    mock_t cmp2 = { .foo = "mock2", .bar = 2 };
    mock_t cmp3 = { .foo = "mock3", .bar = 3 };

    llist_append(l, mock_new("mock1", 1));
    llist_append(l, mock_new("mock2", 2));
    llist_append(l, mock_new("mock3", 3));
    CU_ASSERT_PTR_NULL(llist_remove(l, 1));
    CU_ASSERT_EQUAL(llist_index_of(l, mock_eq, &cmp2), -1);
    llist_remove(l, 1);
    CU_ASSERT_EQUAL(llist_index_of(l, mock_eq, &cmp3), -1);
    llist_remove(l, 0);
    CU_ASSERT_EQUAL(llist_index_of(l, mock_eq, &cmp1), -1);

    llist_append(l2, mock_new("mock1", 1));
    mock_t *data = llist_remove(l2, 0);
    CU_ASSERT_TRUE(mock_eq(data, &cmp1));
    mock_free(data);
    
    llist_free(l);
    llist_free(l2);
}

void test_llist_index_of()
{
    llist_t *l = llist_new(mock_free);

    mock_t cmp1 = { .foo = "mock1", .bar = 1 };
    mock_t cmp2 = { .foo = "mock2", .bar = 2 };
    mock_t cmp3 = { .foo = "mock3", .bar = 3 };
    
    CU_ASSERT_EQUAL(llist_index_of(l, mock_eq, &cmp1), -1);
    llist_insert(l, mock_new("mock1", 1), 0);
    CU_ASSERT_EQUAL(llist_index_of(l, mock_eq, &cmp1), 0);
    llist_insert(l, mock_new("mock2", 2), 1);
    CU_ASSERT_EQUAL(llist_index_of(l, mock_eq, &cmp2), 1);
    llist_insert(l, mock_new("mock3", 3), 1);
    CU_ASSERT_EQUAL(llist_index_of(l, mock_eq, &cmp3), 1);

    llist_free(l);
}

void test_llist_get()
{
    llist_t *l = llist_new(mock_free);

    mock_t cmp1 = { .foo = "mock1", .bar = 1 };
    mock_t cmp2 = { .foo = "mock2", .bar = 2 };
    mock_t cmp3 = { .foo = "mock3", .bar = 3 };

    llist_append(l, mock_new("mock1", 1));
    llist_append(l, mock_new("mock2", 2));
    llist_append(l, mock_new("mock3", 3));
    CU_ASSERT_TRUE(mock_eq(llist_get(l, 1), &cmp2));
    CU_ASSERT_TRUE(mock_eq(llist_get(l, 2), &cmp3));
    CU_ASSERT_TRUE(mock_eq(llist_get(l, 0), &cmp1));

    llist_free(l);
}

void test_llist_search()
{
    llist_t *l = llist_new(mock_free);

    mock_t cmp1 = { .foo = "mock1", .bar = 1 };
    mock_t cmp2 = { .foo = "mock2", .bar = 2 };
    mock_t cmp3 = { .foo = "mock3", .bar = 3 };

    llist_append(l, mock_new("mock1", 1));
    llist_append(l, mock_new("mock2", 2));
    CU_ASSERT_TRUE(mock_eq(llist_search(l, mock_eq, &cmp1), &cmp1));
    CU_ASSERT_TRUE(mock_eq(llist_search(l, mock_eq, &cmp2), &cmp2));
    CU_ASSERT_PTR_NULL(llist_search(l, mock_eq, &cmp3));
    
    llist_free(l);
}
