#include "../data/money_tile.h"
#include "../data/skip_node_tile.h"
#include "../data/boss_tile.h"
#include "../data/easy_tile.h"
#include "../data/norm_tile.h"
#include "../data/hard_tile.h"
#include "../data/compact_arr_tile.h"

#define COMPACT_ARR_TILES 128

#define NO_OUT 2

#define ALL_CON_TOP 128
#define ALL_CON_BOT 129
#define NO_MID_TOP 132
#define NO_MID_BOT 133
#define ONLY_MID_TOP 136
#define ONLY_MID_BOT 137
#define NO_TOP 140
#define NO_BOT 141

#define CREDIT_TILES 144
#define SKIP_NODE_TILES (144+4*1)
#define BOSS_NODE_TILES (144+4*2)
#define EASY_NODE_TILES (144+4*3)
#define NORM_NODE_TILES (144+4*4)
#define HARD_NODE_TILES (144+4*5)

#define LEFT_TOP_TILE 0
#define RIGHT_TOP_TILE 2
#define LEFT_BOT_TILE 1
#define RIGHT_BOT_TILE 3
