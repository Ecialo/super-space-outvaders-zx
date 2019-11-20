#include <arch/zx.h>
#include <arch/zx/sp1.h>
#include <input.h>
#include <z80.h>               // for z80_bpoke(), z80_wpoke(), im2 functions
#include <string.h>            // for memset()
#include <stdlib.h>            // for rand()
#include <intrinsic.h>         // for intrinsic_di()
#include <sound.h>             // for bit_beepfx()

#include "src/world.c"
#include "src/base_sp1.c"
#include "src/cursor.c"
#include "src/map.c"

int main() {
    // char i;
    init_sp1();
    init_all_tilesets();
    // init_map_tiles();
    init_cursor();
    // init_icons();
    generate_world();
    draw_map();
    sp1_UpdateNow();
    select_destination();

    // sp1_UpdateNow();

    while(1);
    return 0;
}
