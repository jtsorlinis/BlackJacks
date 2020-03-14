typedef struct CardPile CardPile;
typedef struct Dealer Dealer;
typedef struct Vector Vector;

typedef struct Table {
    Dealer* m_dealer;
    int m_min_cards;
    int m_num_decks;
    int m_verbose;
    int m_bet_size;
    CardPile* m_card_pile;
    Vector* m_players;
    int m_current_player;
    int m_running_count;
    float m_casino_earnings;

    int m_strat_hard;
    int m_strat_soft;
    int m_strat_split;
} Table;

Table* Table__new(int num_players, int num_decks, int bet_size, int min_cards, int verbose);
void Table__print(Table* self);