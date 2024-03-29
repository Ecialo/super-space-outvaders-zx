#ifndef __SUP_SP_BASE__
#define __SUP_SP_BASE__

#include <arch/zx.h>
#include <arch/zx/sp1.h>
#include <input.h>
#include <z80.h>               // for z80_bpoke(), z80_wpoke(), im2 functions
#include <stdlib.h>            // for rand()
#include <intrinsic.h>         // for intrinsic_di()
#include <sound.h>   

#pragma output REGISTER_SP           = 0xd000    // place stack at $d000 at startup
// #pragma output CLIB_MALLOC_HEAP_SIZE = 3000      // create a 3000-byte heap in BSS section
// #pragma output CLIB_BALLOC_TABLE_SIZE = 1
// #pragma output CRT_STACK_SIZE = 1024

// #pragma output CRT_ORG_CODE          = 32768     // org 32768
// #pragma output CRT_ORG_CODE          = 27148       // org 24500
#pragma output CRT_ORG_CODE          = 24500       // org 24500
#pragma output CLIB_EXIT_STACK_SIZE  = 0         // no atexit() functions
#pragma output CLIB_STDIO_HEAP_SIZE  = 0         // no memory for files
#pragma output CLIB_FOPEN_MAX        = -1        // do not create open files list

#define OFFSET 24

// COLORS
#define LASOR_COLOR (INK_RED | PAPER_BLACK)
#define BACK_COLOR (INK_WHITE | PAPER_BLACK)

#define INTERCEPTOR_COLOR (INK_RED | PAPER_BLACK)
#define BOMBER_COLOR (INK_YELLOW | PAPER_BLACK)
#define DESTROYER_COLOR (INK_BLUE | PAPER_BLACK)
#define SUPPORT_COLOR (INK_GREEN | PAPER_BLACK)
#define BOSS_COLOR (INK_CYAN | PAPER_BLACK)
#define FLASH_COLOR (INK_MAGENTA | PAPER_BLACK)

#define TEXT_COLOR (INK_GREEN | PAPER_BLACK)
#define OPTIONS_COLOR (INK_RED | PAPER_WHITE)
#define NUM_COLOR (INK_RED | PAPER_BLACK)
#define MAP_COLOR (INK_BLACK | PAPER_RED)
#define MAP_SELECTED_COLOR (INK_BLUE | PAPER_RED)

volatile unsigned int vsync_i;
// y, x, w, h
struct sp1_Rect full_screen = {0, 0, 32, 24};
// struct sp1_tp screen_tiles[24 * 32];

struct sp1_Rect our_wing_rect = {17, 3, 15, 7};
struct sp1_Rect env_rect = {0, 3, 15, 7};
struct sp1_Rect map_rect = {0, 0, 21, 8};

struct sp1_Rect target_inspect_rect = {0, 21, 11, 10};
struct sp1_Rect target_inspect_wing_rect = {0, 21, 11, 3};
struct sp1_Rect target_inspect_ship_rect = {3, 21, 11, 7};

struct sp1_Rect our_inspect_wing_rect = {14, 21, 11, 3};
struct sp1_Rect our_inspect_ship_rect = {17, 21, 11, 7};

struct sp1_Rect options_rect = {11, 2, 14, 2};
struct sp1_Rect costs_rect = {13, 2, 14, 1};
// struct sp1_tp env_tiles[21*8];

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


void vsync(int wait) {
    vsync_i = 0;
    while (vsync_i < wait) {
        ++vsync_i;
    }
    // wait(100);
}


// void update_screen() {
//     sp1_UpdateNow();
// }

#endif
