#include "vector.h"

#include <string.h>

Vector* Vector__new(size_t initial_capacity) {
  Vector* ca = malloc(sizeof(Vector));
  ca->items = malloc(initial_capacity * sizeof(void*));
  ca->size = 0;
  ca->capacity = initial_capacity;
  return ca;
}

void Vector__resize(Vector* self, int capacity) {
  void** temp = realloc(self->items, sizeof(void*) * capacity);
  if (temp) {
    self->items = temp;
    self->capacity = capacity;
  }
}

void Vector__push(Vector* self, void* element) {
  // Make room if we need to for new item
  if (self->size == self->capacity) {
    Vector__resize(self, self->capacity + 2);
  }
  self->items[self->size++] = element;
}

void Vector__pop(Vector* self) { --self->size; }

void* Vector__last(Vector* self) { return self->items[self->size - 1]; }

Vector* Vector__copy(Vector* self) {
  Vector* new = malloc(sizeof(Vector));
  new->items = malloc(self->capacity * sizeof(void*));
  new->size = self->size;
  new->capacity = self->capacity;
  memcpy(new->items, self->items, self->size * sizeof(void*));
  return new;
}

void Vector__delete(Vector* self, int index) {
  memmove(&self->items[index], &self->items[index + 1],
          (self->size - index - 1) * sizeof(void*));
  --self->size;
}

void Vector__insert(Vector* self, void* element, int index) {
  // Make room if we need to for new item
  if (self->size == self->capacity) {
    Vector__resize(self, self->capacity + 2);
  }

  // Shift all elements right after insertion point
  for (int i = self->size; i > index; i--) {
    self->items[i] = self->items[i - 1];
  }

  // Increase the size
  self->size++;

  // Finally, insert the element
  self->items[index] = element;
}

void Vector__clear(Vector* self) { self->size = 0; }

void Vector__free(Vector* self) {
  free(self->items);
  free(self);
}