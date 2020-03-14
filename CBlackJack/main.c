#include <stdio.h>
#include <time.h>
#include <stdbool.h>

#include "table.h"
#include "vector.h"

int main(int argc, char const *argv[])
{

  clock_t start = clock();

  Table *t = Table__new(5, 8, 10, 40, true);

  Table__start_round(t);
  Table__split(t);
  Table__clear(t);
  Table__print(t);
  

  printf("Took %f seconds", (double)(clock() - start) / CLOCKS_PER_SEC);

  return 0;
}
