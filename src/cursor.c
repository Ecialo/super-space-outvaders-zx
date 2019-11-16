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
#include "icons.h"
#include "draw_wing.c"
// #include "src/utils.c"

typedef enum CursorOption {
    ATTACK_OPTION,
    FLEE_OPTION,
    SPECIAL_OPTION
} cursor_option;

struct sp1_ss *options_sprites[3];
struct sp1_ss *cursor_sprites[4];

cursor_option battle_options[] = {FLEE_OPTION, ATTACK_OPTION, SPECIAL_OPTION};
char CURSOR_POS;

void color_icons(unsigned int count, struct sp1_cs *cs) {
     cs->attr = INK_MAGENTA | PAPER_WHITE;
}


void init_icons() {
    char i;
    struct sp1_ss *s;
    for (i = 0; i < 3; i++) {
        options_sprites[i] = s = sp1_CreateSpr(SP1_DRAW_MASK2LB, SP1_TYPE_2BYTE, 3, 0, 1);
        sp1_AddColSpr(s, SP1_DRAW_MASK2, SP1_TYPE_2BYTE, icon_offset, 1);
        sp1_AddColSpr(s, SP1_DRAW_MASK2RB, SP1_TYPE_2BYTE, 0, 1);
        sp1_IterateSprChar(s, color_icons);
    }
}

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
    char i;
    for (i = 0; i < 3; i++) {
        sp1_MoveSprAbs(options_sprites[i], &full_screen, NULL, 0, 34, 0, 0);
    }
}

void draw_options(cursor_option *options) {
    unsigned char i;
    for (i = 0; i < 3; i++) {
        switch(options[i]) {
            case ATTACK_OPTION:
                sp1_MoveSprPix(
                    options_sprites[i], 
                    &full_screen, 
                    attack_ic1, 
                    // 0, 
                    16 + 16*i, 
                    64
                );
                break;
            case FLEE_OPTION:
                sp1_MoveSprPix(
                    options_sprites[i], 
                    &full_screen, 
                    flee_ic1, 
                    // 0, 
                    16 + 16*i, 
                    64
                );
                break;
            case SPECIAL_OPTION:
                sp1_MoveSprPix(
                    options_sprites[i], 
                    &full_screen, 
                    special_ic1, 
                    // 0, 
                    16 + 16*i, 
                    64
                );
                break;
        }
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

void select_from_options(cursor_option *options) {
    CURSOR_POS = 1;
    draw_options(options);
    draw_ramka_around(options_sprites[CURSOR_POS]);
    sp1_UpdateNow();
    while(1) {
        in_wait_nokey();
        in_wait_key();


        if (in_key_pressed(IN_KEY_SCANCODE_a) && CURSOR_POS > 0) {
            CURSOR_POS--;
        } else if (in_key_pressed(IN_KEY_SCANCODE_d) && CURSOR_POS < 3) {
            CURSOR_POS++;
        } else if (in_key_pressed(IN_KEY_SCANCODE_SPACE)) {
            break;
        }
        draw_ramka_around(options_sprites[CURSOR_POS]);
        sp1_UpdateNow();
    }
}

void select_from_battle_options() {
    select_from_options(battle_options);
}


void select_from_wing(wing *wing, char side) {
    char offset = side * 5;
    CURSOR_POS = 0;
    char s;
    s = wing->size - 1;
    render_wing(wing, side);
    draw_ramka_around(wing_sprites[CURSOR_POS + offset]);
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
        sp1_UpdateNow();
    }
}

#endif