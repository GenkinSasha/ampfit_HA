#ifndef _MY_MALLOC_H_
#define _MY_MALLOC_H_
#define VERSION "0.1"

#define MAX_POOL_SIZE (100 * 1024)  // 100K max


typedef enum {
    my_malloc_ok = 0,
    my_malloc_init_failed = -1, //memory pool too big or other error
    my_malloc_alloc_failed = -2,
    my_malloc_free_failed = -3
} ret_code_t;

ret_code_t my_malloc_init(void *mem_block, size_t mem_size);
ret_code_t my_malloc_alloc(size_t size, void **out_alloc_mem);
ret_code_t my_malloc_free(void *mem_to_free);

#endif /* _MY_MALLOC_H_ */
