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

void Vector__copy(Vector* self, Vector* target) {
  target->items = realloc(target->items, self->capacity * sizeof(void*));
  target->size = self->size;
  target->capacity = self->capacity;
  memcpy(target->items, self->items, self->size * sizeof(void*));
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
  memmove(&self->items[index + 1], &self->items[index],
          (self->size - index) * sizeof(void*));

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