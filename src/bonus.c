#ifndef __SUP_SP_BONUS__
#define __SUP_SP_BONUS__

#include <arch/zx.h>
#include <arch/zx/sp1.h>
#include <input.h>
#include <z80.h>               // for z80_bpoke(), z80_wpoke(), im2 functions
#include <string.h>            // for memset()
#include <stdlib.h>            // for rand()
#include <intrinsic.h>         // for intrinsic_di()
#include <sound.h>             // for bit_beepfx()

#include "types.h"
#include "ship.c"
#include "wing.c"
#include "utils.c"
#include "base_sp1.c"
#include "tiles.c"
#include "draw_wing.c"
#include "cursor.c"
#include "inspect.c"

#define NUM_OF_MOD_TYPES 10
wing bonus_wing;

char bonus2mod[] = {
    EXTRA_GUNS,
    EXTRA_SHIELD,
    EXTRA_COMP,
    TORPEDO,
    REMTECH,
    ARMOR,
    REBIRTH
};

char bonus_cost[] = {
    3,
    3,
    3,
    3,
    3,
    5,
    7,
    10,
    5,
    1
};

void init_bonus_wing() {
    init_wing(&bonus_wing);
    add_ship(&bonus_wing, "1", INTERCEPTOR);
    add_ship(&bonus_wing, "1", DESTROYER);
    add_ship(&bonus_wing, "1", BOMBER);
    add_ship(&bonus_wing, "1", SUPPORT);
}

void draw_costs(bonus *bonuses, char num) {
    char num_holder[3];
    unsigned char i;
    sp1_ClearRectInv(&costs_rect, INK_RED | PAPER_WHITE, ' ', SP1_RFLAG_TILE | SP1_RFLAG_COLOUR);
    sp1_PrintString(&ps0, "\x14\x50");
    for (i = 0; i < num; i++) {
        to_string(bonus_cost[bonuses[i]], num_holder);
        sp1_SetPrintPos(&ps0, 13, 2 + 3 * i);
        sp1_PrintString(&ps0, num_holder);
    }
}

unsigned char MONEY = 99;
// unsigned char MONEY = 0;
char select_bonus(bonus *bonuses, char num, wing *wing, char is_shop) {
    uint16_t tiles_for_options[5];
    bonus bonus;
    ship *ship;
    char i, selected_bonus;
    for (i = 0; i < num; i++) {
        tiles_for_options[i] = tiles_for_bonus[bonuses[i]];
    }
    if (is_shop) {
        draw_costs(bonuses, num);
    }
    for (;;) {
        select_from_options(tiles_for_options, num, True);
        selected_bonus = CURSOR_POS;
        bonus = bonuses[selected_bonus];
        if (is_shop && MONEY >= bonus_cost[bonus]) {
            MONEY -= bonus_cost[bonus];
            inspect_money(MONEY);
            break;
        } else if (!is_shop) {
            break;
        }
    }
    switch (bonus) {
        case HEAL:
            heal_wing(wing, 1);
            break;
        case UPG:   // if all t2???
            for (;;) {
                select_from_wing(wing, OUR_SIDE);
                if (upgrade_ship(get_ship(wing, CURSOR_POS))) {
                    break;
                }
            }
            break;
        case NEW:
            render_wing(&bonus_wing, THEIR_SIDE);
            select_from_wing(&bonus_wing, THEIR_SIDE);
            clear_screen_from_wing(THEIR_SIDE);
            ship = get_ship(&bonus_wing, CURSOR_POS);
            if (wing->size == MAX_WING_SIZE) {
                select_from_wing(wing, OUR_SIDE);
                remove_ship(wing, CURSOR_POS);
            }
            add_ship(wing, ship->name, ship->type);
            render_wing(wing, OUR_SIDE);
            break;
        default: // mod
            for (;;) {
                select_from_wing(wing, OUR_SIDE);
                ship = get_ship(wing, CURSOR_POS);
                // i = install_mod(wing, ship, bonus2mod[bonus]);
                if (install_mod(    // if no free slot for upgrade???
                    wing, 
                    get_ship(wing, CURSOR_POS), 
                    bonus2mod[bonus]
                )) {
                    break;
                }
            }
            break;
    }
    return selected_bonus;
}

char collect_bonuses(bonus *bonuses, char num, wing *wing, char is_shop) {
    bonus actual_bonuses[5];
    char i, selected_bonus;
    char actual_num = num;
    // actual_num = num;
    for (i = 0; i < num; i++) {
        actual_bonuses[i] = bonuses[i];
    }
    while (actual_num) {
        selected_bonus = select_bonus(actual_bonuses, actual_num, wing, is_shop);
        for (i = selected_bonus + 1; i < actual_num; i++) {
            actual_bonuses[i - 1] = actual_bonuses[i];
        }
        actual_num--;
    }
}

#endif