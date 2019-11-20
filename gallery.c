#include <arch/zx.h>
#include <arch/zx/sp1.h>
#include <input.h>
#include <z80.h>               // for z80_bpoke(), z80_wpoke(), im2 functions
#include <string.h>            // for memset()
#include <stdlib.h>            // for rand()
#include <intrinsic.h>         // for intrinsic_di()
#include <sound.h>             // for bit_beepfx()

#include "src/ship.c"
#include "src/wing.c"
#include "src/icons.h"
#include "src/base_sp1.c"
#include "src/tiles.c"
// #include "src/draw_wing.c"
// #include "src/cursor.c"
// #include "src/map.c"


int main() {
    wing our, their;

    init_sp1();
    init_all_tilesets();
    
    // OPTIONS
    print_big_at_inv(0, 0, INK_WHITE | PAPER_BLACK, ATTACK_TILES);
    print_big_at_inv(0, 2, INK_WHITE | PAPER_BLACK, RETREAT_TILES);
    print_big_at_inv(0, 4, INK_WHITE | PAPER_BLACK, SPECIAL_TILES);
    // bonus and node
    print_big_at_inv(2, 0, INK_WHITE | PAPER_BLACK, CREDIT_TILES);
    // nodes
    print_big_at_inv(2, 2, INK_WHITE | PAPER_BLACK, SKIP_NODE_TILES);
    print_big_at_inv(2, 4, INK_WHITE | PAPER_BLACK, BOSS_NODE_TILES);
    print_big_at_inv(2, 6, INK_WHITE | PAPER_BLACK, EASY_NODE_TILES);
    print_big_at_inv(2, 8, INK_WHITE | PAPER_BLACK, NORM_NODE_TILES);
    print_big_at_inv(2, 10, INK_WHITE | PAPER_BLACK, HARD_NODE_TILES);
    // mods
    print_big_at_inv(4, 0, INK_WHITE | PAPER_BLACK, TORPEDO_MOD_TILES);
    print_big_at_inv(4, 2, INK_WHITE | PAPER_BLACK, ARMOR_MOD_TILES);
    print_big_at_inv(4, 4, INK_WHITE | PAPER_BLACK, REBIRTH_MOD_TILES);
    print_big_at_inv(4, 6, INK_WHITE | PAPER_BLACK, HP_MOD_TILES);
    print_big_at_inv(4, 8, INK_WHITE | PAPER_BLACK, SUPPORT_MOD_TILES);

    print_big_at_inv(6, 0, INK_WHITE | PAPER_BLACK, UPGRADE_TILES);
    print_big_at_inv(6, 2, INK_WHITE | PAPER_BLACK, ADD_SHIP_TILES);
    print_big_at_inv(6, 4, INK_WHITE | PAPER_BLACK, HEAL_TILES);
    

    sp1_UpdateNow();
    while(1);
    return 0;
}
