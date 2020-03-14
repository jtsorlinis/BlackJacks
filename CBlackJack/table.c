#include "table.h"
#include <stdlib.h>

#include "cardpile.h"
#include "vector.h"
#include "player.h"
#include "dealer.h"

Table* Table__new(int num_players, int num_decks, int bet_size, int min_cards, int verbose) {
    Table* t = malloc(sizeof(Table));
    t->m_card_pile = CardPile__new(num_decks);
    t->m_verbose = verbose;
    t->m_bet_size = bet_size;
    t->m_num_decks = num_decks;
    t->m_min_cards = min_cards;
    t->m_casino_earnings = 0;

    t->m_players = Vector__new(num_players);
    for(int i = 0; i < num_players; i++) {
        Vector__push(t->m_players, Player__new(t, NULL));
    }
    t->m_dealer = Dealer__new();

    return t;
}

void Table__print(Table* self) {
    for(int i = 0; i < self->m_players->size; i++) {
        Player__print(self->m_players->items[i]);
    }
    Dealer__print(self->m_dealer);
}