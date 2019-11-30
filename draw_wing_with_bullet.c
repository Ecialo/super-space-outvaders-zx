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


int main() {
    struct sp1_ss *s;
    wing our, their;
    char b[2];
    init_wing(&our);
    init_wing(&their);
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
    render_wing(&our, THEIR_SIDE);
    render_wing(&our, OUR_SIDE);
    // sp1_MoveSprPix(s, &cr, interceptor, 16, 16);

     b[0] = 0;
     b[1] = 2;
    while(1){
         // into_the_hyperspace(5);
    //     //leader_retreat(0, 4);
           leader_retreat(THEIR_SIDE,  1);
           shoot_bullet(0, 0, THEIR_SIDE);
           b[0] = (b[0] + 1) % 5;
    //     b[1] = (b[1] + 3) % 5;

         vsync(10000);
        
        sp1_UpdateNow();
     }

    while(1);
    return 0;
}
