// #include "Vector.h"

typedef struct Vector Vector;

typedef struct {
    Vector* m_cards;
    Vector* m_original_cards;
} CardPile;

CardPile* CardPile__new(int numdecks);
void CardPile__refresh(CardPile* self);
void CardPile__print(CardPile* self);
void CardPile__shuffle(CardPile* self);
