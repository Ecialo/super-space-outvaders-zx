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


#pragma output REGISTER_SP           = 0xd000    // place stack at $d000 at startup
#pragma output CLIB_MALLOC_HEAP_SIZE = 7000      // create a 3000-byte heap in BSS section

#pragma output CRT_ORG_CODE          = 32768     // org 32768
#pragma output CLIB_EXIT_STACK_SIZE  = 0         // no atexit() functions
#pragma output CLIB_STDIO_HEAP_SIZE  = 0         // no memory for files
#pragma output CLIB_FOPEN_MAX        = -1        // do not create open files list



// #define SHIP_SIZE 16
#define SHIP_SIZE 24

// draw_ship

extern unsigned char ship21[];
extern unsigned char ship22[];
extern unsigned char ship23[];
// #include "data/alien.h"
// #include "data/ship2.h"
// #include "data/interceptor.h"

struct sp1_Rect cr = {0, 0, 32, 24};
struct sp1_ss *wing_sprites[10];

struct sp1_ss *bullet_sp;
unsigned char bullet_sprite[23] = {0, 0, 0, 0,
                            0, 0, 0, 255,
                            255, 255, 255, 255,
                            255, 255, 255, 
                            0, 0, 0, 0, 0,
                            0, 0, 0
                           };

uint16_t our_wing_pos_x[] = {SHIP_SIZE * 2, SHIP_SIZE, SHIP_SIZE * 3, 0, SHIP_SIZE * 4};
uint16_t our_wing_pos_y[] = {32, 16, 16, 0, 0};

uint16_t their_wing_pos_x[] = {SHIP_SIZE * 2, SHIP_SIZE, SHIP_SIZE * 3, 0, SHIP_SIZE * 4};
// uint16_t their_wing_pos_y[] = {192 - 16, 192 - 8, 192 - 8, 192 - 0, 192 - 0};
uint16_t their_wing_pos_y[] = {192 - 32 - 24, 192 - 16 - 24, 192 - 16 - 24, 192 - 0 - 24, 192 - 0 - 24};

void render_ship(ship *ship, struct sp1_ss *sprite, int x, int y) {

}

void init_bullet(void) {
  bullet_sp = sp1_CreateSpr(SP1_DRAW_LOAD1LB, SP1_TYPE_1BYTE, 2, (int) (bullet_sprite + 7), 11);
  sp1_AddColSpr(bullet_sp, SP1_DRAW_LOAD1RB, SP1_TYPE_1BYTE, 0, 0);
}

void vsync(int wait){
    volatile int i = wait;
    while (i)
        --i;
}


void move_sprite(struct sp1_ss *sp, int x1, int x2, int y1, int y2, int pause, int steps)
{
    int dx, dy;
    int i;
    dx = (x2 - x1) / steps;
    dy = (y2 - y1) / steps;
    for (i = 0; i < steps - 1; ++i){
        sp1_MoveSprPix(sp, 
            &cr, 
            0, 
            x1 + i * dx, 
            y1 + i * dy
        );
        sp1_UpdateNow();
        vsync(pause);
    }
    sp1_MoveSprPix(sp, 
        &cr, 
        0, 
        x2, 
        y2
       );
    sp1_UpdateNow();
    vsync(pause);
}

void shoot_bullet(char our, char their, int reverse)
{
    int x1,y1,x2,y2;
    x1 = our_wing_pos_x[our] + 8;
    y1 = our_wing_pos_y[our] + 24;  

    x2 = their_wing_pos_x[their] + 8;
    y2 = theirgi_wing_pos_y[their] - 8;

    if (reverse)
        move_sprite(bullet_sp, x2, x1, y2, y1, 1500, 10);
    else
        move_sprite(bullet_sp, x1, x2, y1, y2, 1500, 10);
        
    sp1_MoveSprPix(bullet_sp, 
        &cr, 
        0, 
        256,
        256
    );
    sp1_UpdateNow();
    
}

#define SWAP_VSYNC 800
void swap_ships_sprites(char side, int a, int b) { 
    uint16_t y_upper, y_lower;
 
    struct sp1_ss **sp;
    uint16_t *wing_pos_x;
    uint16_t *wing_pos_y;

    struct sp1_ss *tptr;

    y_upper = 64;
    y_lower = 96;

    if (side) {
        sp = wing_sprites + 5;
        wing_pos_x = our_wing_pos_x;
        wing_pos_y = our_wing_pos_y;
    }
    else {
        sp = wing_sprites;
        wing_pos_x = their_wing_pos_x;
        wing_pos_y = their_wing_pos_y;
    }
    move_sprite(sp[a], wing_pos_x[a], wing_pos_x[a], wing_pos_y[a], y_upper, SWAP_VSYNC, 4);
    move_sprite(sp[b], wing_pos_x[b], wing_pos_x[b], wing_pos_y[b], y_lower, SWAP_VSYNC, 4);

    move_sprite(sp[a], wing_pos_x[a], wing_pos_x[b], y_upper, y_upper, SWAP_VSYNC, 4);
    move_sprite(sp[b], wing_pos_x[b], wing_pos_x[a], y_lower, y_lower, SWAP_VSYNC, 4);

    move_sprite(sp[a], wing_pos_x[b], wing_pos_x[b], y_upper, wing_pos_y[b], SWAP_VSYNC, 4);
    move_sprite(sp[b], wing_pos_x[a], wing_pos_x[a], y_lower, wing_pos_y[a], SWAP_VSYNC, 4);

    tptr = sp[a];
    sp[a] = sp[b];
    sp[b] = tptr;
}

void leader_retreat(char side, int target) {
    for (int i = 1; i <= target; ++i){
        swap_ships_sprites(side, i, i - 1);
    }
}

void init_ship_sprites(void) {
    char i;
    struct sp1_ss *s;
    for(i = 0; i < 10; i++) {
        wing_sprites[i] = s = sp1_CreateSpr(SP1_DRAW_MASK2LB, SP1_TYPE_2BYTE, 4, (int)ship21, 10);
        sp1_AddColSpr(s, SP1_DRAW_MASK2, SP1_TYPE_2BYTE, (int)ship22, 0);
        sp1_AddColSpr(s, SP1_DRAW_MASK2, SP1_TYPE_2BYTE, (int)ship23, 0);
        sp1_AddColSpr(s, SP1_DRAW_MASK2RB, SP1_TYPE_2BYTE, 0, 0);
        // sp1_AddColSpr(s, SP1_DRAW_MASK2RB, 0, 144, 0);
        // sp1_AddColSpr(s, SP1_DRAW_MASK2, 0, 48, 0);
        // sp1_AddColSpr(s, SP1_DRAW_MASK2, 0, 48, 0);
        // sp1_AddColSpr(s, SP1_DRAW_MASK2RB, 0, 144, 0);    
        // sp1_AddColSpr(s, SP1_DRAW_MASK2RB, 0, 0, 0);    
        // sp1_AddColSpr(s, SP1_DRAW_MASK2, 0, 48, 0);
        // sp1_AddColSpr(s, SP1_DRAW_MASK2, 0, 48, 0);
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
    int offset = side * 5;
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
                &cr, 
                // ship2, 
                0, 
                our_wing_pos_x[i], 
                our_wing_pos_y[i]
            );
        } else {
            sp1_MoveSprPix(
                wing_sprites[i + offset], 
                &cr, 
                // interceptor, 
                // ship2, 
                0, 
                their_wing_pos_x[i], 
                their_wing_pos_y[i]
            );
        }
    }
}



int main() {
    struct sp1_ss *s;
    wing our, their;
    char b[2];
    init_wing(&our);
    // init_wing(&their);
    // add_ship(&their, "KEK", INTERCEPTOR);
    add_ship(&our, "KEK", INTERCEPTOR);
    add_ship(&our, "LOL", BOMBER);
    add_ship(&our, "LOL", SUPPORT);
    add_ship(&our, "LOL", DESTROYER);
    add_ship(&our, "LOL", DESTROYER);

    sp1_Initialize(
        SP1_IFLAG_MAKE_ROTTBL | SP1_IFLAG_OVERWRITE_TILES | SP1_IFLAG_OVERWRITE_DFILE, 
        INK_BLACK | PAPER_WHITE, 
        ' '
    );
    sp1_Invalidate(&cr);        // invalidate entire screen so that it is all initially drawn
    // s = sp1_CreateSpr(SP1_DRAW_MASK2LB, SP1_TYPE_2BYTE, 4, 0, 0);
    init_ship_sprites();
    init_bullet();
    // sp1_AddColSpr(s, SP1_DRAW_MASK2, 0, 48, 0);
    // sp1_AddColSpr(s, SP1_DRAW_MASK2RB, 0, 0, 0);
    // sp1_IterateSprChar(s, color_ship);
    render_wing(&our, 0);
    render_wing(&our, 1);
    // sp1_MoveSprPix(s, &cr, interceptor, 16, 16);

    b[0] = 0;
    b[1] = 2;
    while(1){
        leader_retreat(0, 4);
        leader_retreat(1, 1);
        shoot_bullet(b[0], b[1], True);
        b[0] = (b[0] + 1) % 5;
      //  b[1] = (b[1] + 3) % 5;

        vsync(10000);
        
        sp1_UpdateNow();
    }

    while(1);
    return 0;
}
