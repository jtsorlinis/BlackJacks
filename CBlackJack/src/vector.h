#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include <string.h>

typedef struct Vector {
  void** items;
  int size;
  int capacity;
} Vector;

static inline void Vector__init(Vector* self, size_t initial_capacity) {
  self->items = malloc(initial_capacity * sizeof(void*));
  self->size = 0;
  self->capacity = (int)initial_capacity;
}

static inline void Vector__resize(Vector* self, int capacity) {
  void** temp = realloc(self->items, sizeof(void*) * capacity);
  if (temp) {
    self->items = temp;
    self->capacity = capacity;
  }
}

static inline void Vector__push(Vector* self, void* element) {
  if (self->size == self->capacity) {
    int new_capacity = self->capacity > 0 ? self->capacity * 2 : 2;
    Vector__resize(self, new_capacity);
  }
  self->items[self->size++] = element;
}

static inline void Vector__pop(Vector* self) { --self->size; }

static inline void* Vector__last(Vector* self) {
  return self->items[self->size - 1];
}

static inline void Vector__copy(Vector* self, Vector* target) {
  if (target->capacity < self->size) {
    void** temp = realloc(target->items, self->size * sizeof(void*));
    if (!temp) return;
    target->items = temp;
    target->capacity = self->size;
  }
  target->size = self->size;
  memcpy(target->items, self->items, self->size * sizeof(void*));
}

static inline void Vector__delete(Vector* self, int index) {
  memmove(&self->items[index], &self->items[index + 1],
          (self->size - index - 1) * sizeof(void*));
  --self->size;
}

static inline void Vector__insert(Vector* self, void* element, int index) {
  if (self->size == self->capacity) {
    Vector__resize(self, self->capacity + 2);
  }

  memmove(&self->items[index + 1], &self->items[index],
          (self->size - index) * sizeof(void*));

  self->size++;
  self->items[index] = element;
}

static inline void Vector__clear(Vector* self) { self->size = 0; }

static inline void Vector__destroy(Vector* self) { free(self->items); }

#endif
