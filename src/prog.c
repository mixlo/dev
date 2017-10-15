#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <bst.h>

char *strdup(const char *s)
{
    char *d = (char *)malloc(sizeof(char) * strlen(s) + 1);

    if (d != NULL)
    {
	strcpy(d, s);
    }

    return d;
}

void print_tree(void *k, void *v)
{
    printf("Key: %d\tValue: %s\n", *((int *)k), (char *)v);
}

void print(bst_t *bst)
{
    printf("tree height: %d\ntree size: %d\n", bst_height(bst), bst_size(bst));
    bst_apply(bst, &print_tree);
    puts("");
}

void free_data(void *data)
{
    free(data);
}

int main()
{
    bst_t *bst = bst_new(INT);

    int a=1, b=2, c=3, d=4, e=5;

    print(bst);
    bst_insert(bst, &a, malloc(sizeof(char)*10));
    print(bst);
    bst_insert(bst, &b, malloc(sizeof(char)*20));
    print(bst);
    bst_insert(bst, &c, malloc(sizeof(char)*30));
    print(bst);
    bst_insert(bst, &d, malloc(sizeof(char)*40));
    print(bst);
    bst_insert(bst, &e, malloc(sizeof(char)*50));
    print(bst);

    bst_free(bst, free_data);
    print(bst);

    return 0;
}
