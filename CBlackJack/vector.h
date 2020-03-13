#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>

typedef struct Vector {
  void** array;
  int size;
  int capacity;
} Vector;

Vector* Vector__new(size_t initial_capacity);
void Vector__push(Vector* self, void* element);
void* Vector__pop(Vector* self);
Vector* Vector__copy(Vector* self);

#endif