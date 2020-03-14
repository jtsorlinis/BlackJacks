#include <stdbool.h>

typedef struct Table Table;
typedef struct Vector Vector;

typedef struct Player {
    Table* m_table;
    Vector* m_hand;
    struct Player* m_split_from;
    int m_aces;
    int m_split_count;
    float m_bet_mult;
    float m_earnings;
    bool m_has_natural;
    int m_initial_bet;
    bool m_is_done;
    bool m_is_soft;

    char* m_player_num;
    int m_value;
} Player;

Player* Player__new(Table* table, Player* split);