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
#include "src/icons.h"
#include "src/base_sp1.c"
#include "src/draw_wing.c"
#include "src/cursor.c"
// #include "src/utils.c"

int main() {
    wing our, their;

    init_wing(&our);
    init_wing(&their);
    // add_ship(&their, "KEK", INTERCEPTOR);
    add_ship(&our, "KEK", INTERCEPTOR);
    add_ship(&our, "LOL", BOMBER);
    add_ship(&our, "LOL", SUPPORT);
    add_ship(&our, "LOL", DESTROYER);
    add_ship(&our, "LOL", DESTROYER);
    
    add_ship(&their, "LOL", DESTROYER);
    add_ship(&their, "LOL", DESTROYER);

    init_sp1();
    // sp1_Initialize(
    //     SP1_IFLAG_MAKE_ROTTBL | SP1_IFLAG_OVERWRITE_TILES | SP1_IFLAG_OVERWRITE_DFILE, 
    //     // INK_BLACK | PAPER_WHITE, 
    //     // INK_WHITE | PAPER_BLACK, 
    //     0, 
    //     ' '
    // );
    // sp1_ClearRectInv(&cr, INK_WHITE | PAPER_BLACK, 32, SP1_RFLAG_TILE | SP1_RFLAG_COLOUR);
    // sp1_UpdateNow();
    init_icons();
    init_cursor();
    init_ship_sprites();
    // sp1_Invalidate(&full_screen);        // invalidate entire screen so that it is all initially drawn
    
    
    render_wing(&our, 0);
    render_wing(&their, 1);
    // sp1_Invalidate(&full_screen);
    sp1_UpdateNow();
    select_from_wing(&our, 0);
    // select_from_wing(&our, 1);
    while(1);
    return 0;
}
