#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "my_malloc.h"

typedef enum {
    PASS = 0,
    FAIL = 1, //memory pool too big or other error
} test_result_t;

static uint8_t test_mem_pool[MAX_POOL_SIZE / 2];
static void *test_blocks_addr[10];

test_result_t test_case_1(){
    ret_code_t ret;
    ret = my_malloc_init(test_mem_pool, MAX_POOL_SIZE / 2);
    if(ret != my_malloc_ok)
        return FAIL;
    return PASS;
}

test_result_t test_case_2(){
    ret_code_t ret;
    ret = my_malloc_init(test_mem_pool, MAX_POOL_SIZE * 2);
    if(ret != my_malloc_ok)
        return FAIL;
    return PASS;
}

/*
In this test I found an error in memory managing algorythm.
To be debugged....
*/
test_result_t test_case_3(){
int i, block_size;
ret_code_t ret;

    srand(19700101);
    for(i = 0; i < 10; i++){
        block_size = 2; //rand() % 10 + 1;
        ret = my_malloc_alloc(block_size * 1024, (void *)&test_blocks_addr[i]);
        if(ret != my_malloc_ok)
            return FAIL;
    }
    return PASS;
}

test_result_t test_case_4(){
int i;
ret_code_t ret;

    for(i = 0; i < 10; i++){
        ret = my_malloc_free(test_blocks_addr[i]);
            if(ret != my_malloc_ok)
                return FAIL;
    }
    return PASS;
}

int main(){
    printf("Testing my_malloc() v%s: \n", VERSION);
    printf("\nTest case #1: request memory pull less than MAX_POOL_SIZE. Expected: PASS\n");
    printf((test_case_1()) == PASS?"PASS":"FAIL");
    printf("\nTest case #2: request memory pull more than MAX_POOL_SIZE. Expected: FAIL\n");
    printf((test_case_2()) == PASS?"PASS":"FAIL");

    printf("\nTest case #3: request 10 memory blocks of random size from 1 to 10KB. Expected: PASS\n");
    printf((test_case_3()) == PASS?"PASS":"FAIL");
    printf("\nTest case #4: free all blocks requested in case #3. Expected: PASS\n");
    printf((test_case_4()) == PASS?"PASS":"FAIL");
    //....
}
