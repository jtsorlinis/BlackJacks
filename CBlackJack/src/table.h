#include <stdbool.h>

typedef struct CardPile CardPile;
typedef struct Dealer Dealer;
typedef struct Player Player;
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
    int m_true_count;
    float m_casino_earnings;

    char* m_strat_hard;
    char* m_strat_soft;
    char* m_strat_split;
} Table;

Table* Table__new(int num_players, int num_decks, int bet_size, int min_cards, int verbose);
void Table__deal_round(Table* self);
void Table__evaluate_all(Table* self);
void Table__deal(Table* self);
void Table__pre_deal(Table* self);
void Table__select_bet(Table* self, Player* player);
void Table__deal_dealer(Table* self, bool face_down);
void Table__start_round(Table* self);
void Table__get_new_cards(Table* self);
void Table__clear(Table* self);
void Table__update_count(Table* self);
void Table__hit(Table* self);
void Table__stand(Table* self);
void Table__split(Table* self);
void Table__split_aces(Table* self);
void Table__double_bet(Table* self);
void Table__auto_play(Table* self);
void Table__action(Table* self, char action);
void Table__dealer_play(Table* self);
void Table__next_player(Table* self);
void Table__check_player_natural(Table* self);
bool Table__check_dealer_natural(Table* self);
void Table__check_earnings(Table* self);
void Table__finish_round(Table* self);
void Table__print(Table* self);
