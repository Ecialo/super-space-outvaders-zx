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
#include "src/base_sp1.c"
#include "src/draw_wing.c"


int main() {
    struct sp1_ss *s;

    wing our, their;

    init_wing(&our);
    // init_wing(&their);
    // add_ship(&their, "KEK", INTERCEPTOR);
    add_ship(&our, "KEK", INTERCEPTOR);
    add_ship(&our, "LOL", BOMBER);
    add_ship(&our, "LOL", SUPPORT);
    add_ship(&our, "LOL", DESTROYER);
    add_ship(&our, "LOL", DESTROYER);

    init_sp1();
    // s = sp1_CreateSpr(SP1_DRAW_MASK2LB, SP1_TYPE_2BYTE, 4, 0, 0);
    init_ship_sprites();
    
    // sp1_AddColSpr(s, SP1_DRAW_MASK2, 0, 48, 0);
    // sp1_AddColSpr(s, SP1_DRAW_MASK2RB, 0, 0, 0);
    // sp1_IterateSprChar(s, color_ship);
    render_wing(&our, OUR_SIDE);
    render_wing(&our, THEIR_SIDE);
    // sp1_MoveSprPix(s, &cr, interceptor, 16, 16);


    sp1_UpdateNow();
    while(1);

    return 0;
}
