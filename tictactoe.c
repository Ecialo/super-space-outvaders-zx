#pragma output REGISTER_SP = 0xFF58
#pragma output CLIB_MALLOC_HEAP_SIZE = -0xFBFA

#include <z80.h>
#include <string.h>
#include <intrinsic.h>
#include <im2.h>
#include <arch/zx.h>
#include <stdlib.h>
#include <input.h>



#define TABLE_HIGH_BYTE        ((unsigned int)0xfc)
#define JUMP_POINT_HIGH_BYTE   ((unsigned int)0xfb)

#define UI_256                 ((unsigned int)256)

#define TABLE_ADDR             ((void*)(TABLE_HIGH_BYTE*UI_256))
#define JUMP_POINT             ((unsigned char*)( (unsigned int)(JUMP_POINT_HIGH_BYTE*UI_256) + JUMP_POINT_HIGH_BYTE ))


typedef unsigned char uch;

uch cursor[2] = { 0, 0 };
uch glyphs[2][8] = {
                {24, 36, 66, 129, 129, 66, 36, 24},
                {129, 66, 36, 24, 24, 36, 66, 129}
            };


volatile uch vsync_state = 0;

void vsync(uch frames) {
    vsync_state = frames;
    while (vsync_state)
        ;
}

void draw_cursor(uch fill) {
    uch base[] = { cursor[0] * 16, cursor[1] * 16 };
    uch b[2];
    for (uch i = 0; i < 3; ++i){
        for (uch j = 0; j < 3; ++j){
            if (i != j || i != 1) {
                b[0] = base[0] + i * 8;
                b[1] = base[1] + j * 8;
                *zx_pxy2saddr(b[0], b[1] + 0) = fill;
                *zx_pxy2saddr(b[0], b[1] + 1) = fill;
                *zx_pxy2saddr(b[0], b[1] + 2) = fill;
                *zx_pxy2saddr(b[0], b[1] + 3) = fill;
                *zx_pxy2saddr(b[0], b[1] + 4) = fill;
                *zx_pxy2saddr(b[0], b[1] + 5) = fill;
                *zx_pxy2saddr(b[0], b[1] + 6) = fill;
                *zx_pxy2saddr(b[0], b[1] + 7) = fill;
            }
        }
    }
}


#define W 4
void draw_marker(uch id){
    uch base[] = { cursor[0] * 16 + 8, cursor[1] * 16 + 8};
    uch* glyph = glyphs[id];

    *zx_pxy2saddr(base[0], base[1] + 0) = glyph[0];
    vsync(W);

    *zx_pxy2saddr(base[0], base[1] + 1) = glyph[1];
    vsync(W);

    *zx_pxy2saddr(base[0], base[1] + 2) = glyph[2];
    vsync(W);
    *zx_pxy2saddr(base[0], base[1] + 3) = glyph[3];
    vsync(W);
    *zx_pxy2saddr(base[0], base[1] + 4) = glyph[4];
    vsync(W);
    *zx_pxy2saddr(base[0], base[1] + 5) = glyph[5];
    vsync(W);
    *zx_pxy2saddr(base[0], base[1] + 6) = glyph[6];
    vsync(W);
    *zx_pxy2saddr(base[0], base[1] + 7) = glyph[7];
    vsync(W);
}

IM2_DEFINE_ISR(isr)
{
    if (vsync_state) {
        --vsync_state;
    }
}


signed char dir[2] = { 0, 0 };
uch pressed = 2;
uch active_player = 0;
uch field[3][3] = {{2,2,2}, {2,2,2}, {2,2,2}};
void getControls() {
        dir[0] = dir[1] = 0;
        pressed = 0;

        if (in_key_pressed(IN_KEY_SCANCODE_a) && cursor[0] > 0)
            dir[0] = -1;
        if (in_key_pressed(IN_KEY_SCANCODE_d) && cursor[0] < 2)
            dir[0] = 1;
        if (in_key_pressed(IN_KEY_SCANCODE_w) && cursor[1] > 0)
            dir[1] = -1;
        if (in_key_pressed(IN_KEY_SCANCODE_s) && cursor[1] < 2)
            dir[1] = 1;
        if (in_key_pressed(IN_KEY_SCANCODE_l))
            pressed = 1;
}

uch game_finished(){
    uch res;
    for (uch i = 0; i < 3; ++i){
        res = 1;
        for (uch j = 0; j < 3; ++j){
            res = res &&  field[i][j] == active_player;
        }
        if (res)
            return res;
    }

    for (uch i = 0; i < 3; ++i){
        res = 1;
        for (uch j = 0; j < 3; ++j){
            res = res && field[j][i] == active_player;
        }
        if (res)
            return res;
    }

    res = 1;
    for (uch i = 0; i < 3; ++i){
        res = res && field[i][i] == active_player;
    }
    if (res)
        return res;

    res = 1;
    for (uch i = 0; i < 3; ++i){
        res = res && field[i][2 - i] == active_player;
    }
    if (res)
        return res;

    return 0;
}

int main() {
    uch c = 0;
    memset( TABLE_ADDR, JUMP_POINT_HIGH_BYTE, 257 );

    z80_bpoke( JUMP_POINT,   195 );
    z80_wpoke( JUMP_POINT+1, (unsigned int)isr );

    im2_init( TABLE_ADDR );
    intrinsic_ei();

    draw_cursor(255);
    while (1){
        vsync(1);
        if (c > 4 && c < 7)
            getControls();
        ++c;
        if (c < 8)
            continue;
        c = 0;

        if (pressed) {
            if (field[cursor[0]][cursor[1]] == 2) {
                field[cursor[0]][cursor[1]] = active_player;
                draw_marker(active_player);
                if (game_finished())
                    break;
                active_player = 1 - active_player;
            }
        } else {
            if (dir[0] || dir[1]){
                draw_cursor(0);
                cursor[0] += dir[0];
                cursor[1] += dir[1];
                draw_cursor(255);
            }
        }
    }
    return 0;
}