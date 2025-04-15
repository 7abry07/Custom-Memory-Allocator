#include "../includes/mem.h"
#include <stdio.h>

//feel free to modify this file however you want for debugging purpose
//this is just a normal test file
int main() {
  int *num1 = (int *)(malloc(sizeof(int) * 5)); //allocates size for 5 integers
  free(num1); //frees memory

  int *num2 = (int *)(calloc(5, sizeof(int))); //allocates size for an array of "5" elements of "int" size
  int *num3 = (int *)(realloc(num2, sizeof(int) * 10)); //reallocates memory from holding "5" integers to holding "10"

  free(num3); //frees memory
}
