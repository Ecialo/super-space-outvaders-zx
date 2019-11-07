#include <assert.h>
#include <stdio.h>

#include "src/ship.c"
#include "src/wing.c"
// TESTS

int test_random() {
    int i;
    unsigned char rnd_result;
    assert(rnd() == 106);
    for (i = 0; i < 255; i++) {
        rnd();
    }
    assert(rnd() == 106);
    assert(rnd() == 162);
    return OK;
}

int test_wing_ship_add() {
    wing wing1;
    ship s0, s1, s2, s3, s4;
    char a0, a1, a2, a3, a4;
    init_wing(&wing1);

    // render_wing(&wing1);    // [[*], [], [], [], []]
    assert(wing1.size == 0);
    assert(wing1.first_empty_slot == 0);
    
    add_ship(&wing1, "OLOLO", INTERCEPTOR); // [O, [*], [], [], []]
    // render_wing(&wing1);
    assert(wing1.size == 1);
    assert(wing1.first_empty_slot == 1);

    add_ship(&wing1, "KEKEKE", BOMBER); // [O, B, [*], [], []]
    // render_wing(&wing1);
    assert(wing1.size == 2);
    assert(wing1.first_empty_slot == 2);

    add_ship(&wing1, "1488", DESTROYER); // [O, B, 1, [*], []]
    // render_wing(&wing1);
    assert(wing1.size == 3);
    assert(wing1.first_empty_slot == 3);

    add_ship(&wing1, "666", SUPPORT); // [O, B, 1, 6, [*]]
    // render_wing(&wing1);
    assert(wing1.size == 4);
    assert(wing1.first_empty_slot == 4);

    add_ship(&wing1, "HITLER", INTERCEPTOR); // [O, B, 1, 6, H]
    // render_wing(&wing1);
    assert(wing1.size == 5);
    assert(wing1.first_empty_slot == NO_SLOT);

    remove_ship(&wing1, 1); // [O, [*], 1, 6, H] [O, 1, 6, H]
    // render_wing(&wing1);
    assert(wing1.size == 4);
    assert(wing1.first_empty_slot == 1);
    
    remove_ship(&wing1, 1); // [O, [], [*], 6, H] [O 6, H]
    // render_wing(&wing1);
    assert(wing1.size == 3);
    assert(wing1.first_empty_slot == 2);
    assert(wing1.ships[2].next_empty_slot == 1);

    remove_ship(&wing1, 2); // [O, [], [], 6, [*]] [O, 6]
    // render_wing(&wing1);
    assert(wing1.size == 2);
    assert(wing1.first_empty_slot == 4);
    assert(wing1.ships[4].next_empty_slot == 2);
    assert(wing1.ships[2].next_empty_slot == 1);
    assert(wing1.ships[1].next_empty_slot == NO_SLOT);

    add_ship(&wing1, "1488", DESTROYER); // [O, [], [*], 6, 1] [O, 6, 1]
    // render_wing(&wing1);
    assert(wing1.size == 3);
    assert(wing1.first_empty_slot == 2);

    remove_ship(&wing1, 0); // [[*], [], [], 6, 1] [6, 1]
    assert(wing1.size == 2);
    assert(wing1.first_empty_slot == 0);
    assert(wing1.ships[0].next_empty_slot == 2);
    
    remove_ship(&wing1, 0); // [[], [], [], [*], 1] [1]
    assert(wing1.size == 1);
    assert(wing1.first_empty_slot == 3);
    assert(wing1.ships[3].next_empty_slot == 0);

    remove_ship(&wing1, 0); // [[], [], [], [], [*]]
    assert(wing1.size == 0);
    assert(wing1.first_empty_slot == 4);
    assert(wing1.ships[4].next_empty_slot == 3);
    
}

int test_wing_swap_ships() {
    wing wing;
    init_wing(&wing);
    add_ship(&wing, "OLOLO", INTERCEPTOR);
    add_ship(&wing, "KEKEKE", BOMBER);
    add_ship(&wing, "1488", DESTROYER);
    add_ship(&wing, "666", SUPPORT);

    swap_ships(&wing, 0, 1); // [B, I, D, S]
    assert(get_ship(&wing, 0)->type == BOMBER);
    assert(get_ship(&wing, 1)->type == INTERCEPTOR);

    swap_ships(&wing, 0, 0);
    assert(get_ship(&wing, 0)->type == BOMBER);

    swap_ships(&wing, 3, 0); // [S, I, D, B]
    assert(get_ship(&wing, 0)->type == SUPPORT);
    assert(get_ship(&wing, 1)->type == INTERCEPTOR);
    assert(get_ship(&wing, 2)->type == DESTROYER);
    assert(get_ship(&wing, 3)->type == BOMBER);
}

int test_wing_cycle_ships() {
    wing wing;
    init_wing(&wing);
    add_ship(&wing, "OLOLO", INTERCEPTOR);
    add_ship(&wing, "KEKEKE", BOMBER);
    add_ship(&wing, "1488", DESTROYER);
    add_ship(&wing, "666", SUPPORT);
    // [I, B, D, S]

    cycle_ships(&wing); // [B, D, S, I]
    assert(get_ship(&wing, 0)->type == BOMBER);
    assert(get_ship(&wing, 1)->type == DESTROYER);
    assert(get_ship(&wing, 2)->type == SUPPORT);
    assert(get_ship(&wing, 3)->type == INTERCEPTOR);

    cycle_ships(&wing); // [D, S, I, B]
    assert(get_ship(&wing, 0)->type == DESTROYER);
    assert(get_ship(&wing, 1)->type == SUPPORT);
    assert(get_ship(&wing, 2)->type == INTERCEPTOR);
    assert(get_ship(&wing, 3)->type == BOMBER);

    cycle_ships(&wing); // [S, I, B, D]
    assert(get_ship(&wing, 0)->type == SUPPORT);
    assert(get_ship(&wing, 1)->type == INTERCEPTOR);
    assert(get_ship(&wing, 2)->type == BOMBER);
    assert(get_ship(&wing, 3)->type == DESTROYER);

    cycle_ships(&wing); // [I, B, D, S]
    assert(get_ship(&wing, 0)->type == INTERCEPTOR);
    assert(get_ship(&wing, 1)->type == BOMBER);
    assert(get_ship(&wing, 2)->type == DESTROYER);
    assert(get_ship(&wing, 3)->type == SUPPORT);

    cycle_ships(&wing); // [B, D, S, I]
    assert(get_ship(&wing, 0)->type == BOMBER);
    assert(get_ship(&wing, 1)->type == DESTROYER);
    assert(get_ship(&wing, 2)->type == SUPPORT);
    assert(get_ship(&wing, 3)->type == INTERCEPTOR);
}

int test_scrap_ships() {
    wing wing;
    init_wing(&wing);
    add_ship(&wing, "OLOLO", INTERCEPTOR);
    add_ship(&wing, "KEKEKE", BOMBER);
    add_ship(&wing, "1488", DESTROYER);
    add_ship(&wing, "666", SUPPORT);

    get_ship(&wing, 1)->is_alive = False;
    get_ship(&wing, 3)->is_alive = False;

    scrap_dead_ships(&wing);

    assert(wing.size == 2);
    assert(get_ship(&wing, 0)->type == INTERCEPTOR);
    assert(get_ship(&wing, 1)->type == DESTROYER);

    scrap_dead_ships(&wing);

    assert(wing.size == 2);
    assert(get_ship(&wing, 0)->type == INTERCEPTOR);
    assert(get_ship(&wing, 1)->type == DESTROYER);
}

int test_take_damage() {
    ship i, b, d, s;

    init_interceptor(&i, "Int");
    take_damage(&i, 1, 1, DESTROYER);
    assert(i.health == i.max_health - 2);
    init_interceptor(&i, "Int");
    take_damage(&i, 1, 1, INTERCEPTOR);
    assert(i.health == i.max_health - 1);
    init_interceptor(&i, "Int");
    take_damage(&i, 1, 1, BOMBER);
    assert(i.health == i.max_health - 1);
    init_interceptor(&i, "Int");
    take_damage(&i, 1, 1, SUPPORT);
    assert(i.health == i.max_health - 1);

    init_bomber(&b, "Bom");
    take_damage(&b, 1, 1, DESTROYER);
    assert(b.health == b.max_health - 1);
    init_bomber(&b, "Bom");
    take_damage(&b, 1, 1, INTERCEPTOR);
    assert(b.health == b.max_health - 2);
    init_bomber(&b, "Bom");
    take_damage(&b, 1, 1, BOMBER);
    assert(b.health == b.max_health - 1);
    init_bomber(&b, "Bom");
    take_damage(&b, 1, 1, SUPPORT);
    assert(b.health == b.max_health - 1);

    init_destroyer(&d, "Des");
    take_damage(&d, 3, 1, DESTROYER);
    assert(d.health == d.max_health - (3 - d.special));
    init_destroyer(&d, "Des");
    take_damage(&d, 3, 1, INTERCEPTOR);
    assert(d.health == d.max_health - (3 - d.special));
    init_destroyer(&d, "Des");
    take_damage(&d, 3, 1, BOMBER);
    assert(d.health == d.max_health - (6 - d.special));
    init_destroyer(&d, "Des");
    take_damage(&d, 3, 1, SUPPORT);
    assert(d.health == d.max_health - (3 - d.special));

    init_support(&s, "Sup");
    take_damage(&s, 1, 1, DESTROYER);
    assert(s.health == s.max_health - 1);
    init_support(&s, "Sup");
    take_damage(&s, 1, 1, INTERCEPTOR);
    assert(s.health == s.max_health - 1);
    init_support(&s, "Sup");
    take_damage(&s, 1, 1, BOMBER);
    assert(s.health == s.max_health - 1);
    init_support(&s, "Sup");
    take_damage(&s, 1, 1, SUPPORT);
    assert(s.health == s.max_health - 1);

    init_bomber(&b, "Bom");
    take_damage(&b, 2, 2, DESTROYER);
    assert(b.health == b.max_health - 4);
    init_bomber(&b, "Bom");
    take_damage(&b, 2, 2, INTERCEPTOR);
    assert(b.health == b.max_health - 6);

    init_bomber(&b, "Bom");
    assert(b.is_alive);
    take_damage(&b, 100, 1, DESTROYER);
    assert(b.health == 0);
    assert(!b.is_alive);

}

int test_heal() {
    ship a;
    init_interceptor(&a, "Int");
    take_damage(&a, 3, 1, INTERCEPTOR);
    heal(&a, 1);
    assert(a.health == a.max_health - 2);
    heal(&a, 5);
    assert(a.health == a.max_health);
}

int run_tests() {
    test_random();
    test_wing_ship_add();
    test_wing_swap_ships();
    test_wing_cycle_ships();
    test_scrap_ships();
    test_take_damage();
    test_heal();
    // wing wing1;
    
    // init_wing(&wing1);
    
    return OK;
}

int main() {
    run_tests();
    printf("OK\n");
}