#include <stdbool.h>

typedef struct Vector Vector;

typedef struct Dealer
{
    Vector* m_hand;
    char* m_player_num;
    int m_value;
    int m_aces;
    bool m_is_soft;
} Dealer;

Dealer* Dealer__new();
void Dealer__reset_hand(Dealer* self);
int Dealer__up_card(Dealer* self);
void Dealer__evaluate(Dealer* self);
void Dealer__print(Dealer* self);