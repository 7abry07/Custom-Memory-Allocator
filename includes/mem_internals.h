#ifndef MEM_INTERNALS_H
#define MEM_INTERNALS_H

#include "types.h"
#include <stddef.h>
#include <unistd.h>

#define MAGIC_NUM 1234567890
#define round_to_8(x) ((x + 7) >> 3) << 3;

typedef struct meta_block *meta;

struct meta_block {
  i8 b_factor;
  u8 free;
  u32 magic_number;
  meta next;
  meta prev;
  meta left;
  meta right;
  size_t size;
  void *ptr;
  char data[];
};

int isEmpty();
void add_to_tail(meta block);
void split_block(meta block);
meta extend_heap(size_t size);
meta find_suitable_block(size_t size);
meta validate_address(void *pointer);

#endif
