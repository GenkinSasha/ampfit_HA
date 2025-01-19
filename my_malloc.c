/*
I used & modified ready implementation from
https://tharikasblogs.blogspot.com
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "my_malloc.h"

ret_code_t my_malloc_init(void *mem_block, size_t mem_size);
ret_code_t my_malloc_alloc(size_t size, void **out_alloc_mem);
ret_code_t my_malloc_free(void *mem_to_free);

//----------Internal Data -------------
const uint16_t ALLOCATION_KEY = 0xBEEF;

typedef enum {
    BUSY = 0,
    FREE = 1,
} mem_block_state_t;

/* Simple Memory Control Block */
struct mem_ctrl_block{
    size_t size;
    int free;
    uint16_t allocated;
    struct mem_ctrl_block *prev;
    struct mem_ctrl_block *next;
};

#define MCB_SIZE (sizeof(struct mem_ctrl_block))

static void *memory_pool_limits[2] = {NULL, NULL};
static struct mem_ctrl_block *freeList;

//--------- Internal functions --------

static void split(struct mem_ctrl_block *fitting_slot, size_t size){
struct mem_ctrl_block *new_block;

    new_block = (void*)((void*)fitting_slot + size + MCB_SIZE);
    new_block->size = (fitting_slot->size) - size - MCB_SIZE;
    new_block->free = FREE;
    new_block->allocated = 0;
    new_block->next = fitting_slot->next;
    new_block->prev = fitting_slot;
    fitting_slot->size = size;
    fitting_slot->free = BUSY;
    fitting_slot->allocated = ALLOCATION_KEY;
    fitting_slot->next = new_block;
}

static void merge(struct mem_ctrl_block *block_to_free){    
    if(block_to_free->free == FREE){
        if((block_to_free->next != NULL) &&         // if next block is FREE - merge it with the current one
            (block_to_free->next->free == FREE)){
            block_to_free->size += (block_to_free->next->size) + MCB_SIZE;
            block_to_free->next = block_to_free->next->next;
        }
    }
    if (block_to_free->next){
        block_to_free->next->prev = block_to_free;
    }
}

/** Validate the pointer before call free(ptr):
    - is ptr inside the memory area bounds
    - is memory block was allocated by my_malloc()
*/
static int is_pointer_valid(void *ptr){
    struct mem_ctrl_block *p;

    if(((uint8_t *)memory_pool_limits[0] <= (uint8_t *)ptr) &&
        ((uint8_t *)ptr <= (uint8_t *)memory_pool_limits[1])){
            p = (struct mem_ctrl_block *)ptr;
            p--;
            if (p->allocated == ALLOCATION_KEY){
                return 1; // the pointer is valid
            }
        }
    return 0;             // otherwise  - not valid
}

/** Checks memory pool initialisation */
static int is_initialized(){
    if(freeList->size){
            return 1; // already initialized
        }
    return 0;
}

//------------------------  API functions ------------------

/**
ret_code_t my_malloc_init(void *mem_block, size_t mem_size);
- initialize the memory manager, by receiving a continuous memory block and its size (to be used for memory allocation) and
initializing any required internal states. After the init function completes successfully
the memory manager is ready for operation.
 */
ret_code_t my_malloc_init(void *mem_block, size_t mem_size){
    if((mem_block == NULL)||
        (mem_size <= MCB_SIZE))
        return my_malloc_init_failed;
    memset(mem_block, 0, mem_size);
    freeList = (struct mem_ctrl_block *)mem_block;
    freeList->size = MAX_POOL_SIZE - MCB_SIZE;
    freeList->free = FREE;
    freeList->next = NULL;
    freeList->prev = NULL;
    memory_pool_limits[0] = mem_block;
    memory_pool_limits[1] = mem_block + mem_size;

    return my_malloc_ok;
}

/**
ret_code_t my_malloc_alloc(size_t size, void **out_alloc_mem);
- receive a request for memory allocation for a given size,
and allocate a continuous memory block of that size from, if possible,
and return a pointer to the allocated memory block in the out parameter of the
function.
 */
ret_code_t my_malloc_alloc(size_t size, void **out_alloc_mem){
    struct mem_ctrl_block *curr;
    void *result;

    if(!is_initialized()){
        return my_malloc_alloc_failed; // Memory pool was not initialized!
    }
    curr = freeList;
    while((((curr->size) < size) ||
        ((curr->free) == BUSY)) &&
        (curr->next != NULL)){
        //prev = curr;
        curr = curr->next;
    }
    if((curr->size) == size){
        curr->free = BUSY;
        curr->allocated = ALLOCATION_KEY;
        result=(void *)(++curr); // Exact fitting block allocated!
        *out_alloc_mem = result;
        return my_malloc_ok;
    }
    else
        if((curr->size) > (size + MCB_SIZE)){
            split(curr,size);
            *out_alloc_mem = (void *)(++curr);  //Fitting block allocated with a split
            return my_malloc_ok;
        }
        else{
            result = NULL;
        return my_malloc_alloc_failed;    //No sufficient memory to allocate
    }
    return my_malloc_ok;
}

/**
ret_code_t my_malloc_free(void *mem_to_free);
- receive a pointer to a previously allocated memory, and free it.
*/
ret_code_t my_malloc_free(void *mem_to_free){
    if(is_pointer_valid(mem_to_free)){
        struct mem_ctrl_block *curr = mem_to_free;
        --curr;
        curr->free = FREE;
        curr->allocated = 0;    // deallocate
        //merge_old();
        merge(curr);
        return my_malloc_ok;
    }
    else
        return my_malloc_free_failed; // no valld pointer provided!
}
