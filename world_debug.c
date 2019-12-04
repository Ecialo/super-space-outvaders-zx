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
    select_map(rand() % MAPS_COUNT);
    generate_world();
    // init_icons();
    // while (1){
    //     select_map(i);
    //     i = 1 - i;
    //     generate_world();
    //     draw_map();
    //     sp1_UpdateNow();
    //     v = 5000;
    //     while(v--);
    // }
    while (world[current_world].num_of_next_worlds > 0) {
        clear_screen_from_map();
        draw_map();
        sp1_UpdateNow();
        select_destination();
        clear_screen_from_cursor();
        current_world = world[current_world].next_worlds[CURSOR_POS];
        world_x_offset = nodes_x[current_world];
        // sp1_PrintAtInv(22, 20, INK_WHITE | PAPER_BLACK, '0' + world_x_offset);
    }
    // sp1_UpdateNow();

    while(1);
}
