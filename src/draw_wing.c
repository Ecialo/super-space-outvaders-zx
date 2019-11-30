#ifndef __SUP_SP_DRAW_WING__
#define __SUP_SP_DRAW_WING__

#include <arch/zx.h>
#include <arch/zx/sp1.h>
#include <input.h>
#include <z80.h>               // for z80_bpoke(), z80_wpoke(), im2 functions
#include <string.h>            // for memset()
#include <stdlib.h>            // for rand()
#include <intrinsic.h>         // for intrinsic_di()
#include <sound.h>             // for bit_beepfx()

#include "ship.c"
#include "wing.c"
#include "base_sp1.c"
#include "ship_sprites.h"

#define SHIP_SIZE 24

unsigned char flip_buffer[5][192];

struct sp1_ss *wing_sprites[10];

uint16_t our_wing_pos_x[] = {
    SHIP_SIZE * 2 + OFFSET, 
    SHIP_SIZE + OFFSET, 
    SHIP_SIZE * 3 + OFFSET, 
    OFFSET, 
    SHIP_SIZE * 4 + OFFSET
};
uint16_t our_wing_pos_y[] = {192 - 32 - 24, 192 - 16 - 24, 192 - 16 - 24, 192 - 0 - 24, 192 - 0 - 24};

uint16_t their_wing_pos_x[] = {
    SHIP_SIZE * 2 + OFFSET, 
    SHIP_SIZE + OFFSET, 
    SHIP_SIZE * 3 + OFFSET, 
    OFFSET, 
    SHIP_SIZE * 4 + OFFSET
};
uint16_t their_wing_pos_y[] = {32, 16, 16, 0, 0};


// 0   1   2   3   4   5   6  ...  63   64
// i*2                              
unsigned char* flip_sprite(unsigned char *sprite, char buff_i) {
    unsigned char i, column, column_offset, j;
    for (column = 0; column < 3; column++) {
        column_offset = column * 64;
        for (i = 0; i < 24; i++) {
            j = column_offset + 64 - 16 - i - i;
            flip_buffer[buff_i][column_offset + i*2] = sprite[j - 2];
            flip_buffer[buff_i][column_offset + i*2 + 1] = sprite[j - 1];
        }
        memcpy(flip_buffer[buff_i] + column_offset + 48, sprite + column_offset + 48, 16);
    }
    return flip_buffer[buff_i];
}

void init_ship_sprites(void) {
    char i;
    struct sp1_ss *s;
    for(i = 0; i < 10; i++) {
        wing_sprites[i] = s = sp1_CreateSpr(SP1_DRAW_MASK2LB, SP1_TYPE_2BYTE, 4, 0, 0);
        sp1_AddColSpr(s, SP1_DRAW_MASK2, SP1_TYPE_2BYTE, column2_offset, 0);
        sp1_AddColSpr(s, SP1_DRAW_MASK2, SP1_TYPE_2BYTE, column3_offset, 0);
        sp1_AddColSpr(s, SP1_DRAW_MASK2RB, SP1_TYPE_2BYTE, 0, 0);
    }
}


void color_interceptor(unsigned int count, struct sp1_cs *cs) {
    cs->attr_mask = SP1_AMASK_INK;
    cs->attr = INK_RED | PAPER_BLACK;
}

void color_destroyer(unsigned int count, struct sp1_cs *cs) {
    cs->attr_mask = SP1_AMASK_INK;
    cs->attr = INK_BLUE | PAPER_BLACK;
}

void color_support(unsigned int count, struct sp1_cs *cs) {
    cs->attr = INK_GREEN | PAPER_BLACK;
    cs->attr_mask = SP1_AMASK_INK;
}

void color_bomber(unsigned int count, struct sp1_cs *cs) {
    cs->attr_mask = SP1_AMASK_INK;
    cs->attr = INK_YELLOW | PAPER_BLACK;
}


void render_wing(wing *wing, char side) {
    char i;
    char offset = side * 5;
    unsigned char *sprite;
    uint16_t *xs, *ys;
    ship_type st;

    if (side == OUR_SIDE) {
        xs = our_wing_pos_x;
        ys = our_wing_pos_y;
    } else {
        xs = their_wing_pos_x;
        ys = their_wing_pos_y;
    }

    for (i = 0; i < wing->size; i++) {
        st = get_ship(wing, i)->type;
        switch (st) {
            case INTERCEPTOR:
                sp1_IterateSprChar(wing_sprites[i + offset], color_interceptor);
                sprite = interceptor_11;
                break;
            case BOMBER:
                sp1_IterateSprChar(wing_sprites[i + offset], color_bomber);
                sprite = bomber_11;
                break;
            case DESTROYER:
                sp1_IterateSprChar(wing_sprites[i + offset], color_destroyer);
                sprite = destroyer_11;
                break;
            case SUPPORT:
                sp1_IterateSprChar(wing_sprites[i + offset], color_support);
                sprite = support_11;
                break;
        }
        if (side == OUR_SIDE) {
            sp1_MoveSprPix(
                wing_sprites[i + offset], 
                &full_screen, 
                sprite, 
                xs[i], 
                ys[i]
            );
        } else {
            sp1_MoveSprPix(
                wing_sprites[i + offset], 
                &full_screen, 
                flip_sprite(sprite, i), 
                xs[i], 
                ys[i]
            );
        }
    }
    if (side == OUR_SIDE) {
        sp1_Invalidate(&our_wing_rect);
    } else {
        sp1_Invalidate(&env_rect);
    }
}

void clear_screen_from_wing(char side) {
    char i;
    char offset = side * 5;
    for (i = 0; i < 5; i++) {
        sp1_MoveSprAbs(
            wing_sprites[i + offset],
            &full_screen,
            NULL,
            0, 34, 0, 0
        );
    }
}


#endif