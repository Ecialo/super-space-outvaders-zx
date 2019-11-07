#ifndef __SUP_SP_BASE__
#define __SUP_SP_BASE__

#include <arch/zx.h>
#include <arch/zx/sp1.h>
#include <input.h>
#include <z80.h>               // for z80_bpoke(), z80_wpoke(), im2 functions
#include <string.h>            // for memset()
#include <stdlib.h>            // for rand()
#include <intrinsic.h>         // for intrinsic_di()
#include <sound.h>   

#pragma output REGISTER_SP           = 0xd000    // place stack at $d000 at startup
#pragma output CLIB_MALLOC_HEAP_SIZE = 8000      // create a 3000-byte heap in BSS section

#pragma output CRT_ORG_CODE          = 32768     // org 32768
#pragma output CLIB_EXIT_STACK_SIZE  = 0         // no atexit() functions
#pragma output CLIB_STDIO_HEAP_SIZE  = 0         // no memory for files
#pragma output CLIB_FOPEN_MAX        = -1        // do not create open files list


// y, x, w, h
struct sp1_Rect full_screen = {0, 0, 32, 24};

// struct sp1_Rect our_wing_rect = {17, 0, 17, 7};
// struct sp1_Rect env_rect = {0, 0, 17, 7};
// struct sp1_Rect options_rect = {8, 0, 17, 10};

void init_sp1() {
    sp1_Initialize(
        SP1_IFLAG_MAKE_ROTTBL | SP1_IFLAG_OVERWRITE_TILES | SP1_IFLAG_OVERWRITE_DFILE, 
        // INK_BLACK | PAPER_WHITE, 
        // INK_WHITE | PAPER_BLACK,
        0, 
        ' '
    );
    sp1_Invalidate(&full_screen);
}

void update_screen() {
    sp1_UpdateNow();
}

#endif
