#include "vector.h"

Vector* Vector__new(size_t initial_capacity) {
  Vector* ca = malloc(sizeof(Vector));
  ca->items = malloc(initial_capacity * sizeof(void*));
  ca->size = 0;
  ca->capacity = initial_capacity;
  return ca;
}

void Vector__push(Vector* self, void* element) {
  if (self->size == self->capacity) {
    self->capacity *= 2;
    self->items = realloc(self->items, self->capacity * sizeof(void*));
  }
  self->items[self->size++] = element;
}

void Vector__pop(Vector* self) {
  self->items[--self->size] = NULL;
  }

void* Vector__last(Vector* self) {
    return self->items[self->size-1];
}

Vector* Vector__copy(Vector* self) {
    Vector* new = malloc(sizeof(Vector));
    new->items = malloc(self->capacity * sizeof(void*));
    new->size = self->size;
    new->capacity = self->capacity;
    for (int i = 0; i < self->size; i++) {
        new->items[i] = self->items[i];
    }
    return new;
}

void Vector__delete(Vector* self, int index) {
    self->items[index] = NULL;
    for (int i = index; i < self->size-1; i++) {
        self->items[i] = self->items[i+1];
        self->items[i+1] = NULL;
    }
    self->size--;
}

void Vector__clear(Vector* self) {
    for(int i = 0; i < self->size; i++) {
        self->items[i] = NULL;
    }
    self->size = 0;
}