#include <stddef.h>
#include <unistd.h>

// useful macros
#define MAGIC_NUM 1234567890
#define round_to_8(x) ((x + 7) >> 3) << 3; // rouds the given number to the nearest biggest multiple of 8

// utility typedef
typedef struct meta_block *meta;

// head and tail of the list
meta head = NULL;
meta tail = NULL;

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

// adds the block to the end of the list
void add_to_tail(meta block)
{
  if (!head)
  {
    head = block;
    tail = head;
  }
  else
  {
    tail->next = block;
    block->prev = tail;
    tail = block;
  }
}

// splits the memory block in half if its too large and
// appends the newly created block at the end of the list
void split_block(meta block)
{
  size_t current_size = block->size;
  size_t new_size = (current_size / 2) - (sizeof(struct meta_block) / 2);
  meta new_block = (meta)((char *)block + new_size + sizeof(struct meta_block));

  new_block->free = 1;
  new_block->next = NULL;
  new_block->prev = NULL;
  new_block->size = current_size - new_size - sizeof(struct meta_block);
  new_block->magic_number = MAGIC_NUM;
  new_block->ptr = new_block->data;

  block->size = new_size;

  add_to_tail(new_block);
}

// extends the heap memory of "size" and
// creates a block that gets added at the end of the list
// if "sbrk" fails it returns NULL
meta extend_heap(size_t size)
{
  meta old_break = sbrk(0);
  meta new_break = sbrk(size + sizeof(struct meta_block));

  meta new = sbrk(0);

  if (new_break == (void *)-1)
    return NULL;

  old_break->free = 0;
  old_break->next = NULL;
  old_break->prev = NULL;
  old_break->size = size;
  old_break->magic_number = 1234567890;
  old_break->ptr = old_break->data;

  add_to_tail(old_break);

  return old_break;
}

// tries to find a suitable memory block
// for the given size using the first-fit policy
// if it doesnt find any it return NULL
meta find_suitable_block(size_t size)
{
  meta p = head;
  meta block = NULL;
  size_t s = 0;
  int i = 0;

  while (p)
  {
    if (!(p->free && p->size > size))
    {
      p = p->next;
      continue;
    }

    if (!i) {
      s = p->size;
      block = p;
      i++;
    }
    if (p->size < s)
    {
      s = p->size;
      block = p;
    }
    p = p->next;
  }
  return block;
}

// checks if the given address is valid (allocated by malloc, calloc or realloc)
// if it is it returns a pointer to the start of the memory block
// if its not it returns NULL
meta validate_address(void *pointer)
{
  meta block = pointer - sizeof(struct meta_block);
  if (block->magic_number != MAGIC_NUM)
    return NULL;
  return block;
}

// allocates "size" memory on the heap
// and returns a pointer to that memory
// if it fails it returns NULL
void *malloc(size_t s)
{
  if (!s)
    return NULL;

  meta block;
  size_t size = round_to_8(s);

  if (!head)
  {
    block = extend_heap(size);
    if (!block)
      return NULL;
  }
  else
  {
    block = find_suitable_block(size);
    if (!block)
    {
      block = extend_heap(size);
      if (!block)
        return NULL;
    }
    else if (block)
      block->free = 0;
    if (block->size > 2 * size && block->size > sizeof(struct meta_block))
      split_block(block);
  }
  return block->ptr;
}

// it changes the size of the memory
// pointed from pointer to "size"
// (passing 0 to "size" results in the memory being freed
// retaining it's original size)
void *realloc(void *pointer, size_t s)
{
  meta block = validate_address(pointer);
  size_t size = round_to_8(s);
  meta temp = block;

  if (!size)
  {
    block->free = 1;
    return block->ptr;
  }

  if (!block)
    return NULL;

  if (size < block->size)
    block->size = size;

  else if (size > block->size)
  {
    block->free = 1;

    block = (meta)(malloc(size));

    for (int i = 0; i < temp->size; i++)
      block->data[i] = temp->data[i];
  }
  return block->ptr;
}

void *calloc(size_t n_elements, size_t s_elements)
{
  size_t size = round_to_8(n_elements * s_elements);
  meta block = validate_address(malloc(size));

  for (int i = 0; i < size; i++)
    block->data[i] = 0;

  return block->ptr;
}

// frees memory where pointer points too
// returns 0 if the memory is successfully freed
// returns 1 if the memory was already free or
// if the address is invalid
// (accessing freed memory results in undefined behaviour)
int free(void *pointer)
{
  meta block = validate_address(pointer);
  if (block == NULL || block->free == 1)
    return 1;

  block->free = 1;
  return 0;
}
