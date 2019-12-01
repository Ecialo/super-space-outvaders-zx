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
#include "tiles.c"

#define SHIP_SIZE 24

 unsigned char flip_buffer[5][96];

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
         column_offset = column * 32;
         for (i = 0; i < 24; i++) {
             j = column_offset + 32 - 8 - i;
             flip_buffer[buff_i][column_offset + i] = sprite[j - 1];
         }
         memcpy(flip_buffer[buff_i] + column_offset + 24, sprite + column_offset + 24, 8);
     }
    return flip_buffer[buff_i];
}

void init_ship_sprites(void) {
    char i;
    struct sp1_ss *s;
    for(i = 0; i < 10; i++) {
        // wing_sprites[i] = s = sp1_CreateSpr(SP1_DRAW_MASK2LB, SP1_TYPE_2BYTE, 4, 0, 0);
        // sp1_AddColSpr(s, SP1_DRAW_MASK2, SP1_TYPE_2BYTE, column2_offset, 0);
        // sp1_AddColSpr(s, SP1_DRAW_MASK2, SP1_TYPE_2BYTE, column3_offset, 0);
        // sp1_AddColSpr(s, SP1_DRAW_MASK2RB, SP1_TYPE_2BYTE, 0, 0);
        wing_sprites[i] = s = sp1_CreateSpr(SP1_DRAW_OR1LB, SP1_TYPE_1BYTE, 4, 0, 0);
        sp1_AddColSpr(s, SP1_DRAW_OR1, SP1_TYPE_1BYTE, column2_offset, 0);
        sp1_AddColSpr(s, SP1_DRAW_OR1, SP1_TYPE_1BYTE, column3_offset, 0);
        sp1_AddColSpr(s, SP1_DRAW_OR1RB, SP1_TYPE_1BYTE, 0, 0);
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

void vsync(int wait) {
    vsync_i = wait;
    while (vsync_i) {
        --vsync_i;
    }
}

struct sp1_ss *bullet_sp;
unsigned char bullet_sprite[23] = {0, 0, 0, 0,
                            0, 0, 0, 255,
                            255, 255, 255, 255,
                            255, 255, 255, 
                            0, 0, 0, 0, 0,
                            0, 0, 0
                           };


void bullet_color(unsigned int count, struct sp1_cs *cs) {
     cs->attr = INK_RED | PAPER_BLACK;
}
void init_bullet(void) {
    bullet_sp = sp1_CreateSpr(SP1_DRAW_LOAD1LB, SP1_TYPE_1BYTE, 2, (int) (bullet_sprite + 7), 11);
    sp1_AddColSpr(bullet_sp, SP1_DRAW_LOAD1RB, SP1_TYPE_1BYTE, 0, 0);
    sp1_IterateSprChar(bullet_sp, bullet_color);
}

#define MAX_SPRITES_MOVE MAX_WING_SIZE
void move_multiple_sprites(int count, struct sp1_ss **sp, int *x1, int *x2, int *y1, int *y2, int pause, int steps)
{
    int dx[MAX_SPRITES_MOVE], dy[MAX_SPRITES_MOVE];
    int i, j;
    for (j = 0; j < count; ++j){
        dx[j] = (x2[j] - x1[j]) / steps;
        dy[j] = (y2[j] - y1[j]) / steps;
    }
    for (i = 0; i < steps - 1; ++i){
        for (j = 0; j < count; ++j) {
            sp1_MoveSprPix(sp[j], 
                &full_screen, 
                0, 
                x1[j] + i * dx[j], 
                y1[j] + i * dy[j]
            );
        }
        sp1_UpdateNow();
        vsync(pause);
    }
    for (j = 0; j < count; ++j){
        sp1_MoveSprPix(sp[j], 
            &full_screen, 
            0, 
            x2[j], 
            y2[j]
        );
    }
    sp1_UpdateNow();
    vsync(pause);
}

void move_sprite(struct sp1_ss *sp, int x1, int x2, int y1, int y2, int pause, int steps)
{
    move_multiple_sprites(1, &sp, &x1, &x2, &y1, &y2, pause, steps);
}

void shoot_bullet(char our, char their, char side)
{

    int x1,y1,x2,y2;
    x1 = our_wing_pos_x[our] + 8;
    y1 = our_wing_pos_y[our] - 8;  

    x2 = their_wing_pos_x[their] + 8;
    y2 = their_wing_pos_y[their] + 24;

    if (side == THEIR_SIDE)
        move_sprite(bullet_sp, x2, x1, y2, y1, 1500, 10);
    else
        move_sprite(bullet_sp, x1, x2, y1, y2, 1500, 10);
    bit_fx(BFX_EXPLOSION);
    sp1_MoveSprPix(bullet_sp, 
        &full_screen, 
        0, 
        256,
        256
    );
    sp1_UpdateNow();
}

#define SWAP_VSYNC 2000
void swap_ships_sprites(char side, int a, int b) { 
    uint16_t y_upper, y_lower;
 
    struct sp1_ss **sp;
    uint16_t *wing_pos_x;
    uint16_t *wing_pos_y;

    struct sp1_ss *tptr;

    int xOrigin[2];
    int xTarget[2];

    int yOrigin[2];
    int yTarget[2];

    struct sp1_ss* sprites[2];

    y_upper = 64;
    y_lower = 96;

    sp = wing_sprites + 5 * side;
    if (side == OUR_SIDE) {
        wing_pos_x = our_wing_pos_x;
        wing_pos_y = our_wing_pos_y;
    }
    else {
        wing_pos_x = their_wing_pos_x;
        wing_pos_y = their_wing_pos_y;
    }
    sprites[0] = sp[a];
    sprites[1] = sp[b];

    xOrigin[0] = wing_pos_x[a];
    xTarget[0] = wing_pos_x[a];

    xOrigin[1] = wing_pos_x[b];
    xTarget[1] = wing_pos_x[b];

    yOrigin[0] = wing_pos_y[a];
    yTarget[0] = y_upper;

    yOrigin[1] = wing_pos_y[b];
    yTarget[1] = y_lower;

    move_multiple_sprites(2, sprites, xOrigin, xTarget, yOrigin, yTarget, SWAP_VSYNC, 4);

    xOrigin[0] = wing_pos_x[a];
    xTarget[0] = wing_pos_x[b];

    xOrigin[1] = wing_pos_x[b];
    xTarget[1] = wing_pos_x[a];

    yOrigin[0] = y_upper;
    yTarget[0] = y_upper;

    yOrigin[1] = y_lower;
    yTarget[1] = y_lower;

    move_multiple_sprites(2, sprites, xOrigin, xTarget, yOrigin, yTarget, SWAP_VSYNC, 8);

    xOrigin[0] = wing_pos_x[b];
    xTarget[0] = wing_pos_x[b];

    xOrigin[1] = wing_pos_x[a];
    xTarget[1] = wing_pos_x[a];

    yOrigin[0] = y_upper;
    yTarget[0] = wing_pos_y[b];

    yOrigin[1] = y_lower;
    yTarget[1] = wing_pos_y[a];

    move_multiple_sprites(2, sprites, xOrigin, xTarget, yOrigin, yTarget, SWAP_VSYNC, 4);

    tptr = sp[a];
    sp[a] = sp[b];
    sp[b] = tptr;
}


#define HYPERSPACE_STEPS 2
void into_the_hyperspace(int count) {
    // struct sp1_ss *sprites[MAX_WING_SIZE];
    int yOrigin[MAX_WING_SIZE];
    int yTarget[MAX_WING_SIZE];

    int steps[HYPERSPACE_STEPS];
    int *or, *tar, *tmp;
    char i, j;
    
    for (i = 0; i < count; ++i) {
        // sprites[i] = wing_sprites[i];
        yOrigin[i] = our_wing_pos_y[i];
    }

    for (i = 0; i < HYPERSPACE_STEPS; ++i){
        steps[i] = 15 - i * 10;
    }
    or = yOrigin;
    tar = yTarget;

    for (j = 0; j < HYPERSPACE_STEPS; ++j){
        for (i = 0; i < count; ++i)
            tar[i] = or[i] - 100;
        move_multiple_sprites(count, wing_sprites, our_wing_pos_x, our_wing_pos_x, or, tar, 400, steps[j]);
        tmp = or;
        or = tar;
        tar = tmp;
    }
}

void leader_retreat(char side, int target) {
    for (int i = 1; i <= target; ++i){
        swap_ships_sprites(side, i, i - 1);
    }
}


void draw_stars() {
    for (int i = 0; i < 21; ++i)
        for (int j = 0; j < 24; ++j){
            if (rand() % 4 == 0)
                sp1_PrintAtInv(j, i, INK_WHITE | PAPER_BLACK,  STAR_TILES + rand() % STAR_TILE_TYPES);
            else
                sp1_PrintAtInv(j, i, INK_WHITE | PAPER_BLACK,  ' ');
        }
}



#endif