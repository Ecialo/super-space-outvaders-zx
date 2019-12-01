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

int main() {
    struct sp1_ss *s;
    wing our, their;
    init_wing(&our);
    init_wing(&their);
    // add_ship(&their, "KEK", INTERCEPTOR);
    add_ship(&our, "KEK", INTERCEPTOR);
    promote_ship(get_ship(&our, 0));
    add_ship(&our, "LOL", BOMBER);
    promote_ship(get_ship(&our, 1));

    add_ship(&our, "LOL", SUPPORT);
    promote_ship(get_ship(&our, 2));

    add_ship(&our, "LOL", DESTROYER);
    add_ship(&our, "LOL", DESTROYER);
    promote_ship(get_ship(&our, 4));

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


    while(1){
    //     //leader_retreat(0, 4);
           leader_retreat(THEIR_SIDE,  1);
           shoot_bullet(0, 0, THEIR_SIDE);
                     into_the_hyperspace(5);

        //   b[0] = (b[0] + 1) % 5;
    //     b[1] = (b[1] + 3) % 5;

   //      vsync(10000);
        
        sp1_UpdateNow();
     }

  //  while(1);
    return 0;
}
