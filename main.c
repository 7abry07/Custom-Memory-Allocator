#include "includes/mem.h"
#include <stdio.h>

int main() {
  int *num1 = (int *)(malloc(sizeof(int) * 5)); //allocates size for 5 integers
  free(num1); //frees memory

  int *num2 = (int *)(calloc(5, sizeof(int))); //allocates size for an array of "5" elements of "int" size
  int *num3 = (int *)(realloc(num2, sizeof(int) * 10)); //reallocates memory from holding "5" integers to holding "10"

  free(num2); //frees memory
  free(num3); //frees memory
}
