#ifndef __SUP_SP_CURSOR__
#define __SUP_SP_CURSOR__

#include <arch/zx.h>
#include <arch/zx/sp1.h>
#include <input.h>
#include <z80.h>               // for z80_bpoke(), z80_wpoke(), im2 functions
#include <string.h>            // for memset()
#include <stdlib.h>            // for rand()
#include <intrinsic.h>         // for intrinsic_di()
#include <sound.h>             // for bit_beepfx()

#include "base_sp1.c"
#include "ship.c"
#include "wing.c"
#include "draw_wing.c"
#include "tiles.c"
#include "inspect.c"

#define SWAP_OPTION 0
#define SPY_OPTION 1
#define ADVANCE_OPTION 2

#define OPTION 0
#define BONUS 1
#define SHOP 2
// #include "src/utils.c"

extern unsigned char ramka_lb1[];
extern unsigned char ramka_rb1[];
extern unsigned char ramka_rt1[];
extern unsigned char ramka_lt1[];
struct sp1_ss *cursor_sprites[4];

uint16_t battle_options[] = {RETREAT_TILES, ATTACK_TILES, SPECIAL_TILES};
uint16_t prepare_options[] = {RETREAT_TILES, SPY_TILES, ATTACK_TILES};
char CURSOR_POS;

void init_cursor() {
    char i;
    struct sp1_ss *s;
    for (i = 0; i < 4; i++) { 
        cursor_sprites[i] = s = sp1_CreateSpr(SP1_DRAW_MASK2LB, SP1_TYPE_2BYTE, 2, 0, 0);
        sp1_AddColSpr(s, SP1_DRAW_MASK2RB, SP1_TYPE_2BYTE, 0, 0);
    }
}

void clear_screen_from_cursor() {
    char i;
    for (i = 0; i < 4; i++) {
        sp1_MoveSprAbs(cursor_sprites[i], &full_screen, NULL, 0, 34, 0, 0);
    }
}

void clear_screen_from_options() {
    sp1_ClearRectInv(&options_rect, INK_BLACK | PAPER_BLACK, ' ', SP1_RFLAG_TILE | SP1_RFLAG_COLOUR);
    // char i;
    // for (i = 0; i < 5; i++) {
    //     print_big_empty(8, 2 + 2 * i, INK_MAGENTA | PAPER_WHITE);
    // }
}

void draw_options(uint16_t *options, char num_of_options) {
    unsigned char i;
    for (i = 0; i < num_of_options; i++) {
        print_big_at_inv(
            11,
            2 + 3 * i,
            // INK_MAGENTA | PAPER_WHITE,
            OPTIONS_COLOR,
            options[i]
        );
    }
}

void draw_ramka_at(uint16_t row, uint16_t col, uint16_t w, uint16_t h) {
    sp1_MoveSprAbs(
        cursor_sprites[0], &full_screen, 
        ramka_lt1, 
        row, 
        col, 
        0, 0
    );
    sp1_MoveSprAbs(
        cursor_sprites[1], &full_screen, 
        ramka_lb1, 
        row + h - 1, 
        col, 
        0, 0
    );
    sp1_MoveSprAbs(
        cursor_sprites[2], &full_screen, 
        ramka_rt1, 
        row, 
        col + w - 1, 
        0, 0
    );
    sp1_MoveSprAbs(
        cursor_sprites[3], &full_screen, 
        ramka_rb1, 
        row + h - 1, 
        col + w - 1, 
        0, 0
    );
}

void draw_ramka_around(struct sp1_ss *target) {
    sp1_MoveSprAbs(
        cursor_sprites[0], &full_screen, 
        ramka_lt1, 
        target->row, 
        target->col, 
        target->vrot, target->hrot
    );
    sp1_MoveSprAbs(
        cursor_sprites[1], &full_screen, 
        ramka_lb1, 
        target->row + target->height - 2, 
        target->col, 
        target->vrot, target->hrot
    );
    sp1_MoveSprAbs(
        cursor_sprites[2], &full_screen, 
        ramka_rt1, 
        target->row, 
        (target->col) + (target->width) - 2, 
        target->vrot, target->hrot
    );
    sp1_MoveSprAbs(
        cursor_sprites[3], &full_screen, 
        ramka_rb1, 
        target->row + target->height - 2, 
        (target->col) + (target->width) - 2, 
        target->vrot, target->hrot
    );
}

void select_from_options(uint16_t *options, char num_of_options, char mode) {
    char s;
    if (num_of_options > 1) {
        CURSOR_POS = 1;
    } else {
        CURSOR_POS = 0;
    }
    s = num_of_options - 1;
    sp1_ClearRectInv(&options_rect, OPTIONS_COLOR, ' ', SP1_RFLAG_TILE | SP1_RFLAG_COLOUR);
    draw_options(options, num_of_options);
    draw_ramka_at(11, 2 + 3 * CURSOR_POS, 2, 2);
    if (mode != OPTION) {
        inspect_bonus(options[CURSOR_POS]);
    }
    // draw_ramka_around(options_sprites[CURSOR_POS]);
    sp1_UpdateNow();
    while(1) {
        in_wait_nokey();
        in_wait_key();


        if (in_key_pressed(IN_KEY_SCANCODE_a) && CURSOR_POS > 0) {
            CURSOR_POS--;
        } else if (in_key_pressed(IN_KEY_SCANCODE_d) && CURSOR_POS < s) {
            CURSOR_POS++;
        } else if (in_key_pressed(IN_KEY_SCANCODE_SPACE)) {
            break;
        }
        draw_ramka_at(11, 2 + 3 * CURSOR_POS, 2, 2);
        if (mode != OPTION) {
            inspect_bonus(options[CURSOR_POS]);
        }
        // draw_ramka_around(options_sprites[CURSOR_POS]);
        sp1_UpdateNow();
    }
    clear_screen_from_options();
}

void select_from_battle_options() {
    select_from_options(battle_options, 3, False);
}

void select_from_prepare_options() {
    select_from_options(prepare_options, 3, False);
}


void select_from_wing(wing *wing, char side) {
    char s;
    char offset = side * 5;
    CURSOR_POS = 0;

    s = wing->size - 1;
    render_wing(wing, side);
    draw_ramka_around(wing_sprites[CURSOR_POS + offset]);
    inspect_ship(get_ship(wing, CURSOR_POS), &target_inspect_ship_rect);
    sp1_UpdateNow();
    while(1) {
        in_wait_nokey();
        in_wait_key();
        if (in_key_pressed(IN_KEY_SCANCODE_a) && CURSOR_POS > 0) {
            CURSOR_POS--;
        } else if (in_key_pressed(IN_KEY_SCANCODE_d) && CURSOR_POS < s) {
            CURSOR_POS++;
        } else if (in_key_pressed(IN_KEY_SCANCODE_SPACE)) {
            break;
        }
        render_wing(wing, side);
        draw_ramka_around(wing_sprites[CURSOR_POS + offset]);

        inspect_ship(get_ship(wing, CURSOR_POS), &target_inspect_ship_rect);
        
        sp1_UpdateNow();
    }
}

#endif