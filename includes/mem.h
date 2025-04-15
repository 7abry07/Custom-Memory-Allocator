#ifndef MEM_H
#define MEM_H

#include <stddef.h>
#include <unistd.h>

// allocates "size" memory on the heap
// and returns a pointer to that memory
// if it fails it returns NULL
void *malloc(size_t s);

// it changes the size of the memory
// pointed from pointer to "size"
// (passing 0 to "size" results in the memory being freed
// retaining it's original size)
void *realloc(void *pointer, size_t s);

// allocates an array of "n_elements" with size "s_elements"
// and returns a pointer to the allocated memory
// if it fails returns NULL
void *calloc(size_t n_elements, size_t s_elements);

// frees memory where pointer points too
// returns 0 if the memory is successfully freed
// returns 1 if the memory was already free or
// if the address is invalid
// (accessing freed memory results in undefined behaviour)
int free(void *pointer);

#endif