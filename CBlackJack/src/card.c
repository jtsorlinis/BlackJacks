#include "card.h"

Card* Card__new(char* rank, char* suit) {
  Card* c = malloc(sizeof(Card));
  c->m_rank = rank;
  c->m_suit = suit;
  c->m_value = Card__evaluate(c);
  c->m_count = Card__count(c);
  c->m_is_ace = strcmp("A", rank) == 0;
  return c;
}

char* Card__print(Card* const self) {
  return self->m_face_down ? "X" : self->m_rank;
}

int Card__evaluate(Card* self) {
  if (strcmp(self->m_rank, "J") == 0 || strcmp(self->m_rank, "Q") == 0 ||
      strcmp(self->m_rank, "K") == 0) {
    return 10;
  }
  if (strcmp(self->m_rank, "A") == 0) {
    return 11;
  }
  return atoi(self->m_rank);
}

int Card__count(Card* self) {
  if (strcmp(self->m_rank, "10") == 0 || strcmp(self->m_rank, "J") == 0 ||
      strcmp(self->m_rank, "Q") == 0 || strcmp(self->m_rank, "K") == 0 ||
      strcmp(self->m_rank, "A") == 0) {
    return -1;
  }
  if (strcmp(self->m_rank, "7") == 0 || strcmp(self->m_rank, "8") == 0 ||
      strcmp(self->m_rank, "9") == 0) {
    return 0;
  }
  return 1;
}