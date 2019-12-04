#ifndef __SUP_SP_BONUS__
#define __SUP_SP_BONUS__

#include <arch/zx.h>
#include <arch/zx/sp1.h>
#include <input.h>
#include <z80.h>               // for z80_bpoke(), z80_wpoke(), im2 functions
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

uch bonus2mod[] = {
    EXTRA_GUNS,
    EXTRA_SHIELD,
    EXTRA_COMP,
    TORPEDO,
    REMTECH,
    ARMOR,
    REBIRTH
};

uch bonus_cost[] = {
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
    add_ship(&bonus_wing, INTERCEPTOR);
    add_ship(&bonus_wing, DESTROYER);
    add_ship(&bonus_wing, BOMBER);
    add_ship(&bonus_wing, SUPPORT);
}

void draw_costs(bonus *bonuses, uch num) {
    uch i;
    // sp1_ClearRectInv(&costs_rect, INK_RED | PAPER_WHITE, ' ', SP1_RFLAG_TILE | SP1_RFLAG_COLOUR);
    sp1_PrintString(&ps0, "\x14\x50");
    for (i = 0; i < num; i++) {
        to_string(bonus_cost[bonuses[i]]);
        sp1_SetPrintPos(&ps0, 13, 2 + 3 * i);
        sp1_PrintString(&ps0, num_holder);
    }
}

// uch MONEY = 99;
uch MONEY = 0;
uch select_bonus(bonus *bonuses, uch num, wing *wing, uch is_shop) {
    uint16_t tiles_for_options[5];
    bonus bonus;
    ship *ship;
    uch i, selected_bonus;
    for (i = 0; i < num; i++) {
        tiles_for_options[i] = tiles_for_bonus[bonuses[i]];
    }
    if (is_shop) {
        draw_costs(bonuses, num);
    }
    for (;;) {
        select_from_options(tiles_for_options, num, True);
        if (CURSOR_POS == CURSOR_CANCEL) {
            return CURSOR_CANCEL;
        }
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
    if (is_shop) {
        sp1_ClearRectInv(&costs_rect, INK_RED | PAPER_WHITE, ' ', SP1_RFLAG_TILE | SP1_RFLAG_COLOUR);
    }
    switch (bonus) {
        case HEAL:
            heal_wing(wing, 1);
            break;
        case UPG:   // if all t2???
            for (;;) {
                select_from_wing(wing, OUR_SIDE);
                if (promote_ship(get_ship(wing, CURSOR_POS)) || CURSOR_POS == CURSOR_CANCEL) {
                    CURSOR_POS = 1;
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
            add_ship(wing, ship->type);
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
                ) || CURSOR_POS == CURSOR_CANCEL) {
                    break;
                }
            }
            break;
    }
    return selected_bonus;
}

bonus actual_bonuses[5];
void collect_bonuses(bonus *bonuses, uch num, wing *wing, uch is_shop) {
    uch i, selected_bonus;
    uch actual_num = num;
    // actual_num = num;
    for (i = 0; i < num; i++) {
        actual_bonuses[i] = bonuses[i];
    }
    while (actual_num) {
        selected_bonus = select_bonus(actual_bonuses, actual_num, wing, is_shop);
        if (selected_bonus == CURSOR_CANCEL) {
            break;
        }
        for (i = selected_bonus + 1; i < actual_num; i++) {
            actual_bonuses[i - 1] = actual_bonuses[i];
        }
        actual_num--;
        render_wing(wing, OUR_SIDE);
        sp1_UpdateNow();
    }
}

bonus rand_mod() {
    return (bonus) (rand() % 7);
}

bonus bonuses[3];
void get_bonuses(uch power, wing *wing) {
    uch rpower, i, num;
    rpower = 0;
    if (power > 3) {
        power = 3;
    }
    if (power == 0) {
        return;
    }

    power = 1;

    for (i = 0; i < power; i++) {
        rpower += (rand() % 4) + 1;
    }
    switch (rpower) {
        case 9:
        case 1:
            bonuses[0] = HEAL;
            num = 1;
            break;
        case 2:
        case 6:
        case 3:
            bonuses[0] = rand_mod();
            num = 1;
            break;
        case 4:
            bonuses[0] = HEAL;
            bonuses[1] = rand_mod();
            num = 2;
            break;
        case 7:
            bonuses[0] = rand_mod();
            bonuses[1] = rand_mod();
            num = 2;
            break;
        case 8:
            bonuses[0] = UPG;
            num = 1;
            break;
        case 10:
            bonuses[0] = HEAL;
            bonuses[1] = rand_mod();
            bonuses[2] = UPG;
            num = 3;
            break;
        case 11:
            bonuses[0] = HEAL;
            bonuses[1] = ARM;
            bonuses[2] = UPG;
            num = 3;
            break;
        case 12:
            bonuses[0] = HEAL;
            bonuses[1] = REB;
            bonuses[2] = UPG;
            num = 3;
            break;
    }
    collect_bonuses(bonuses, num, wing, False);
}

#endif