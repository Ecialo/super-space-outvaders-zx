#ifndef __SUP_SP_TILES__
#define __SUP_SP_TILES__

// #include "../data/rebirth.h"
// #include "../data/attack.h"
// #include "../data/boss_tile.h"
// #include "../data/compact_arr_tile.h"
// #include "../data/money_tile.h"
// #include "../data/retreat.h"
// #include "../data/hp_mod.h"
// #include "../data/torpedo.h"
// #include "../data/armor.h"
// #include "../data/armor.h"
// #include "../data/repair_mod.h"
// #include "../data/special.h"
// #include "../data/ship_tile.h"
// #include "../data/heal.h"
// #include "../data/upgrade.h"
// #include "../data/skip_node_tile.h"
// #include "../data/easy_tile.h"
// #include "../data/norm_tile.h"
// #include "../data/hard_tile.h"
// #include "../data/spy_tile.h"
#include "../data/star_tile.h"
#include "ship.c"

#include "types.h"
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

#define SHIP_TILES_C (144 + 4*18 + 6)
// #define I2 (216 + 9*1)
// #define B1 (216 + 9*3)
// #define B2 (216 + 9*4)
// #define S1 (216 + 9*5)
// #define S2 (216 + 9*6)
// #define D1 (216 + 9*7)
// #define D2 (216 + 9*8)
// #define BOSS1 (216 + 9*9)
// #define BOSS2 (216 + 9*10)

#define STAR_TILES (144 + 4*18)


#define LEFT_TOP_TILE 0
#define RIGHT_TOP_TILE 2
#define LEFT_BOT_TILE 1
#define RIGHT_BOT_TILE 3

extern uch rebirth_ic[];
extern uch attack_ic[];
extern uch boss[];
extern uch money_ic[];
extern uch compact_arr[];
extern uch retreat_ic[];
extern uch hp_mod_ic[];
extern uch torpedo_ic[];
extern uch armor_ic[];
extern uch repair_mod_ic[];
extern uch special_ic[];
extern uch ship_ic[];
extern uch heal_ic[];
extern uch upgrade_ic[];
extern uch skip_node[];
extern uch easy[];
extern uch norm[];
extern uch hard[];
extern uch spy_ic[];

extern unsigned char interceptor_1[];
extern unsigned char interceptor_2[];
extern unsigned char bomber_1[];
extern unsigned char bomber_2[];
extern unsigned char support_1[];
extern unsigned char support_2[];
extern unsigned char destroyer_1[];
extern unsigned char destroyer_2[];
extern unsigned char boss_1[];
extern unsigned char boss_2[];

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

void init_tileset(uint8_t *tileset, uint16_t offset, uch size) {
    uch i;
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

    // init_tileset(interceptor_1, I1, 9);
    // init_tileset(interceptor_2, I2, 9);
    // init_tileset(bomber_1, B1, 9);
    // init_tileset(bomber_2, B2, 9);
    // init_tileset(destroyer_1, D1, 9);
    // init_tileset(destroyer_2, D2, 9);
    // init_tileset(support_1, S1, 9);
    // init_tileset(support_2, S2, 9);
    // init_tileset(boss_1, BOSS1, 9);
    // init_tileset(boss_2, BOSS2, 9);

    init_tileset(stars, STAR_TILES, STAR_TILE_TYPES);
}

void print_big_at_inv(uint16_t row, uint16_t col, uint16_t colour, uint16_t tile) {
    sp1_PrintAtInv(row, col, colour, tile + LEFT_TOP_TILE);
    sp1_PrintAtInv(row, col + 1, colour, tile + RIGHT_TOP_TILE);
    sp1_PrintAtInv(row + 1, col, colour, tile + LEFT_BOT_TILE);
    sp1_PrintAtInv(row + 1, col + 1, colour, tile + RIGHT_BOT_TILE);
}


extern uch flip_buffer[];
uch* flip(uch *sprite) {
    //  uch i, column, column_offset, j;
    //  for (column = 0; column < 3; column++) {
    //      column_offset = column * 24;
    //      for (i = 0; i < 24; i++) {
    //          j = column_offset + 24 - i;
    //          flip_buffer[column_offset + i] = sprite[j - 1];
    //      }
    //  }
    // return flip_buffer;
    return sprite;
}


uch *last_ship = NULL;
void print_ship_at_inv(uint16_t row, uint16_t col, ship *ship, uch is_flash, uch is_flip) {
    uch i, j, tier, color;
    uch *ship_sp;
    tier = ship->tier;
    switch (ship->type) {
        case INTERCEPTOR:
            ship_sp = (tier == 2) ? interceptor_2 : interceptor_1;
            color = INTERCEPTOR_COLOR;
            break;
        case BOMBER:
            ship_sp = (tier == 2) ? bomber_2 : bomber_1;
            color = BOMBER_COLOR;
            break;
        case DESTROYER:
            ship_sp = (tier == 2) ? destroyer_2 : destroyer_1;
            color = DESTROYER_COLOR;
            break;
        case SUPPORT:
            ship_sp = (tier == 2) ? support_2 : support_1;
            color = SUPPORT_COLOR;
            break;
        case BOSS:
            ship_sp = (tier == 2) ? boss_2 : boss_1;
            color = BOSS_COLOR;
        default:
            break;
    }
    if (is_flash) {
        color = FLASH_COLOR;
    }
    if (is_flip) {
        ship_sp = flip(ship_sp);
    } 
    // if (ship_sp != last_ship) {
    //     init_tileset(
    //         ship_sp,
    //         SHIP_TILES_C,
    //         9
    //     );
    //     last_ship = ship_sp;
    // }
    for (i=0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            sp1_PrintAtInv(row + j, col + i, color, (uint16_t)(ship_sp + i*3 + j));
        }
    }
    // sp1_UpdateNow();
}

// uch *actual_ship;
// uch actual_color;
// uch cx, cy;
// void print_ship(struct sp1_update *u)
// {
//    unsigned char *p;

//    // locate character square on screen

//    p = zx_cxy2saddr(csx, csy);

//    // store graphic location and colour in struct update

//    u->tile = (unsigned int)csdst;
//    u->colour = *zx_saddr2aaddr(p);

//    // copy screen graphics to screen buffer

//    // next character square

//    if (++csx == 3)
//    {
//       csx = 0;
//       ++csy;
//    }
// }

void print_big_empty(uint16_t row, uint16_t col, uint16_t colour) {
    sp1_PrintAtInv(row, col, colour, ' ');
    sp1_PrintAtInv(row, col + 1, colour, ' ');
    sp1_PrintAtInv(row + 1, col, colour, ' ');
    sp1_PrintAtInv(row + 1, col + 1, colour, ' ');
}

#endif