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


#pragma output REGISTER_SP           = 0xd000    // place stack at $d000 at startup
#pragma output CLIB_MALLOC_HEAP_SIZE = 10000      // create a 3000-byte heap in BSS section

#pragma output CRT_ORG_CODE          = 32768     // org 32768
#pragma output CLIB_EXIT_STACK_SIZE  = 0         // no atexit() functions
#pragma output CLIB_STDIO_HEAP_SIZE  = 0         // no memory for files
#pragma output CLIB_FOPEN_MAX        = -1        // do not create open files list

#define INSPECT_SHIP_X 10
#define INSPECT_SHIP_Y 10

struct sp1_pss ps0;
struct sp1_Rect cr = {0, 0, 32, 24};

void to_string(char s, char *out) {
    out[0] = s / 10 + '0';
    out[1] = s % 10 + '0';
}

void inspect_ship(ship *ship) {
    char num_holder[3];
    num_holder[2] = '\0';
    sp1_SetPrintPos(&ps0, INSPECT_SHIP_Y, INSPECT_SHIP_X);

    // INIT
    sp1_PrintString(&ps0, "\x14\x50");

    // NAME
    sp1_PrintString(&ps0, ship->name);

    // TYPE

    // ATTACK
    sp1_SetPrintPos(&ps0, INSPECT_SHIP_Y + 1, INSPECT_SHIP_X);
    to_string(ship->attack, num_holder);
    sp1_PrintString(&ps0, "ATK:");
        sp1_PrintString(&ps0, num_holder);

    // SPECIAL
    to_string(ship->special, num_holder);
    sp1_SetPrintPos(&ps0, INSPECT_SHIP_Y + 2, INSPECT_SHIP_X);
    sp1_PrintString(&ps0, "SPC:"); sp1_PrintString(&ps0, num_holder);
    
    // HP
    to_string(ship->health, num_holder);
    sp1_SetPrintPos(&ps0, INSPECT_SHIP_Y + 3, INSPECT_SHIP_X);
    sp1_PrintString(&ps0, "SHD:"); 
        sp1_PrintString(&ps0, num_holder);
        to_string(ship->max_health, num_holder);
        sp1_PrintString(&ps0, "/");
        sp1_PrintString(&ps0, num_holder);

}


int main() {
    ship ship;
    init_bomber(&ship, "LOLO");
    ps0.bounds = &cr;
    ps0.flags = SP1_PSSFLAG_INVALIDATE;
    ps0.visit = 0;

    sp1_Initialize(
        SP1_IFLAG_MAKE_ROTTBL | SP1_IFLAG_OVERWRITE_TILES | SP1_IFLAG_OVERWRITE_DFILE, 
        INK_BLACK | PAPER_WHITE, 
        ' '
    );
    sp1_Invalidate(&cr);        // invalidate entire screen so that it is all initially drawn

    inspect_ship(&ship);

    sp1_UpdateNow();
    while(1);

    return 0;
}
