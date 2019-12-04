#ifndef __SUP_SP_INSPECT__
#define __SUP_SP_INSPECT__

#include "ship.c"
#include "wing.c"
#include "base_sp1.c"
#include "tiles.c"
#include "types.h"

struct sp1_pss ps0;
struct sp1_pss pst;

extern uch att_bonus_text[];
extern uch hull_bonus_text[];
extern uch sp_bonus_text[];
extern uch msl_bonus_text[];
extern uch sup_bonus_text[];
extern uch arm_bonus_text[];
extern uch reb_bonus_text[];
extern uch upg_bonus_text[];
extern uch add_bonus_text[];
extern uch rep_bonus_text[];

uch money_tile_not_ready = True;

void init_inspector() {
    ps0.bounds = &full_screen;
    ps0.flags = SP1_PSSFLAG_INVALIDATE | SP1_PSSFLAG_XWRAP | SP1_PSSFLAG_YINC;
    ps0.visit = 0;
    pst.bounds = &target_inspect_rect;
    pst.flags = SP1_PSSFLAG_INVALIDATE;
    pst.visit = 0;
}

// void print_num(struct sp1_pss &ps, char num) {
//     return;
// }

void inspect_ship(ship *ship, struct sp1_Rect *rect) {
    // uch num_holder[3];
    uch i, mods, x, y;
    num_holder[2] = '\0';
    if (!ship->is_alive) {
        return;
    }
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
    to_string(ship->attack);
    sp1_PrintString(&ps0, "AT");
        sp1_PrintString(&ps0, num_holder);

    // SPECIAL
    to_string(ship->special);
    sp1_SetPrintPos(&ps0, y + 2, x);
    sp1_PrintString(&ps0, "SP"); sp1_PrintString(&ps0, num_holder);
    
    // HP
    to_string(ship->health);
    sp1_SetPrintPos(&ps0, y + 1, x + 4);
    sp1_PrintString(&ps0, "HP"); 
        sp1_PrintString(&ps0, num_holder);
        to_string(ship->max_health);
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
    // uch num_holder[3];
    uch x, y;
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
    to_string(wing->missile);
    sp1_PrintString(&ps0, "MSL");
    sp1_PrintString(&ps0, num_holder);
    
    to_string(wing->heal);
    sp1_PrintString(&ps0, " SUP");
    sp1_PrintString(&ps0, num_holder);

    inspect_ship(get_leader(wing), ship_rect);
}

void inspect_money(uch money) {
    if (money_tile_not_ready) {
        print_big_at_inv(12, 21, INK_RED | PAPER_WHITE, CREDIT_TILES);
        money_tile_not_ready = False;
    }
    to_string(money);
    sp1_SetPrintPos(&ps0, 13, 23);
    sp1_PrintString(&ps0, num_holder);
}

void inspect_bonus(uint16_t bonus) {
    sp1_ClearRectInv(&target_inspect_rect, INK_RED | PAPER_WHITE, ' ', SP1_RFLAG_TILE | SP1_RFLAG_COLOUR);
    sp1_PrintString(&pst, "\x14\x50""\x18\x01""\x19\x01");
    pst.bounds = &target_inspect_rect;
    // sp1_PrintString(&ps0, "\x19\x01");
    print_big_at_inv(3, 25, INK_RED | PAPER_WHITE, bonus);
    sp1_SetPrintPos(&pst, 5, 22);

    switch (bonus) {
        case ATTACK_TILES: 
            sp1_PrintString(&pst, att_bonus_text);
            break;
        case HP_MOD_TILES:
            sp1_PrintString(&pst, hull_bonus_text);
            break;
        case SPECIAL_TILES:
            sp1_PrintString(&pst, sp_bonus_text);
            break;
        case TORPEDO_MOD_TILES:
            sp1_PrintString(&pst, msl_bonus_text);
            break;
        case SUPPORT_MOD_TILES:
            sp1_PrintString(&pst, sup_bonus_text);
            break;
        case ARMOR_MOD_TILES:
            sp1_PrintString(&pst, arm_bonus_text);
            break;
        case REBIRTH_MOD_TILES:
            sp1_PrintString(&pst, reb_bonus_text);
            break;
        case UPGRADE_TILES:
            sp1_PrintString(&pst, upg_bonus_text);
            break;
        case ADD_SHIP_TILES:
            sp1_PrintString(&pst, add_bonus_text);
            break;
        case HEAL_TILES:
            sp1_PrintString(&pst, rep_bonus_text);
            break;
    }
}
#endif