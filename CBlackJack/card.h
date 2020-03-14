#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int m_count;
  bool m_is_ace;
  char* m_rank;
  int m_value;
  char* m_suit;
  bool m_face_down;
} Card;

Card* Card__new(char* rank, char* suit);
char* Card__print(Card* const self);
int Card__evaluate(Card* self);
int Card__count(Card* self);
