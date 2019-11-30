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
#include "src/cursor.c"
#include "src/bonus.c"
#include "src/inspect.c"

int main() {
    wing our;
    bonus b[] = {
        ATK,
        ATK,
        HEAL,
        UPG,
        NEW
    }; // 5
    init_wing(&our);
    add_ship(&our, "LOL", DESTROYER);
    init_sp1();
    init_inspector();
    init_bonus_wing();
    init_all_tilesets();
    init_cursor();
    init_ship_sprites();
    // sp1_Invalidate(&full_screen);        // invalidate entire screen so that it is all initially drawn
    
    
    render_wing(&our, 0);
    collect_bonuses(b, 5, &our, 1);
    // render_wing(&their,  1);
    // sp1_Invalidate(&full_screen);
    // select_from_wing(&our, 1);
    while(1);
    return 0;
}
