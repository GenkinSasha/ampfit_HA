/*
I used & modified ready implementation from
https://tharikasblogs.blogspot.com
*/
#include <stdio.h>
#include <stdlib.h>
#include "my_malloc.h"

ret_code_t my_malloc_init(void *mem_block, size_t mem_size);
ret_code_t my_malloc_alloc(size_t size, void **out_alloc_mem);
ret_code_t my_malloc_free(void *mem_to_free);

//----------Internal Data -------------
typedef enum {
    BUSY = 0,
    FREE = 1, //memory pool too big or other error
} mem_block_state_t;

struct block{   //should be packed??
 size_t size;
 int free;
 struct block *next;
};

static void *memory_pool_limits[2] = {NULL, NULL};
static struct block *freeList;

//--------- Internal functions --------

static void split(struct block *fitting_slot,size_t size){
struct block *new_block=(void*)((void*)fitting_slot + size + sizeof(struct block));

    new_block->size = (fitting_slot->size) - size - sizeof(struct block);
    new_block->free = 1;
    new_block->next = fitting_slot->next;
    fitting_slot->size = size;
    fitting_slot->free = 0;
    fitting_slot->next = new_block;
}

static void merge(){
struct block *curr;
    curr = freeList;
    while((curr->next)!=NULL){
        if((curr->free) &&
            (curr->next->free)){
            curr->size += (curr->next->size)+sizeof(struct block);
            curr->next = curr->next->next;
        }
        curr = curr->next;
    }
}

//----------  API functions --------------
ret_code_t my_malloc_init(void *mem_block, size_t mem_size){
    if((mem_size > MAX_POOL_SIZE)||
        (mem_block == NULL))
        return my_malloc_init_failed;

    freeList = (struct block *)mem_block;
    freeList->size = MAX_POOL_SIZE - sizeof(struct block);
    freeList->free = 1;
    freeList->next = NULL;
    memory_pool_limits[0] = mem_block;
    memory_pool_limits[1] = mem_block + mem_size;

    return my_malloc_ok;
}

ret_code_t my_malloc_alloc(size_t size, void **out_alloc_mem){
struct block *curr;
void *result;

    if(!(freeList->size)){
        return my_malloc_alloc_failed; // Memory pool was not initialized!
    }
    curr=freeList;
    while((((curr->size) < size)||
        ((curr->free) == 0))&&
        (curr->next != NULL)){
        //prev = curr;
        curr = curr->next;
    }
    if((curr->size)==size){
        curr->free=0;
        result=(void *)(++curr); // Exact fitting block allocated!
        *out_alloc_mem = result;
        return my_malloc_ok;
    }
    else
        if((curr->size)>(size+sizeof(struct block))){
            split(curr,size);
            *out_alloc_mem=(void *)(++curr);  //Fitting block allocated with a split
            return my_malloc_ok;
        }
        else{
            result=NULL;
        return my_malloc_alloc_failed;    //No sufficient memory to allocate
    }
    return my_malloc_ok;
}

ret_code_t my_malloc_free(void *mem_to_free){
    if((memory_pool_limits[0]<=mem_to_free)&&
        (mem_to_free<=memory_pool_limits[1])){
        struct block* curr=mem_to_free;
        --curr;
        curr->free=1;
        merge();
        return my_malloc_ok;
    }
    else
        return my_malloc_free_failed; // no valld pointer provided!
}
