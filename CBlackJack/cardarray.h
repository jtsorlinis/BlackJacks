#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "card.h"

unsigned int seed;

unsigned int xorshift() {
  seed ^= seed << 13;
  seed ^= seed >> 17;
  seed ^= seed << 5;
  return seed;
}

typedef struct CardArray {
  Card** array;
  int size;
  int capacity;
} CardArray;

CardArray* CardArray__new(size_t initial_capacity) {
  seed = time(NULL);
  CardArray* ca = malloc(sizeof(CardArray));
  ca->array = malloc(initial_capacity * sizeof(Card*));
  ca->size = 0;
  ca->capacity = initial_capacity;
  return ca;
}

void CardArray__push(CardArray* self, Card* element) {
  if (self->size == self->capacity) {
    self->capacity *= 2;
    self->array = realloc(self->array, self->capacity * sizeof(Card*));
  }
  self->array[self->size++] = element;
}

Card* CardArray__pop(CardArray* self) { return self->array[self->size--]; }

CardArray* CardArray__copy(CardArray* self) {
    CardArray* new = malloc(sizeof(CardArray*));
    new->array = malloc(self->capacity * sizeof(Card*));
    new->size = self->size;
    new->capacity = self->capacity;
    for (int i = 0; i < self->size; i++) {
        new->array[i] = self->array[i];
    }
    return new;
}

void CardArray__print(CardArray* self) {
  for (int i = 0; i < self->size; i++) {
    printf("%s", self->array[i]->mRank);
  }
  printf("\n");
}

void CardArray__shuffle(CardArray* self) {
  for (int i = self->size - 1; i > 0; i--) {
    int j = xorshift() % (i + 1);
    Card* temp = self->array[i];
    self->array[i] = self->array[j];
    self->array[j] = temp;
  }
}