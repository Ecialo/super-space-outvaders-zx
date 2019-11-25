#ifndef __SUP_SP_INSPECT__
#define __SUP_SP_INSPECT__

#include "ship.c"
#include "wing.c"
#include "base_sp1.c"
#include "tiles.c"

struct sp1_pss ps0;

char money_tile_not_ready = True;

void init_inspector() {
    ps0.bounds = &full_screen;
    ps0.flags = SP1_PSSFLAG_INVALIDATE;
    ps0.visit = 0;
}

void inspect_ship(ship *ship, struct sp1_Rect *rect) {
    char num_holder[3];
    char i, mods, x, y;
    num_holder[2] = '\0';
    x = rect->col;
    y = rect->row;

    sp1_ClearRectInv(rect, INK_RED | PAPER_WHITE, ' ', SP1_RFLAG_TILE | SP1_RFLAG_COLOUR);
    sp1_SetPrintPos(&ps0, y, x);

    // INIT
    sp1_PrintString(&ps0, "\x14\x50");

    // NAME
    sp1_PrintString(&ps0, ship->name);

    // TYPE

    // ATTACK
    sp1_SetPrintPos(&ps0, y + 1, x);
    to_string(ship->attack, num_holder);
    sp1_PrintString(&ps0, "AT");
        sp1_PrintString(&ps0, num_holder);

    // SPECIAL
    to_string(ship->special, num_holder);
    sp1_SetPrintPos(&ps0, y + 2, x);
    sp1_PrintString(&ps0, "SP"); sp1_PrintString(&ps0, num_holder);
    
    // HP
    to_string(ship->health, num_holder);
    sp1_SetPrintPos(&ps0, y + 1, x + 4);
    sp1_PrintString(&ps0, "HP"); 
        sp1_PrintString(&ps0, num_holder);
        to_string(ship->max_health, num_holder);
        sp1_PrintString(&ps0, "/");
        sp1_PrintString(&ps0, num_holder);

    // Mods
    mods = ship->mods;
    for (i = 0; i < 4; i++) {
        if (ALL_MODS[i] & mods) {
            print_big_at_inv(y + 3, x + i*3, INK_RED | PAPER_WHITE,  tiles_for_bonus[i]);
        }
    }
    for (i = 0; i < 3; i++) {
        if (ALL_MODS[i + 4] & mods) {
            print_big_at_inv(y + 5, x + 1 + i*3, INK_RED | PAPER_WHITE,  tiles_for_bonus[i + 4]);
        }
    }


}

void inspect_wing(wing *wing, struct sp1_Rect *wing_rect, struct sp1_Rect *ship_rect) {
    char num_holder[3];
    char x, y;
    sp1_ClearRectInv(wing_rect, INK_RED | PAPER_WHITE, ' ', SP1_RFLAG_TILE | SP1_RFLAG_COLOUR);
    if (wing->size == 0) {
        return;
    }
    num_holder[2] = '\0';
    
    x = wing_rect->col;
    y = wing_rect->row;

    sp1_SetPrintPos(&ps0, y + 1, x);
    // INIT
    sp1_PrintString(&ps0, "\x14\x50");
    to_string(wing->missile, num_holder);
    sp1_PrintString(&ps0, "MSL");
    sp1_PrintString(&ps0, num_holder);
    
    to_string(wing->heal, num_holder);
    sp1_PrintString(&ps0, " SUP");
    sp1_PrintString(&ps0, num_holder);

    inspect_ship(get_leader(wing), ship_rect);
}

void inspect_money(char money) {
    char num_holder[3];
    num_holder[2] = '\0';

    if (money_tile_not_ready) {
        print_big_at_inv(12, 21, INK_RED | PAPER_WHITE, CREDIT_TILES);
        money_tile_not_ready = False;
    }
    to_string(money, num_holder);
    sp1_SetPrintPos(&ps0, 13, 23);
    sp1_PrintString(&ps0, num_holder);

}

// void inspect_bonus(bonus bonus, char x, char y) {
// 
// }
#endif