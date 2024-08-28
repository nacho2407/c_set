#include "nset.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TEST_SIZE 128

// debugging required
void test_set(void)
{
        if(TEST_SIZE < 1)
                return;

        nset_i* set;
        if(!nset_get_i(set, NULL, NULL))
                return;

        int test_case[TEST_SIZE];

        printf("Test cases :");

        srand(time(NULL));
        for(int i = 0; i < TEST_SIZE; i++) {
                test_case[i] = rand() % TEST_SIZE;

                printf(" %d", test_case[i]);

                nset_insert_i(set, test_case[i]);
        }
        printf("\n\n");

        printf("Set :");
        
        nset_iter_i* iter = nset_begin_i(set);
        int i;
        nset_foreach(i, iter, i)
                printf(" %d", i);
        printf("\n\n");
        nset_free_iter_i(iter);

        int target = rand() % TEST_SIZE;
        nset_erase_i(set, nset_find_i(set, target));
        printf("Erase from set : %d", target);
        for(int i = 0; i < TEST_SIZE / 10; i++) {
                target = rand() % TEST_SIZE;
                nset_erase_i(set, nset_find_i(set, target));
                printf(", %d", target);
        }
        printf("\n\n");

        printf("Set :");
        
        iter = nset_begin_i(set);
        nset_foreach(i, iter, i)
                printf(" %d", i);
        printf("\n\n");
        nset_free_iter_i(iter);

        nset_free_i(set);
}

int main(void)
{
        test_set();

        return 0;
}
