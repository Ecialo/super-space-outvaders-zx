
#pragma output REGISTER_SP = 0xFF58
#pragma output CLIB_MALLOC_HEAP_SIZE = -0xFBFA

#include <z80.h>
#include <string.h>
#include <intrinsic.h>
#include <im2.h>
#include <arch/zx.h>
#include <stdlib.h>
#include <input.h>

#include "stdio.h"


typedef unsigned char uch;
uch glyphs16x8[][16] = {
    {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
    },
    {
        212, ~212, 212, ~212, 212, ~212, 212, ~212,
        ~212, 212, ~212, 212, ~212, 212, ~212, 212,
    },
    {
        129, ~129, 129, ~129, 129, ~129, 129, ~129,
        ~129, 129, ~129, 129, ~129, 129, ~129, 129,
    },
    {
        87, ~87, 87, ~87, 87, ~87, 87, ~87,
        ~87, 87, ~87, 87, ~87, 87, ~87, 87,
    },
    {
        68, ~68, 68, ~68, 68, ~68, 68, ~68,
        ~68, 68, ~68, 68, ~68, 68, ~68, 68,
    }
};

volatile uch vsync_state = 0;

void vsync(uch frames) {
    vsync_state = frames;
    while (vsync_state)
        ;
}

IM2_DEFINE_ISR(isr)
{
    if (vsync_state) {
        --vsync_state;
    }
}

uch glyphs16[4][32] = {
    {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0

    },
    {
        0, 0, 8, 0, 38, 0, 42, 0, 8, 0, 8, 0 , 8 , 0
    },
    {
        0,0, 8, 0, 8, 0, 8, 0, 42, 0, 38, 0, 8, 0,
    },
    {
        0, 0, 8, 0, 8, 0, (255 - 128), 0, 8, 0, 8, 0, 8, 0
    }
};

uch glyphs24[1][48] = {{0}};



#define WRITE_GLYPH(I, X, Y) { \
    uch i = I;\
    uch x = X;\
    uch y = Y;\
    for (uch j = 0; j < 16; ++j){\
        uch* s = zx_pxy2saddr(x, y + j);\
        s[0] = glyphs16[i][j * 2];\
        s[1] = glyphs16[i][j * 2 + 1];\
    }\
}

#define WRITE_BORDER(X, Y) { \
    uch x = X;\
    uch y = Y;\
    \
    uch* s = zx_pxy2saddr(x, y);\
    s[0] ^= 255;\
    s[1] ^= 255;\
    \
    for (uch j = 1; j < 15; ++j){\
        uch* s = zx_pxy2saddr(x, y + j);\
        s[0] ^= 128;\
        s[1] ^= 1;\
    }\
    \
    s = zx_pxy2saddr(x, y + 15);\
    s[0] ^= 255;\
    s[1] ^= 255;\
    \
}


#define WRITE_GLYPH_MIRRORED(I, X, Y) { \
    uch i = I;\
    uch x = X;\
    uch y = Y + 16;\
    for (uch j = 0; j < 16; ++j){\
        uch* s = zx_pxy2saddr(x, y - j);\
        s[0] = glyphs16[i][j * 2];\
        s[1] = glyphs16[i][j * 2 + 1];\
    }\
}

#define WRITE_GLYPH_W(I, X, Y) { \
    uch i = I;\
    uch x = X;\
    uch y = Y;\
    for (uch j = 0; j < 8; ++j){\
        uch* s = zx_pxy2saddr(x, y + j);\
        s[0] = glyphs16x8[i][j * 2];\
        s[1] = glyphs16x8[i][j * 2 + 1];\
    }\
}


#define WRITE_GLYPH_W_MIRRORED(I, X, Y) { \
    uch i = I;\
    uch x = X;\
    uch y = Y + 8;\
    for (uch j = 0; j < 8; ++j){\
        uch* s = zx_pxy2saddr(x, y - j);\
        s[0] = glyphs16x8[i][j * 2];\
        s[1] = glyphs16x8[i][j * 2 + 1];\
    }\
}

#define WRITE_GLYPH_L(I, X, Y) { \
    uch i = I;\
    uch x = X;\
    uch y = Y;\
    for (uch j = 0; j < 24; ++j){\
        uch* s = zx_pxy2saddr(x, y + j);\
        s[0] = glyphs246[i][j * 3];\
        s[1] = glyphs24[i][j * 3 + 1];\
        s[2] = glyphs24[i][j * 3 + 2];\
    }\
}

#define WRITE_GLYPH_L_MIRRORED(I, X, Y) { \
    uch _i = I;\
    uch _x = X;\
    uch _y = Y + 24;\
    for (uch _j = 0; _j < 24; ++_j){\
        uch* _s = zx_pxy2saddr(_x, _y - _j);\
        _s[0] = glyphs16[i][j * 3];\
        _s[1] = glyphs16[i][j * 3 + 1];\
        _s[2] = glyphs16[i][j * 3 + 2];\
    }\
}


// first 5 their then our. 0 -- empty 
uch wing_sprites_i_to_render[10] = {0};
#define SHIP_OFFSET 64
uch gl_shipPos[] = {52, 40, 28, 16, 4, 94, 106, 118, 130, 142};
void render_wings(){
        WRITE_GLYPH_W_MIRRORED(wing_sprites_i_to_render[4], 64, 4);
        WRITE_GLYPH_W_MIRRORED(wing_sprites_i_to_render[3], 64, 16);
        WRITE_GLYPH_W_MIRRORED(wing_sprites_i_to_render[2], 64, 28);
        WRITE_GLYPH_W_MIRRORED(wing_sprites_i_to_render[1], 64, 40);
        WRITE_GLYPH_W_MIRRORED(wing_sprites_i_to_render[0], 64, 52);

        WRITE_GLYPH_W(wing_sprites_i_to_render[5], 64, 94);
        WRITE_GLYPH_W(wing_sprites_i_to_render[6], 64, 106);
        WRITE_GLYPH_W(wing_sprites_i_to_render[7], 64, 118);
        WRITE_GLYPH_W(wing_sprites_i_to_render[8], 64, 130);
        WRITE_GLYPH_W(wing_sprites_i_to_render[9], 64, 142);
}

uch chooserCursor = 0;
uch chooserMax = 2;
uch chooserPos[][2] = { {96, 16}, {96, 40}, {96, 64}, {96, 88}};
const uch gl_options_pos[][2] = { {96, 16}, {96, 40}, {96, 64}, {96, 88}};

// void 

void choose() {
    chooserCursor = 0;
    WRITE_BORDER(chooserPos[chooserCursor][0], chooserPos[chooserCursor][1]);
    while (1) {
        vsync(10);
        if (in_key_pressed(IN_KEY_SCANCODE_SPACE)){
            WRITE_BORDER(chooserPos[chooserCursor][0], chooserPos[chooserCursor][1]);
            break;
        }

        if (in_key_pressed(IN_KEY_SCANCODE_w)){
            if (chooserCursor > 0){
                WRITE_BORDER(chooserPos[chooserCursor][0], chooserPos[chooserCursor][1]);
                chooserCursor--;
                WRITE_BORDER(chooserPos[chooserCursor][0], chooserPos[chooserCursor][1]);
            }
        }

        if (in_key_pressed(IN_KEY_SCANCODE_s)){
            if (chooserCursor < chooserMax) {
                WRITE_BORDER(chooserPos[chooserCursor][0], chooserPos[chooserCursor][1]);
                chooserCursor++;
                WRITE_BORDER(chooserPos[chooserCursor][0], chooserPos[chooserCursor][1]);
            }
        }
    }
}














// int main() {
//     memset( TABLE_ADDR, JUMP_POINT_HIGH_BYTE, 257 );

//     z80_bpoke( JUMP_POINT,   195 );
//     z80_wpoke( JUMP_POINT+1, (unsigned int)isr );

//     im2_init( TABLE_ADDR );
//     intrinsic_ei();

//     WRITE_GLYPH(0, 64, 64);
//     WRITE_GLYPH(1, 88, 90);

//     WRITE_GLYPH_MIRRORED(0, 32, 64);
//     WRITE_GLYPH_MIRRORED(1, 64, 90);
//     render_wings();

//     WRITE_BORDER(50, 64);
//     choose();
//     return 0;
// }