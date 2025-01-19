# ampfit_HA
# Ampfit home assigment
my_malloc is a small memory manager - simple utilite for memory allocation in embedded system.
version 0.1

To build test target only:
    make test

To build all:
    make

To clean:
    make clean

To test:
        bin/test

Usage:
ret_code_t my_malloc_init(void *mem_block, size_t mem_size);
- initialize the memory manager, by receiving a continuous memory block and its size (to be used for memory allocation) and
initializing any required internal states. After the init function completes successfully
the memory manager is ready for operation.

ret_code_t my_malloc_alloc(size_t size, void **out_alloc_mem);
- receive a request for memory allocation for a given size,
and allocate a continuous memory block of that size from, if possible,
and return a pointer to the allocated memory block in the out parameter of the
function.

ret_code_t my_malloc_free(void *mem_to_free);
- receive a pointer to a previously allocated memory, and free it.

