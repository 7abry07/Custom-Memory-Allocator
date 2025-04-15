#include "../includes/mem_internals.h"

void *malloc(size_t s)
{
  if (!s)
    return NULL;

  meta block;
  size_t size = round_to_8(s);

  if (isEmpty())
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

void *realloc(void *pointer, size_t s)
{
  meta block = validate_address(pointer);
  if (!block)
    return NULL;
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
    block = validate_address((malloc(size)));

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

int free(void *pointer)
{
  meta block = validate_address(pointer);
  if (block == NULL || block->free == 1)
    return 1;

  block->free = 1;
  return 0;
}
