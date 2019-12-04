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
// #include "src/draw_wing.c"
// #include "src/cursor.c"
// #include "src/bonus.c"
#include "src/inspect.c"
#include "src/utils.c"

int main() {
    uch a;
    init_sp1();
    init_inspector();
    
    select_seed();
    a = rand() % 100;
    to_string(a);
    sp1_SetPrintPos(&ps0, 10, 10);

    // INIT
    sp1_PrintString(&ps0, "\x14\x50");

    // NAME
    sp1_PrintString(&ps0, num_holder);
    sp1_UpdateNow();
    
    while(1);
    return 0;
}
