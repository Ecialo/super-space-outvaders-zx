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

// #define SHIP_SIZE 16
#define SHIP_SIZE 24

// draw_ship

extern unsigned char ship21[];
extern unsigned char ship22[];
extern unsigned char ship23[];
// #include "data/alien.h"
// #include "data/ship2.h"
// #include "data/interceptor.h"

// struct sp1_Rect cr = {0, 0, 32, 24};
struct sp1_ss *wing_sprites[10];

uint16_t our_wing_pos_x[] = {
    SHIP_SIZE * 2 + OFFSET, 
    SHIP_SIZE + OFFSET, 
    SHIP_SIZE * 3 + OFFSET, 
    OFFSET, 
    SHIP_SIZE * 4 + OFFSET
};
uint16_t our_wing_pos_y[] = {32, 16, 16, 0, 0};

uint16_t their_wing_pos_x[] = {
    SHIP_SIZE * 2 + OFFSET, 
    SHIP_SIZE + OFFSET, 
    SHIP_SIZE * 3 + OFFSET, 
    OFFSET, 
    SHIP_SIZE * 4 + OFFSET
};
// uint16_t their_wing_pos_y[] = {192 - 16, 192 - 8, 192 - 8, 192 - 0, 192 - 0};
uint16_t their_wing_pos_y[] = {192 - 32 - 24, 192 - 16 - 24, 192 - 16 - 24, 192 - 0 - 24, 192 - 0 - 24};

void render_ship(ship *ship, struct sp1_ss *sprite, int x, int y) {

}

void init_ship_sprites(void) {
    char i;
    struct sp1_ss *s;
    for(i = 0; i < 10; i++) {
        // wing_sprites[i] = s = sp1_CreateSpr(SP1_DRAW_MASK2LB, SP1_TYPE_2BYTE, 4, (int)ship21, 0);
        // sp1_AddColSpr(s, SP1_DRAW_MASK2, SP1_TYPE_2BYTE, (int)ship22, 0);
        // sp1_AddColSpr(s, SP1_DRAW_MASK2, SP1_TYPE_2BYTE, (int)ship23, 0);
        // sp1_AddColSpr(s, SP1_DRAW_MASK2RB, SP1_TYPE_2BYTE, 0, 0);
        wing_sprites[i] = s = sp1_CreateSpr(SP1_DRAW_MASK2LB, SP1_TYPE_2BYTE, 4, 0, 3);
        sp1_AddColSpr(s, SP1_DRAW_MASK2, SP1_TYPE_2BYTE, (int)ship22 - (int)ship21, 3);
        sp1_AddColSpr(s, SP1_DRAW_MASK2, SP1_TYPE_2BYTE, (int)ship23 - (int)ship21, 3);
        sp1_AddColSpr(s, SP1_DRAW_MASK2RB, SP1_TYPE_2BYTE, 0, 1);
    }
}


void color_interceptor(unsigned int count, struct sp1_cs *cs) {
     cs->attr = INK_RED | PAPER_BLACK;
}

void color_destroyer(unsigned int count, struct sp1_cs *cs) {
     cs->attr = INK_BLUE | PAPER_BLACK;
}

void color_support(unsigned int count, struct sp1_cs *cs) {
     cs->attr = INK_GREEN | PAPER_BLACK;
}

void color_bomber(unsigned int count, struct sp1_cs *cs) {
     cs->attr = INK_YELLOW | PAPER_BLACK;
}


void render_wing(wing *wing, char side) {
    char i;
    char offset = side * 5;
    // char offset = 0;
    ship_type st;
    for (i = 0; i < wing->size; i++) {
        st = get_ship(wing, i)->type;
        switch (st) {
            case INTERCEPTOR:
                sp1_IterateSprChar(wing_sprites[i + offset], color_interceptor);
                break;
            case BOMBER:
                sp1_IterateSprChar(wing_sprites[i + offset], color_bomber);
                break;
            case DESTROYER:
                sp1_IterateSprChar(wing_sprites[i + offset], color_destroyer);
                break;
            case SUPPORT:
                sp1_IterateSprChar(wing_sprites[i + offset], color_support);
                break;
        }
        if (side) {
            sp1_MoveSprPix(
                wing_sprites[i + offset], 
                // &our_wing_rect, 
                &full_screen, 
                ship21, 
                // 0, 
                // 0,
                // 0
                our_wing_pos_x[i], 
                our_wing_pos_y[i]
            );
        } else {
            sp1_MoveSprPix(
                wing_sprites[i + offset], 
                // &env_rect, 
                &full_screen, 
                // interceptor, 
                ship21, 
                // 0, 
                // 0,
                // 0
                their_wing_pos_x[i], 
                their_wing_pos_y[i]
            );
        }
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