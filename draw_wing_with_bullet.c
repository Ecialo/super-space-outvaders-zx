#include <arch/zx.h>
#include <arch/zx/sp1.h>
#include <input.h>
#include <z80.h>               // for z80_bpoke(), z80_wpoke(), im2 functions
#include <string.h>            // for memset()
#include <stdlib.h>            // for rand()
#include <intrinsic.h>         // for intrinsic_di()
#include <sound.h>             // for bit_beepfx()

#include "src/base_sp1.c"
#include "src/ship.c"
#include "src/wing.c"
#include "src/tiles.c"
#include "src/draw_wing.c"


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


void init_bullet(void) {
  bullet_sp = sp1_CreateSpr(SP1_DRAW_LOAD1LB, SP1_TYPE_1BYTE, 2, (int) (bullet_sprite + 7), 11);
  sp1_AddColSpr(bullet_sp, SP1_DRAW_LOAD1RB, SP1_TYPE_1BYTE, 0, 0);
}

void vsync(int wait){
    volatile int i = wait;
    while (i)
        --i;
}

struct sp1_Rect levelbrect  = {0, 0, 32, 24};
unsigned char levelb[32*24*3];

void drawmap(void)
{
   sp1_PutTiles(&levelbrect, (struct sp1_tp *)(levelb));
   sp1_Invalidate(&levelbrect);
}


void loadlevel(void)
{
   unsigned int p;
   unsigned int i;

   p = 0;
   i = 0;

   while (i<32*24)
   {
      levelb[p+1] = 108+(rand()&3);
      levelb[p]   = 68+(rand()&3);

      if ((i<64) || (i>703) || ((i&31)>29) || ((i&31)<2))
      {
         levelb[p+1] =107;
         levelb[p]   =86;
      }
      
      i++;
      p += 3;
   }

   p=p-15+1;
   
   levelb[p]=84;
   levelb[p+3]=73;
   levelb[p+6]=77;
   levelb[p+9]=77;
   levelb[p+12]=89;

   drawmap();
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
                &cr, 
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
            &cr, 
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

void shoot_bullet(char our, char their, int reverse)
{
    int x1,y1,x2,y2;
    x1 = our_wing_pos_x[our] + 8;
    y1 = our_wing_pos_y[our] + 24;  

    x2 = their_wing_pos_x[their] + 8;
    y2 = their_wing_pos_y[their] - 8;

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
    struct sp1_ss *sprites[MAX_WING_SIZE];
    int yOrigin[MAX_WING_SIZE];
    int yTarget[MAX_WING_SIZE];

    int steps[HYPERSPACE_STEPS];
    int *or, *tar, *tmp;
    int i, j;
    
    for (i = 0; i < count; ++i) {
        sprites[i] = wing_sprites[5 + i];
        yOrigin[i] = our_wing_pos_y[i];
    }

    for (i = 0; i < HYPERSPACE_STEPS; ++i){
        steps[i] = 15 - i * 10;
    }
    or = yOrigin;
    tar = yTarget;

    for (j = 0; j < HYPERSPACE_STEPS; ++j){
        for (int i = 0; i < count; ++i)
            tar[i] = or[i] + 100;
        move_multiple_sprites(count, sprites, our_wing_pos_x, our_wing_pos_x, or, tar, 400, steps[j]);
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

void draw_stars(){
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 24; ++j)
            sp1_PrintAtInv(j, i, INK_WHITE | PAPER_BLACK, STAR_TILES + (i + 4)  * (j + 7) % STAR_TILE_TYPES);
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
    init_sp1();
    init_all_tilesets();
    init_ship_sprites();
    init_bullet();
    draw_stars();
    // loadlevel();
    // drawmap();
    // s = sp1_CreateSpr(SP1_DRAW_MASK2LB, SP1_TYPE_2BYTE, 4, 0, 0);

    // sp1_AddColSpr(s, SP1_DRAW_MASK2, 0, 48, 0);
    // sp1_AddColSpr(s, SP1_DRAW_MASK2RB, 0, 0, 0);
    // sp1_IterateSprChar(s, color_ship);
    //render_wing(&our, 0);
    render_wing(&our, OUR_SIDE);
    // sp1_MoveSprPix(s, &cr, interceptor, 16, 16);

    // b[0] = 0;
    // b[1] = 2;
    // while(1){
    //     into_the_hyperspace(5);
    //     //leader_retreat(0, 4);
    //     //leader_retreat(1, 1);
    //     //shoot_bullet(b[0], b[1], True);
    //     //b[0] = (b[0] + 1) % 5;
    //   //  b[1] = (b[1] + 3) % 5;

    //     vsync(10000);
        
        sp1_UpdateNow();
    // }

    while(1);
    return 0;
}
