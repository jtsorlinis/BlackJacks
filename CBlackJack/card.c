#include "card.h"

Card* Card__new(char* rank, char* suit) {
  Card* c = malloc(sizeof(Card));
  c->m_rank = rank;
  c->m_suit = suit;
  c->m_value = Card__evaluate(c);
  c->m_count = Card__count(c);
  return c;
}

char* Card__print(Card* const self) { return self->m_rank; }

int Card__evaluate(Card* self) {
  if (strcmp(self->m_rank, "J") == 0 || strcmp(self->m_rank, "Q") == 0 ||
      strcmp(self->m_rank, "K") == 0) {
    return 10;
  } else if (strcmp(self->m_rank, "A") == 0) {
    return 11;
  } else {
    return atoi(self->m_rank);
  }
}

int Card__count(Card* self) {
  if (strcmp(self->m_rank, "10") == 0 || strcmp(self->m_rank, "J") == 0 ||
      strcmp(self->m_rank, "Q") == 0 || strcmp(self->m_rank, "K") == 0 ||
      strcmp(self->m_rank, "A") == 0) {
    return -1;
  } else if (strcmp(self->m_rank, "7") == 0 || strcmp(self->m_rank, "8") == 0 ||
             strcmp(self->m_rank, "9") == 0) {
    return 0;
  } else {
    return 1;
  }
}