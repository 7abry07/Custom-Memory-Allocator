#ifndef TREE_H
#define TREE_H

#include "mem_internals.h"

void add(meta node);
meta best_fit();

void RR(meta node);
void RL(meta node);
void LL(meta node);
void LR(meta node);

#endif
