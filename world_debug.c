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
    char i = 0;
    volatile int v;
    init_sp1();
    init_all_tilesets();
    // init_map_tiles();
    init_cursor();
    // init_icons();
    while (1){
        select_map(i);
        i = 1 - i;
        generate_world();
        draw_map();
        sp1_UpdateNow();
        v = 5000;
        while(v--);
    }
    select_destination();

    // sp1_UpdateNow();

    while(1);
}
