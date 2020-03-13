#include <stdlib.h>
#include <time.h>
#include <stdio.h>

typedef struct Vector {
  void** array;
  int size;
  int capacity;
} Vector;

Vector* Vector__new(size_t initial_capacity) {
  Vector* ca = malloc(sizeof(Vector));
  ca->array = malloc(initial_capacity * sizeof(void*));
  ca->size = 0;
  ca->capacity = initial_capacity;
  return ca;
}

void Vector__push(Vector* self, void* element) {
  if (self->size == self->capacity) {
    self->capacity *= 2;
    self->array = realloc(self->array, self->capacity * sizeof(void*));
  }
  self->array[self->size++] = element;
}

void* Vector__pop(Vector* self) { return self->array[self->size--]; }

Vector* Vector__copy(Vector* self) {
    Vector* new = malloc(sizeof(Vector));
    new->array = malloc(self->capacity * sizeof(void*));
    new->size = self->size;
    new->capacity = self->capacity;
    for (int i = 0; i < self->size; i++) {
        new->array[i] = self->array[i];
    }
    return new;
}