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
#include "src/inspect.c"

int main() {
    ship ship;
    init_bomber(&ship, "LOLO");
    // ps0.bounds = &cr;
    // ps0.flags = SP1_PSSFLAG_INVALIDATE;
    // ps0.visit = 0;

    sp1_Initialize(
        SP1_IFLAG_MAKE_ROTTBL | SP1_IFLAG_OVERWRITE_TILES | SP1_IFLAG_OVERWRITE_DFILE, 
        INK_BLACK | PAPER_WHITE, 
        ' '
    );
    init_inspector();
    sp1_Invalidate(&full_screen);        // invalidate entire screen so that it is all initially drawn

    inspect_ship(&ship, 10, 10);

    sp1_UpdateNow();
    while(1);

    return 0;
}
