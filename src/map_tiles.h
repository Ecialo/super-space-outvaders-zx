#include "../data/money_tile.h"
#include "../data/skip_node_tile.h"
#include "../data/compact_arr_tile.h"

// #include "../data/con_arr_tile.h"
// #include "../data/down_arr_tile.h"
// #include "../data/down_mid_arr_tile.h"
// #include "../data/hb_mid_arr_tile.h"
// #include "../data/ht_mid_arr_tile.h"
// #include "../data/up_arr_tile.h"
// #include "../data/up_mid_arr_tile.h"


#define COMPACT_ARR_TILES 128

#define NO_OUT 2

#define ALL_CON_TOP 128
#define ALL_CON_BOT 129
// #define ALL_CON_TOP_NO_OUT 130
// #define ALL_CON_BOT_NO_OUT 131
#define NO_MID_TOP 132
#define NO_MID_BOT 133
// #define NO_MID_TOP_NO_OUT 134
// #define NO_MID_BOT_NO_OUT 135
#define ONLY_MID_TOP 136
#define ONLY_MID_BOT 137
// #define ONLY_MID_TOP_NO_OUT 138
// #define ONLY_MID_BOT_NO_OUT 139
#define NO_TOP 140
#define NO_BOT 141
// #define NO_TOP_NO_OUT 142
// #define NO_BOT_NO_OUT 143

#define CREDIT_TILES 144
#define SKIP_NODE_TILES (144+4*1)
// #define DOWN_ARR_TILES (128+4*2)
// #define HB_MID_ARR_TILES (128+4*3)
// #define HT_MID_ARR_TILES (128+4*4)
// #define UP_ARR_TILES (128+4*5)
// #define UP_MID_ARR_TILES (128+4*6)

#define LEFT_TOP_TILE 0
#define RIGHT_TOP_TILE 2
#define LEFT_BOT_TILE 1
#define RIGHT_BOT_TILE 3
