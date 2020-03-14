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

    char m_player_num[10];
    int m_value;
} Player;

Player* Player__new(Table* table, Player* split);
void Player__double_bet(Player* self);
void Player__reset_hand(Player* self);
int Player__can_split(Player* self);
void Player__win(Player* self, float mult);
void Player__lose(Player* self);
void Player__print(Player* self);
void Player__evaluate(Player* self);
void Player__free(Player* self);