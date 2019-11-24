#ifndef __SUP_SP_TILES__
#define __SUP_SP_TILES__

#include "../data/rebirth.h"
#include "../data/attack.h"
#include "../data/boss_tile.h"
#include "../data/compact_arr_tile.h"
#include "../data/money_tile.h"
#include "../data/retreat.h"
#include "../data/hp_mod.h"
#include "../data/torpedo.h"
#include "../data/armor.h"
// #include "../data/armor.h"
#include "../data/repair_mod.h"
#include "../data/special.h"
#include "../data/ship_tile.h"
#include "../data/heal.h"
#include "../data/upgrade.h"
#include "../data/skip_node_tile.h"
#include "../data/easy_tile.h"
#include "../data/norm_tile.h"
#include "../data/hard_tile.h"
#include "../data/spy_tile.h"

#include "base_sp1.c"

#define COMPACT_ARR_TILES 128 // compact_arr

#define NO_OUT 2

#define ALL_CON_TOP 128
#define ALL_CON_BOT 129
#define NO_MID_TOP 132
#define NO_MID_BOT 133
#define ONLY_MID_TOP 136
#define ONLY_MID_BOT 137
#define NO_TOP 140
#define NO_BOT 141

#define ATTACK_TILES (144+4*0) // attack action and attack mod attack.png 
#define BOSS_NODE_TILES (144+4*1) // node boss.png
#define CREDIT_TILES (144+4*2)    // money and shop credit.png
#define RETREAT_TILES (144+4*3) // retreat action cycle.png
#define HP_MOD_TILES (144+4*4) // hp mode forward-field.png
#define NORM_NODE_TILES (144+4*5) // norm node morbid-humour.png
#define EASY_NODE_TILES (144+4*6) // easy node piece-skull.png
#define HARD_NODE_TILES (144+4*7) // hard node pirate-skull
#define TORPEDO_MOD_TILES (144+4*8) // torpedo mod rocket.png
#define ARMOR_MOD_TILES (144+4*9) // armor mod shield.png
#define ADD_SHIP_TILES (144+4*10) // add ship bonus spaceship.png 
#define SUPPORT_MOD_TILES (144+4*11) // support mod spanner.png 
#define SPECIAL_TILES (144+4*12) // special action special.png
#define UPGRADE_TILES (144+4*13) // upgrade bonus upgrade.png
#define REBIRTH_MOD_TILES (144+4*14) // rebirth mod tiles angel-wings.png
#define HEAL_TILES (144+4*15) // heal all bonus techno-heart.png
#define SKIP_NODE_TILES (144+4*16) // heal all bonus techno-heart.png
#define SPY_TILES (144+4*17) // heal all bonus techno-heart.png

#define LEFT_TOP_TILE 0
#define RIGHT_TOP_TILE 2
#define LEFT_BOT_TILE 1
#define RIGHT_BOT_TILE 3

uint16_t tiles_for_bonus[] = {
    ATTACK_TILES, 
    HP_MOD_TILES,
    SPECIAL_TILES,
    TORPEDO_MOD_TILES,
    SUPPORT_MOD_TILES,
    ARMOR_MOD_TILES,
    REBIRTH_MOD_TILES,
    UPGRADE_TILES,
    ADD_SHIP_TILES,
    HEAL_TILES
};

void init_tileset(uint8_t *tileset, uint16_t offset, char size) {
    char i;
    for (i = 0; i < size; i++) {
        sp1_TileEntry(offset + i, tileset + i*8);
    }
}

void init_all_tilesets() {
    // options and mods
    init_tileset(attack_ic, ATTACK_TILES, 4);
    init_tileset(special_ic, SPECIAL_TILES, 4);
    // options
    init_tileset(retreat_ic, RETREAT_TILES, 4);
    init_tileset(spy_ic, SPY_TILES, 4);

    // bonus and node
    init_tileset(money_ic, CREDIT_TILES, 4);

    // nodes
    init_tileset(skip_node, SKIP_NODE_TILES, 4);
    init_tileset(boss, BOSS_NODE_TILES, 4);
    init_tileset(easy, EASY_NODE_TILES, 4);
    init_tileset(norm, NORM_NODE_TILES, 4);
    init_tileset(hard, HARD_NODE_TILES, 4);
    init_tileset(compact_arr, COMPACT_ARR_TILES, 16);

    // mods and bonuses
    init_tileset(torpedo_ic, TORPEDO_MOD_TILES, 4);
    init_tileset(armor_ic, ARMOR_MOD_TILES, 4);
    init_tileset(rebirth_ic, REBIRTH_MOD_TILES, 4);
    init_tileset(hp_mod_ic, HP_MOD_TILES, 4);
    init_tileset(repair_mod_ic, SUPPORT_MOD_TILES, 4);

    // bonuses
    init_tileset(upgrade_ic, UPGRADE_TILES, 4);
    init_tileset(ship_ic, ADD_SHIP_TILES, 4);
    init_tileset(heal_ic, HEAL_TILES, 4);
}

void print_big_at_inv(uint16_t row, uint16_t col, uint16_t colour, uint16_t tile) {
    sp1_PrintAtInv(row, col, colour, tile + LEFT_TOP_TILE);
    sp1_PrintAtInv(row, col + 1, colour, tile + RIGHT_TOP_TILE);
    sp1_PrintAtInv(row + 1, col, colour, tile + LEFT_BOT_TILE);
    sp1_PrintAtInv(row + 1, col + 1, colour, tile + RIGHT_BOT_TILE);
}

void print_big_empty(uint16_t row, uint16_t col, uint16_t colour) {
    sp1_PrintAtInv(row, col, colour, ' ');
    sp1_PrintAtInv(row, col + 1, colour, ' ');
    sp1_PrintAtInv(row + 1, col, colour, ' ');
    sp1_PrintAtInv(row + 1, col + 1, colour, ' ');
}

#endif