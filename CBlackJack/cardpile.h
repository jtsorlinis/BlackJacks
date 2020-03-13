
typedef struct CardArray CardArray;

typedef struct {
    CardArray* m_cards;
    CardArray* m_original_cards;
} CardPile;

CardPile* CardPile__new(int numdecks);
void CardPile__refresh();
void CardPile__print();
void CardPile__shuffle();