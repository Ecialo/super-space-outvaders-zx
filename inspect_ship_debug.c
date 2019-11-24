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
    wing wing;
    ship *ship;
    char i;
    init_wing(&wing);
    ship = get_leader(&wing);
    add_ship(&wing, "OLOLO", BOMBER);
    for (i = 0; i < 7; i++) {
        install_mod(&wing, ship, ALL_MODS[i]);
    }
    // ps0.bounds = &cr;
    // ps0.flags = SP1_PSSFLAG_INVALIDATE;
    // ps0.visit = 0;

    init_sp1();
    init_inspector();
    init_all_tilesets();

    // inspect_ship(ship, 10, 10);
    inspect_wing(&wing, &our_inspect_wing_rect, &our_inspect_ship_rect);
    inspect_wing(&wing, &target_inspect_wing_rect, &target_inspect_ship_rect);

    sp1_UpdateNow();
    while(1);

    return 0;
}
