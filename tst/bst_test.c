#include <CUnit/Basic.h>
#include <bst.h>

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
    if ((CU_add_test(pSuite1, "test of bst_insert()", test_bst_insert) == NULL) ||
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

void test_bst_insert()
{
    printf("TBI ...");
}

void test_bst_remove()
{
    printf("TBI ...");
}

void test_bst_search()
{
    printf("TBI ...");
}
