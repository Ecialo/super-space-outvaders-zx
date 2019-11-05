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
#include "src/icons.h"
// #include "src/utils.c"

#pragma output REGISTER_SP           = 0xd000    // place stack at $d000 at startup
#pragma output CLIB_MALLOC_HEAP_SIZE = 12000      // create a 3000-byte heap in BSS section

#pragma output CRT_ORG_CODE          = 32768     // org 32768
// #pragma output CRT_ORG_CODE          = 24500      // org 32768
#pragma output CLIB_EXIT_STACK_SIZE  = 0         // no atexit() functions
#pragma output CLIB_STDIO_HEAP_SIZE  = 0         // no memory for files
#pragma output CLIB_FOPEN_MAX        = -1        // do not create open files list

typedef enum CursorOption {
    ATTACK,
    FLEE,
    SPECIAL
} cursor_option;

// unsigned char  joys;
// udk_t          joykeys;

// #ifdef __SDCC
// uint16_t (*joyfunc)(udk_t *);  // pointer to joystick function
// #endif

// #ifdef __SCCZ80
// void *joyfunc;                 // pointer to joystick function
// #endif

struct sp1_ss *cursor_options[3];
struct sp1_ss *cursor;
struct sp1_Rect cr = {0, 0, 32, 24};

void init_icons_and_cursor() {
    char i;
    struct sp1_ss *s;
    for (i = 0; i < 3; i++) {
        cursor_options[i] = s = sp1_CreateSpr(SP1_DRAW_MASK2LB, SP1_TYPE_2BYTE, 3, 0, 1);
        sp1_AddColSpr(s, SP1_DRAW_MASK2, SP1_TYPE_2BYTE, icon_offset, 1);
        sp1_AddColSpr(s, SP1_DRAW_MASK2RB, SP1_TYPE_2BYTE, 0, 1);
    }
    cursor = s = sp1_CreateSpr(SP1_DRAW_MASK2LB, SP1_TYPE_2BYTE, 3, 0, 0);
    sp1_AddColSpr(s, SP1_DRAW_MASK2, SP1_TYPE_2BYTE, icon_offset, 0);
    sp1_AddColSpr(s, SP1_DRAW_MASK2RB, SP1_TYPE_2BYTE, 0, 0);
}

void color(unsigned int count, struct sp1_cs *cs) {
     cs->attr = INK_BLUE | PAPER_GREEN;
}

void draw_options(cursor_option *options) {
    unsigned char i;
    // cursor_option o;
    for (i = 0; i < 3; i++) {
        // o = options[i];
        switch(options[i]) {
            case ATTACK:
                sp1_MoveSprPix(
                    cursor_options[i], 
                    &cr, 
                    attack_ic1, 
                    // 0, 
                    16 + 16*i, 
                    16
                );
                break;
            case FLEE:
                sp1_MoveSprPix(
                    cursor_options[i], 
                    &cr, 
                    flee_ic1, 
                    // 0, 
                    16 + 16*i, 
                    16
                );
                break;
            case SPECIAL:
                sp1_MoveSprPix(
                    cursor_options[i], 
                    &cr, 
                    special_ic1, 
                    // 0, 
                    16 + 16*i, 
                    16
                );
                break;
        }
    }
}

// void setupkeyboard(void) 
// {
//    joyfunc = in_stick_keyboard;

//    joykeys.fire  = IN_KEY_SCANCODE_SPACE;
//    joykeys.left  = IN_KEY_SCANCODE_a;
//    joykeys.right = IN_KEY_SCANCODE_d;
//    joykeys.up    = IN_KEY_SCANCODE_w;
//    joykeys.down  = IN_KEY_SCANCODE_s;
// }


void select_from_options(cursor_option *options) {
    char pos = 1;
    unsigned int l, r, e;
    draw_options(options);
    sp1_MoveSprPix(
        cursor, 
        &cr, 
        ramka1, 
        // 0, 
        16 + 16 * pos, 
        16
    );
    sp1_UpdateNow();
    while(1) {
        // joys = (joyfunc) (&joykeys);
        in_wait_nokey();
        in_wait_key();


        if (in_key_pressed(IN_KEY_SCANCODE_a) && pos > 0) {
        // if (l && pos > 0) {
            pos--;
        } else if (in_key_pressed(IN_KEY_SCANCODE_d) && pos < 3) {
        // } else if (r && pos < 3) {
            pos++;
        }
        sp1_MoveSprPix(
            cursor, 
            &cr, 
            ramka1, 
            // 0, 
            16 + 16 * pos, 
            16
        );
        sp1_UpdateNow();
    }
}

int main() {
    cursor_option o[] = {ATTACK, FLEE, SPECIAL};
    sp1_Initialize(
        SP1_IFLAG_MAKE_ROTTBL | SP1_IFLAG_OVERWRITE_TILES | SP1_IFLAG_OVERWRITE_DFILE, 
        INK_BLACK | PAPER_WHITE, 
        ' '
    );
    init_icons_and_cursor();
    sp1_Invalidate(&cr);        // invalidate entire screen so that it is all initially drawn
    select_from_options(o);
    // draw_options(o);
    // sp1_IterateSprChar(cursor_options[0], color);
    // sp1_MoveSprPix(
    //     cursor_options[0], 
    //     &cr, 
    //     // 0, 
    //     special_ic1, 
    //     16, 
    //     16
    // );
    // // z80_delay_ms(10);
    // sp1_UpdateNow();
    while(1);
    return 0;
}
