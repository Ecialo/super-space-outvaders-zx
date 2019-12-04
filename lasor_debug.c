#include <arch/zx.h>
#include <arch/zx/sp1.h>
#include <input.h>
#include <z80.h>               // for z80_bpoke(), z80_wpoke(), im2 functions
#include <string.h>            // for memset()
#include <stdlib.h>            // for rand()
#include <intrinsic.h>         // for intrinsic_di()
#include <sound.h>             // for bit_beepfx()

#include "src/base_sp1.c"

typedef unsigned char uch;

uch tile_buffer[8] = {0, 0, 0, 0, 0, 0, 0, 0};
uch c_t_x = 0;
uch c_t_y = 0;
uch l_t_x = 0;
uch l_t_y = 0;
int a_t = 144; 
void putpix(int x, int y) {
    uch t_x = x / 8;
    uch t_y = y / 8;
    if (t_x != c_t_x || t_y != c_t_y) {
        sp1_TileEntry(a_t, tile_buffer);
        sp1_PrintAtInv(l_t_y, l_t_x, INK_WHITE | PAPER_BLACK, ' ');
        sp1_PrintAtInv(c_t_y, c_t_x, INK_WHITE | PAPER_BLACK, a_t);
        sp1_UpdateNow();
        l_t_x = c_t_x;
        l_t_y = c_t_y;
        c_t_x = t_x;
        c_t_y = t_y;
        memset(tile_buffer, 0, 8);
        // a_t++;
    }
    tile_buffer[y % 8] =  1 << (7 - (x % 8));
}
// void shoot_lasor(int f_x, int f_y, int t_x, int t_y, uch color) {
void shoot_lasor(int x0, int y0, int x1,int y1) {
    int dx, dy, err, e2;
    int sx, sy;
    dx =  abs(x1-x0);
    sx = x0<x1 ? 1 : -1;
    dy = -abs(y1-y0);
    sy = y0<y1 ? 1 : -1;
    err = dx + dy;  /* error value e_xy */
    while (1) {   /* loop */
        if (x0==x1 && y0==y1) break;
        e2 = 2*err;
        if (e2 >= dy) {
            err += dy; /* e_xy+e_x > 0 */
            x0 += sx;
        }
        if (e2 <= dx) { /* e_xy+e_y < 0 */
            err += dx;
            y0 += sy;
        }
        putpix(x0, y0);
        vsync(200);
    }
}

const uint8_t stars[] = {
    0, 0, 0, 2, 0, 0, 128, 0, 0, 0, 0, 0, 4, 0, 0, 2, 0, 0, 0, 4, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

uch *head;

int main() {
    init_sp1();
    // tile_buffer[1] = 8;
    // tile_buffer[2] = 16;
    // head = stars;
    // sp1_TileEntry(a_t, head);
    // sp1_PrintAtInv(10, 10, INK_WHITE | PAPER_BLACK, a_t);
    // sp1_UpdateNow();

    // head += 8;
    // sp1_TileEntry(a_t, head);
    // sp1_PrintAtInv(12, 12, INK_WHITE | PAPER_BLACK, a_t);
    // sp1_UpdateNow();
    shoot_lasor(115, 33, 31, 10);
}
