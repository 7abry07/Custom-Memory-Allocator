#ifndef MEM_INTERNALS_H
#define MEM_INTERNALS_H

#include <stddef.h>
#include <unistd.h>

#define MAGIC_NUM 1234567890
#define round_to_8(x) ((x + 7) >> 3) << 3; // rouds the given number to the nearest biggest multiple of 8

// utility typedef
typedef struct meta_block *meta;

// meta-data struct for storing metadata about the memory block
// ----------------------------------------------------------------------------------------
// free         -> 1 (or every other number) if the memory is free / 0 if its not
// magic_number -> number used to validate the pointer in free()
// next         -> pointer to the next meta-data element in the list
// prev         -> pointer to the previous meta-data element in the list
// *ptr         -> pointer that points to data[]
// data[]       -> starting address of the user's memory
// ----------------------------------------------------------------------------------------
struct meta_block
{
  int free;
  int magic_number;
  meta next;
  meta prev;
  size_t size;
  void *ptr;
  char data[];
};

// checks if there are any allocated blocks
int isEmpty();

//adds the block to the end of the list
void add_to_tail(meta block);

// splits the memory block in half if its too large and
// appends the newly created block at the end of the list
void split_block(meta block);

// extends the heap memory of "size" and
// creates a block that gets added at the end of the list
// if "sbrk" fails it returns NULL
meta extend_heap(size_t size);

// tries to find a suitable memory block
// for the given size using the first-fit policy
// if it doesnt find any it return NULL
meta find_suitable_block(size_t size);

// checks if the given address is valid (allocated by malloc, calloc or realloc)
// if it is it returns a pointer to the start of the memory block
// if its not it returns NULL
meta validate_address(void *pointer);

#endif