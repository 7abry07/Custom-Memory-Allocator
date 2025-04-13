#include "includes/mem.h"

int main() {
  int *num1 = (int *)(calloc(5, sizeof(int)));

  for (int i = 0; i < 5; i++){
    printf("%d", num1[i]);
  }
}
