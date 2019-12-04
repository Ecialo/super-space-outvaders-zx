#include <string.h>
#include "src/base_sp1.c"
#include "src/draw_wing.c"
#include "src/cursor.c"
#include "src/inspect.c"
#include "src/utils.c"
#include "src/ship.c"
#include "src/wing.c"

#define NO_MODS 0
#define TORPEDO 1
#define REMTECH 2

#define NUM_OF_ACTIONS 3
#define NO_CHOICE 7

#define OUR_SIDE 0
#define THEIR_SIDE 1


typedef enum State {
    IDLE,
    FLY,
    FIGHT,
    VICTORY,
    DEFEAT,
} state;

typedef enum Action {
    RETREAT,
    ATTACK,
    SPECIAL,
} action;

typedef enum FightResults {
    CONTINUE,
    THEY,
    WE,
} fight_result;

// GLOBAL GAME STATE
typedef struct GameState
{
    char state;
    wing player_wing;
    char combat_round;
    unsigned char money;
} game_state;

int init_state(game_state *state) {
    state->state = IDLE;
    state->current_world = 0;
    init_wing(&state->player_wing);
    state->combat_round = 0;
    state->money = 0;
    return OK;
}

// // READ
char read_action() {
    select_from_battle_options();
    return CURSOR_POS;
}

char read_ship_i(wing *wing, char side) {
    select_from_wing(wing, side);
    return CURSOR_POS;
}

// ACTIONS

fight_result check_fight_result(wing *player, wing *enemy) {
    if (player->size == 0) {
        return THEY;
    } else if (enemy->size == 0) {
        return WE;
    } else {
        return CONTINUE;
    }
}

int perform_action(wing *you, wing *oppose, action action, char choice, char damage_multiplier) {
    ship *you_leader = get_leader(you);
    ship *oppose_leader = get_leader(oppose);
    
    switch (action) {
        case ATTACK:
            take_damage(
                oppose_leader, 
                you_leader->attack, 
                damage_multiplier,
                you_leader->type
            );
            break;
        case RETREAT:
            cycle_ships(you);
            if (you_leader->type == INTERCEPTOR) {
                take_damage(
                    oppose_leader, 
                    you_leader->special, 
                    damage_multiplier,
                    NOT_A_SHIP
                );  
            }
            break;
        case SPECIAL:
            switch (you_leader->type) {
                case INTERCEPTOR:
                    swap_ships(oppose, 0, choice);
                    break;
                case BOMBER:
                    take_damage(
                        get_ship(oppose, choice), 
                        you_leader->special, 
                        damage_multiplier,
                        NOT_A_SHIP
                    );
                    break;
                case DESTROYER:
                    swap_ships(you, 0, 1);
                    // TODO RAISE DESTROYER SHIELD
                    break;  
                case SUPPORT:
                    heal(
                        get_ship(you, choice),
                        you_leader->special
                    );
                    break;
                default:
                    // assert(0);
                    break;
            }
            break;
        default:
            // assert(0);
            break;
    }
}

int perform_round(
    wing *player, 
    wing *enemy, 
    char damage_multiplier,
    action player_action,
    char player_choice,
    action enemy_action,
    char enemy_choice
) {
    char i, wing_size, result_effect;
    ship player_leader, enemy_leader;

    wing_size = player->size;
    for (i = 1; i < wing_size; i++) {
        heal(
            get_ship(player, i), 
            player->heal
        );
    }
    wing_size = enemy->size;
    for (i = 1; i < wing_size; i++) {
        heal(
            get_ship(enemy, i), 
            enemy->heal
        );
    }

    result_effect = player->heal - (enemy->missile) * 2;
    if (result_effect > 0) {
        heal(
            get_leader(player), 
            result_effect
        );
    } else if (result_effect < 0) {
        take_damage(
            get_leader(player), 
            result_effect,
            damage_multiplier,
            NOT_A_SHIP
        );
    }

    result_effect = enemy->heal - (player->missile) * 2;
    if (result_effect > 0) {
        heal(
            get_leader(enemy), 
            result_effect
        );
    } else if (result_effect < 0) {
        take_damage(
            get_leader(enemy), 
            result_effect,
            damage_multiplier,
            NOT_A_SHIP
        );
    }
    player_leader = *get_leader(player);
    enemy_leader = *get_leader(enemy);

    perform_action(player, enemy, player_action, player_choice, damage_multiplier);
    perform_action(enemy, player, enemy_action, enemy_choice, damage_multiplier);
    // TODO COMPLETE PERFORMER
}

void perform_fight(game_state *state) {
    action player_action, enemy_action; 
    char player_choice, enemy_choice;
    fight_result fight_result;
    wing PLAYER_WING;
    PLAYER_WING = state->player_wing;
    
    wing enemy_wing;
    init_enemy_wing(&enemy_wing);

    for(;;) {
        render_wing(&PLAYER_WING, OUR_SIDE);
        render_wing(&enemy_wing, THEIR_SIDE);
        inspect_ship(get_leader(&PLAYER_WING), 18, 10);
        inspect_ship(get_leader(&enemy_wing), 18, 3);

        player_action = (action) read_action();
//         printf("%d", (int) player_action);
        if (player_action == SPECIAL) {
            switch (get_leader(&PLAYER_WING)->type) {
                case BOMBER:
                case INTERCEPTOR:
                    player_choice = read_ship_i(&enemy_wing, THEIR_SIDE);
                    break;
                case SUPPORT:
                    player_choice = read_ship_i(&PLAYER_WING, OUR_SIDE);
                    break;
                default:
                    player_choice = NO_CHOICE;
                    break;
            }
        } else {
            player_choice = NO_CHOICE;
        }

        enemy_action = (action) (rnd() % NUM_OF_ACTIONS);
        if (enemy_action == SPECIAL) {
            switch (get_leader(&PLAYER_WING)->type) {
                case BOMBER:
                    enemy_choice = (char) (rnd() % PLAYER_WING.size);
                    break;
                case SUPPORT:
                case INTERCEPTOR:
                    enemy_choice = (char) (rnd() % enemy_wing.size);
                    break;
                default:
                    enemy_choice = NO_CHOICE;
                    break;
            }
        } else {
            enemy_choice = NO_CHOICE;
        }

        perform_round(
            &PLAYER_WING,
            &enemy_wing,
            (state->combat_round / 10) + 1,
            player_action,
            player_choice,
            enemy_action,
            enemy_choice
        );
        scrap_dead_ships(&PLAYER_WING);
        scrap_dead_ships(&enemy_wing);
        fight_result = check_fight_result(&PLAYER_WING, &enemy_wing);
        if (fight_result == CONTINUE) {
            state->combat_round++;
        } else if (fight_result == WE) {
            state->state = IDLE;
            break;
        } else if (fight_result == THEY) {
            state->state = DEFEAT;
            break;
        }
    }
}

// MAIN

void just_fight() {
    game_state state;
    init_state(&state);
    add_ship(&state.player_wing, "KUKAREK", DESTROYER);
    add_ship(&state.player_wing, "KUKARE2", BOMBER);
    perform_fight(&state);
}

int main() {
    init_sp1();
    init_icons();
    init_cursor();
    init_ship_sprites();
    init_inspector();

    just_fight();

    // init
    // state state;
    // init_state(&state);
    // generate_world();

    // // main game loop
    // for (a = 0; a < 10; a++) {
    //     if (state.state == FLY) { perform_fly(&state); }
    //     else if (state.state == FIGHT) { perform_fight(&state); }
    //     else if (state.state == IDLE) { perform_idle(&state); }
    //     else if (state.state = VICTORY) {
    //         render_victory();
    //         break;
    //     }
    //     printf("\n");
    // }
}