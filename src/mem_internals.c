#include "../includes/mem_internals.h"

meta head = NULL;
meta tail = NULL;

int isEmpty(){
    return head == NULL;
}

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

void split_block(meta block)
{
  size_t current_size = block->size;
  size_t new_size = (current_size / 2) - (sizeof(struct meta_block) / 2);
  meta new_block = (meta)((char *)block + new_size + sizeof(struct meta_block));

  new_block->free = 1;
  new_block->next = NULL;
  new_block->prev = NULL;
  new_block->size = new_size;
  new_block->magic_number = MAGIC_NUM;
  new_block->ptr = new_block->data;

  block->size = new_size;

  add_to_tail(new_block);
}

meta extend_heap(size_t size)
{
  meta old_break = sbrk(0);
  meta new_break = sbrk(size + sizeof(struct meta_block));

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

meta validate_address(void *pointer)
{
  meta block = pointer - sizeof(struct meta_block);
  if (block->magic_number != MAGIC_NUM)
    return NULL;
  return block;
}
