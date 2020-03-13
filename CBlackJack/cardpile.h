// #include "cardarray.h"

typedef struct CardArray CardArray;

typedef struct {
    CardArray* m_cards;
    CardArray* m_original_cards;
} CardPile;

CardPile* CardPile__new(int numdecks);
void CardPile__refresh(CardPile* self);
void CardPile__print(CardPile* self);
void CardPile__shuffle(CardPile* self);
