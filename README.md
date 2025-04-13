
# Custom Memory Allocator
This project implements a simple dynamic memory allocator in C, similar in functionality to standard malloc, calloc, realloc, and free. It manages memory by manually extending the heap using sbrk, organizing memory blocks in a doubly linked list, and storing metadata for each allocated block.

# Features
✅ Manual heap management using `sbrk`

✅ Block metadata tracking (`free`, `size`, etc.)

✅ Basic first-fit allocation policy

✅ Support for `malloc`, `calloc`, `realloc`, and `free`

✅ Splitting large blocks to reduce fragmentation

✅ Magic number validation for safe deallocation

# Data Structures
### meta_block
Each block of memory is associated with metadata stored in a struct `meta_block`, which contains:

* `free` : Whether the block is free (1) or allocated (0)

* `magic_number` : Used for verifying the validity of the pointer

* `next / prev` : Doubly linked list pointers for block traversal

* `size` : Size of the data section

* `ptr` : Pointer to the usable memory (points to data[])

* `data[]` : Start of the memory returned to the user

# Function Overview
#### `void *malloc(size_t size)` 
Allocates memory of the requested size, rounded up to the nearest multiple of 8 for alignment. If no suitable free block is found, the heap is extended.

#### `void *calloc(size_t n_elements, size_t s_elements)` 
Allocates and zeroes memory for an array of elements. Uses malloc under the hood and then initializes memory to 0.

#### `void *realloc(void *ptr, size_t size)` 
Changes the size of an allocated block:
If new size is smaller, it trims the block.
If larger, allocates a new block, copies old data, and marks the old block as free.

#### `int free(void *ptr)` 
Marks a memory block as free for future use. If the pointer is invalid or the block is already free, it returns 1. Otherwise, returns 0.

# Internal Helpers
#### `meta extend_heap(size_t size)` 
Uses `sbrk()` to grow the heap and creates a new memory block with associated metadata.

#### `meta find_suitable_block(size_t size)` 
Implements the first-fit strategy to find a free block large enough to accommodate the requested size.

#### `void split_block(meta block)` 
If a block is significantly larger than required, it is split into two blocks to reduce wasted space.

#### `meta validate_address(void *ptr)` 
Checks if a pointer refers to a valid allocated block by comparing its magic number.

# Limitations
* No coalescing of adjacent free blocks (leads to fragmentation)

* No thread safety (not suitable for multithreaded environments)

* Does not return memory to the OS

* Simple first-fit strategy may lead to inefficiencies over time

# To-Do / Improvements
* Add block merging (coalescing) on free

* Implement a more advanced allocation strategy (e.g., best-fit)

* Support alignment constraints

* Add thread safety (e.g., using pthread_mutex)

* Implement memory shrinking or release back to OS
