#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "player.h"
#include "table.h"
#include "vector.h"

const int max_splits = 10;
int player_num_count = 0;

Player* Player__new(Table* table, Player* split) {
    Player* p = malloc(sizeof(Player));
    p->m_table = table;
    if(table == NULL) return p;
    p->m_initial_bet = table->m_bet_size;
    if (split != NULL) {
        Vector__push(p->m_hand, split->m_hand->items[1]);
        p->m_split_count++;
        p->m_player_num = split->m_player_num;
        sprintf(p->m_player_num, "%sS", split->m_player_num);
        p->m_split_from = split;
    } else {
        player_num_count++;
        char str[2];
        _itoa(player_num_count, str,10);
        p->m_player_num = str;
        printf("Player %s", p->m_player_num);
    }
    return p;
} 
  
