#include <CUnit/Basic.h>
#include <bst.h>

void test_bst_size();
void test_bst_height();
void test_bst_insert();
void test_bst_remove();
void test_bst_search();

int main()
{
    CU_pSuite pSuite1 = NULL;
    
    // Initialise registry
    if (CU_initialize_registry() != CUE_SUCCESS)
	return CU_get_error();
    
    // Add suites to registry
    pSuite1 = CU_add_suite("BST Basic Functions Suite", NULL, NULL);
    if (pSuite1 == NULL)
    {
	CU_cleanup_registry();
	return CU_get_error();
    }
    
    // Add tests to suites
    if ((CU_add_test(pSuite1, "test of bst_size()", test_bst_size) == NULL) ||
	(CU_add_test(pSuite1, "test of bst_height()", test_bst_height) == NULL) ||
	(CU_add_test(pSuite1, "test of bst_insert()", test_bst_insert) == NULL) ||
	(CU_add_test(pSuite1, "test of bst_remove()", test_bst_remove) == NULL) ||
	(CU_add_test(pSuite1, "test of bst_search()", test_bst_search) == NULL))
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

void test_bst_size()
{
    bst_t *bst = NULL;
    int a=1, b=2, c=3;
    
    CU_ASSERT_EQUAL(bst_size(bst), -1);
    bst = bst_new(INT);
    CU_ASSERT_EQUAL(bst_size(bst), 0);
    bst_insert(bst, &a, NULL);
    CU_ASSERT_EQUAL(bst_size(bst), 1);
    bst_insert(bst, &b, NULL);
    bst_insert(bst, &c, NULL);
    CU_ASSERT_EQUAL(bst_size(bst), 3);
    bst_remove(bst, &a);
    bst_remove(bst, &b);
    bst_remove(bst, &c);
    CU_ASSERT_EQUAL(bst_size(bst), 0);

    bst_free(bst, NULL);
}

void test_bst_height()
{
    bst_t *bst = NULL;
    int a=1, b=2, c=3, d=4, e=5, f=6, g=7, h=8;
    
    CU_ASSERT_EQUAL(bst_height(bst), -1);
    bst = bst_new(INT);
    CU_ASSERT_EQUAL(bst_height(bst), 0);
    bst_insert(bst, &a, NULL);
    CU_ASSERT_EQUAL(bst_height(bst), 1);
    bst_insert(bst, &b, NULL);
    bst_insert(bst, &c, NULL);
    CU_ASSERT_EQUAL(bst_height(bst), 2);
    bst_insert(bst, &d, NULL);
    bst_insert(bst, &e, NULL);
    bst_insert(bst, &f, NULL);
    bst_insert(bst, &g, NULL);
    CU_ASSERT_EQUAL(bst_height(bst), 3);
    bst_insert(bst, &h, NULL);
    CU_ASSERT_EQUAL(bst_height(bst), 4);
    bst_remove(bst, &d);
    CU_ASSERT_EQUAL(bst_height(bst), 3);

    bst_free(bst, NULL);
}

void test_bst_insert()
{
    bst_t *bst = NULL;
    int a=1, b=2, c=3;
    
    CU_ASSERT_EQUAL(bst_size(bst), -1);
    bst = bst_new(INT);
    CU_ASSERT_EQUAL(bst_size(bst), 0);
    bst_insert(bst, &a, "a");
    bst_insert(bst, &b, "b");
    bst_insert(bst, &c, "c");
    CU_ASSERT_STRING_EQUAL((char *)bst_search(bst, &c), "c");
    bst_remove(bst, &a);
    bst_remove(bst, &b);
    bst_remove(bst, &c);
    bst_insert(bst, &c, "d");
    CU_ASSERT_STRING_EQUAL((char *)bst_search(bst, &c), "d");
    
    bst_free(bst, NULL);
}

void test_bst_remove()
{
    printf("TBI ...");
}

void test_bst_search()
{
    printf("TBI ...");
}
