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
#pragma output CLIB_MALLOC_HEAP_SIZE = 10000      // create a 3000-byte heap in BSS section

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

#define BULLET_STEP_COUNT 10
#define WAIT 1500
void shoot_bullet(char our, char their, int reverse)
{
    int x[BULLET_STEP_COUNT];
    int y[BULLET_STEP_COUNT];
    int dx, dy;
    x[0] = our_wing_pos_x[our] + 8;
    y[0] = our_wing_pos_y[our] + 24;  

    x[BULLET_STEP_COUNT - 1] = their_wing_pos_x[their] + 8;
    y[BULLET_STEP_COUNT - 1] = their_wing_pos_y[their] - 8;

    dx = their_wing_pos_x[their] - our_wing_pos_x[our];
    dy = their_wing_pos_y[their] - our_wing_pos_y[our];

    for (int i = 0; i < BULLET_STEP_COUNT - 1; ++i) {
        x[i] = x[0] +  (dx * i) / BULLET_STEP_COUNT;
        y[i] = y[0] +  (dy * i) / BULLET_STEP_COUNT;
    }

    if (reverse){
        for (int i = BULLET_STEP_COUNT - 1; i >= 0; --i) {
            sp1_MoveSprPix(bullet_sp, 
                &cr, 
                0, 
                x[i], 
                y[i]
            );
            sp1_UpdateNow();
            vsync(WAIT);
        }
    } else
    {
        for (int i = 0; i < BULLET_STEP_COUNT; ++i){
            sp1_MoveSprPix(bullet_sp, 
                &cr, 
                0, 
                x[i], 
                y[i]
            );
            sp1_UpdateNow();
            vsync(WAIT);
        }
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
    sp1_MoveSprPix(bullet_sp, &cr, 0, our_wing_pos_x[0] -6, our_wing_pos_y[1] + 2);
    // sp1_AddColSpr(s, SP1_DRAW_MASK2, 0, 48, 0);
    // sp1_AddColSpr(s, SP1_DRAW_MASK2RB, 0, 0, 0);
    // sp1_IterateSprChar(s, color_ship);
    render_wing(&our, 0);
    render_wing(&our, 1);
    // sp1_MoveSprPix(s, &cr, interceptor, 16, 16);

    b[0] = 0;
    b[1] = 2;
    while(1){
        shoot_bullet(b[0], b[1], False);
        b[0] = (b[0] + 2) % 5;
      //  b[1] = (b[1] + 3) % 5;
        
        sp1_UpdateNow();
    }

    return 0;
}
