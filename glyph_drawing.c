#include <z80.h>
#include <string.h>
#include <arch/zx.h>

typedef unsigned char uch;
uch glyphs16x8[3][16] = {
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
    }
};

uch glyphs16[3][32] = {
    {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0

    },
    {
        212, ~212, 212, ~212, 212, ~212, 212, ~212,
        ~212, 212, ~212, 212, ~212, 212, ~212, 212,
        212, ~212, 212, ~212, 212, ~212, 212, ~212,
        ~212, 212, ~212, 212, ~212, 212, ~212, 212
    },
    {
        129, ~129, 129, ~129, 129, ~129, 129, ~129,
        ~129, 129, ~129, 129, ~129, 129, ~129, 129,
        129, ~129, 129, ~129, 129, ~129, 129, ~129,
        ~129, 129, ~129, 129, ~129, 129, ~129, 129
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
        s[0] = glyphs16[i][j * 2];\
        s[1] = glyphs16[i][j * 2 + 1];\
    }\
}


#define WRITE_GLYPH_W_MIRRORED(I, X, Y) { \
    uch i = I;\
    uch x = X;\
    uch y = Y + 8;\
    for (uch j = 0; j < 8; ++j){\
        uch* s = zx_pxy2saddr(x, y - j);\
        s[0] = glyphs16[i][j * 2];\
        s[1] = glyphs16[i][j * 2 + 1];\
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


uch wings[10] = {1, 2, 1, 2, 1, 1, 1 ,1, 1, 1};
void render_wings(){
        WRITE_GLYPH_W_MIRRORED(wings[0], 64, 4);
        WRITE_GLYPH_W_MIRRORED(wings[1], 64, 16);
        WRITE_GLYPH_W_MIRRORED(wings[2], 64, 28);
        WRITE_GLYPH_W_MIRRORED(wings[3], 64, 40);
        WRITE_GLYPH_W_MIRRORED(wings[4], 64, 52);

        WRITE_GLYPH_W(wings[5], 64, 94);
        WRITE_GLYPH_W(wings[6], 64, 106);
        WRITE_GLYPH_W(wings[7], 64, 118);
        WRITE_GLYPH_W(wings[8], 64, 130);
        WRITE_GLYPH_W(wings[9], 64, 142);
}



int main() {
    WRITE_GLYPH(0, 64, 64);
    WRITE_GLYPH(1, 88, 90);

    WRITE_GLYPH_MIRRORED(0, 32, 64);
    WRITE_GLYPH_MIRRORED(1, 64, 90);
    render_wings();
    return 0;
}