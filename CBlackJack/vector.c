#include "vector.h"

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

void* Vector__pop(Vector* self) {
  self->array[self->size] = NULL;
  return self->array[self->size--]; 
  }

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

void Vector__delete(Vector* self, int index) {
    self->array[index] = NULL;
    for (int i = index; i < self->size-1; i++) {
        self->array[i] = self->array[i+1];
        self->array[i+1] = NULL;
    }
    self->size--;
}