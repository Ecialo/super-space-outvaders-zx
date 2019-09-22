#include <z80.h>
#include <string.h>
#include <arch/zx.h>

typedef unsigned char uch;
uch glyphs16[2][32] = {
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
    uch i = I;\
    uch x = X;\
    uch y = Y + 24;\
    for (uch j = 0; j < 24; ++j){\
        uch* s = zx_pxy2saddr(x, y - j);\
        s[0] = glyphs16[i][j * 3];\
        s[1] = glyphs16[i][j * 3 + 1];\
        s[2] = glyphs16[i][j * 3 + 2];\
    }\
}



int main() {
    WRITE_GLYPH(0, 64, 64);
    WRITE_GLYPH(1, 88, 90);

    WRITE_GLYPH_MIRRORED(0, 32, 64);
    WRITE_GLYPH_MIRRORED(1, 64, 90);

    return 0;
}