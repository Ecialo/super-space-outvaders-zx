#!/bin/bash

source $HOME/.bashrc

function generate_graphics {
    # python3 ./png2sp1sprite.py data/grph/ship2.png -i ship2 > data/ship2.asm
    # python3 ./png2sp1sprite.py data/grph/sprites/interceptor_1.png -i interceptor_1 --bit > data/i1.asm
    # python3 ./png2sp1sprite.py data/grph/sprites/destroyer-1.png -i destroyer_1 --bit > data/d1.asm
    # python3 ./png2sp1sprite.py data/grph/sprites/bomber-1.png -i bomber_1 --bit > data/b1.asm
    # python3 ./png2sp1sprite.py data/grph/sprites/support-1.png -i support_1 --bit > data/s1.asm

    python3 ./png2ugd.py data/grph/sprites/interceptor_1.png -i interceptor_1 -a > data/i1.asm
    python3 ./png2ugd.py data/grph/sprites/destroyer-1.png -i destroyer_1 -a > data/d1.asm
    python3 ./png2ugd.py data/grph/sprites/bomber-1.png -i bomber_1 -a > data/b1.asm
    python3 ./png2ugd.py data/grph/sprites/support-1.png -i support_1 -a > data/s1.asm

    # python3 ./png2sp1sprite.py data/grph/sprites/interceptor_2.png -i interceptor_2 --bit > data/i2.asm
    # python3 ./png2sp1sprite.py data/grph/sprites/destroyer-2.png -i destroyer_2 --bit > data/d2.asm
    # python3 ./png2sp1sprite.py data/grph/sprites/bomber-2.png -i bomber_2 --bit > data/b2.asm
    # python3 ./png2sp1sprite.py data/grph/sprites/support-2.png -i support_2 --bit > data/s2.asm
    
    python3 ./png2ugd.py data/grph/sprites/interceptor_2.png -i interceptor_2 -a > data/i2.asm
    python3 ./png2ugd.py data/grph/sprites/destroyer-2.png -i destroyer_2 -a > data/d2.asm
    python3 ./png2ugd.py data/grph/sprites/bomber-2.png -i bomber_2 -a > data/b2.asm
    python3 ./png2ugd.py data/grph/sprites/support-2.png -i support_2 -a > data/s2.asm

    # python3 ./png2sp1sprite.py data/grph/sprites/boss1.png -i boss_1 --bit > data/boss_1.asm
    # python3 ./png2sp1sprite.py data/grph/sprites/boss2.png -i boss_2 --bit > data/boss_2.asm

    python3 ./png2ugd.py data/grph/sprites/boss1.png -i boss_1 -a > data/boss_1.asm
    python3 ./png2ugd.py data/grph/sprites/boss2.png -i boss_2 -a > data/boss_2.asm
    
    python3 ./png2sp1sprite.py data/grph/sprites/shield.png -i shield_sp --bit > data/shield_sp.asm


    # python3 ./png2sp1sprite.py data/grph/attack.png -i attack_ic > data/attack_icon.asm
    # python3 ./png2sp1sprite.py data/grph/flee.png -i flee_ic > data/flee_icon.asm
    # python3 ./png2sp1sprite.py data/grph/special.png -i special_ic > data/special_icon.asm
    
    # python3 ./png2ugd.py data/grph/boss.png -a -i boss_ic > data/boss_tile.asm
    # python3 ./png2ugd.py data/grph/credit.png -a -i money_ic > data/money_tile.asm
    
    python3 ./png2sp1sprite.py data/grph/sprites/ramka_lb.png -i ramka_lb > data/ramka_lb.asm
    python3 ./png2sp1sprite.py data/grph/sprites/ramka_rb.png -i ramka_rb > data/ramka_rb.asm
    python3 ./png2sp1sprite.py data/grph/sprites/ramka_lt.png -i ramka_lt > data/ramka_lt.asm
    python3 ./png2sp1sprite.py data/grph/sprites/ramka_rt.png -i ramka_rt > data/ramka_rt.asm
    python3 ./png2sp1sprite.py data/grph/sprites/bullet.png -i bullet > data/bullet.asm
}

function generate_tiles {
        # python3 ./png2ugd.py data/grph/icons/angel-wings.png -i rebirth_ic > data/rebirth.h
        python3 ./png2ugd.py data/grph/icons/angel-wings.png -i rebirth_ic -a > data/rebirth.asm
        # python3 ./png2ugd.py data/grph/icons/attack.png -i attack_ic > data/attack.h
        python3 ./png2ugd.py data/grph/icons/attack.png -i attack_ic -a > data/attack.asm
        # python3 ./png2ugd.py data/grph/icons/boss.png -i boss > data/boss_tile.h
        python3 ./png2ugd.py data/grph/icons/boss.png -i boss -a > data/boss_tile.asm
        # python3 ./png2ugd.py data/grph/icons/compact_arr.png -i compact_arr -a > data/compact_arr_tile.h
        python3 ./png2ugd.py data/grph/icons/compact_arr.png -i compact_arr -a > data/compact_arr_tile.asm
        # python3 ./png2ugd.py data/grph/icons/credit.png -i money_ic > data/money_tile.h
        python3 ./png2ugd.py data/grph/icons/credit.png -i money_ic -a > data/money_tile.asm
        # python3 ./png2ugd.py data/grph/icons/cycle.png -i retreat_ic > data/retreat.h
        python3 ./png2ugd.py data/grph/icons/cycle.png -i retreat_ic -a > data/retreat.asm
        # python3 ./png2ugd.py data/grph/icons/forward-field.png -i hp_mod_ic > data/hp_mod.h
        python3 ./png2ugd.py data/grph/icons/forward-field.png -i hp_mod_ic -a > data/hp_mod.asm
        # python3 ./png2ugd.py data/grph/icons/morbid-humour.png -i norm > data/norm_tile.h
        python3 ./png2ugd.py data/grph/icons/morbid-humour.png -i norm -a > data/norm_tile.asm
        # python3 ./png2ugd.py data/grph/icons/piece-skull.png -i easy > data/easy_tile.h
        python3 ./png2ugd.py data/grph/icons/piece-skull.png -i easy -a > data/easy_tile.asm
        # python3 ./png2ugd.py data/grph/icons/pirate-skull.png -i hard > data/hard_tile.h
        python3 ./png2ugd.py data/grph/icons/pirate-skull.png -i hard -a > data/hard_tile.asm
        # python3 ./png2ugd.py data/grph/icons/rocket.png -i torpedo_ic > data/torpedo.h
        python3 ./png2ugd.py data/grph/icons/rocket.png -i torpedo_ic -a > data/torpedo.asm
        # python3 ./png2ugd.py data/grph/icons/shield.png -i armor_ic > data/armor.h
        python3 ./png2ugd.py data/grph/icons/shield.png -i armor_ic -a > data/armor.asm
        # python3 ./png2ugd.py data/grph/icons/skip_node.png -i skip_node > data/skip_node_tile.h
        python3 ./png2ugd.py data/grph/icons/skip_node.png -i skip_node -a > data/skip_node_tile.asm
        # python3 ./png2ugd.py data/grph/icons/spaceship.png -i ship_ic > data/ship_tile.h
        python3 ./png2ugd.py data/grph/icons/spaceship.png -i ship_ic -a > data/ship_tile.asm
        # python3 ./png2ugd.py data/grph/icons/spanner.png -i repair_mod_ic > data/repair_mod.h
        python3 ./png2ugd.py data/grph/icons/spanner.png -i repair_mod_ic -a > data/repair_mod.asm
        # python3 ./png2ugd.py data/grph/icons/special.png -i special_ic > data/special.h
        python3 ./png2ugd.py data/grph/icons/special.png -i special_ic -a > data/special.asm
        # python3 ./png2ugd.py data/grph/icons/techno-heart.png -i heal_ic > data/heal.h
        python3 ./png2ugd.py data/grph/icons/techno-heart.png -i heal_ic -a > data/heal.asm
        # python3 ./png2ugd.py data/grph/icons/upgrade.png -i upgrade_ic > data/upgrade.h
        python3 ./png2ugd.py data/grph/icons/upgrade.png -i upgrade_ic -a > data/upgrade.asm
        # python3 ./png2ugd.py data/grph/icons/semi-closed-eye.png -i spy_ic > data/spy_tile.h
        python3 ./png2ugd.py data/grph/icons/semi-closed-eye.png -i spy_ic -a > data/spy_tile.asm
}
# zcc +zx -vn -clib=sdcc_iy -startup=31 glyph_drawing.c -o glph -create-app
# zcc +zx -vn -clib=sdcc_iy -startup=31 mvp.c -o mvp -create-app && fuse mvp.tap
# zcc +zx -vn -startup=31 -clib=new sp1d.c graphics.asm -o demo -create-app
# zcc +zx -vn -SO2 -startup=31 -clib=sdcc_iy --max-allocs-per-node200000 sp1d.c graphics.asm -o demo -create-app
# zcc +zx -vn -SO2 -startup=0 -clib=sdcc_iy --max-allocs-per-node200000 draw_wing.c data/ship2.asm  -o draw_wing -create-app && fuse draw_wing.tap
# zcc +zx -vn -SO2 -startup=0 -clib=sdcc_iy --max-allocs-per-node200000 inspect_ship.c -o inspect_ship -create-app && fuse inspect_ship.tap

generate_graphics
generate_tiles

case $1 in 
    main)
        zcc +zx -vn -SO3 -startup=31 -DWFRAMES=3 -clib=sdcc_iy -m --list --c-code-in-asm --opt-code-size --max-allocs-per-node200000 \
            mvp.c \
            data/b1.asm \
            data/b2.asm \
            data/i1.asm \
            data/i2.asm \
            data/s1.asm \
            data/s2.asm \
            data/d1.asm \
            data/d2.asm \
            data/boss_1.asm \
            data/boss_2.asm \
            data/bullet.asm \
            data/dynamic_data.asm \
            data/static_data.asm \
            data/rebirth.asm \
            data/attack.asm \
            data/boss_tile.asm \
            data/compact_arr_tile.asm \
            data/money_tile.asm \
            data/retreat.asm \
            data/hp_mod.asm \
            data/norm_tile.asm \
            data/easy_tile.asm \
            data/hard_tile.asm \
            data/torpedo.asm \
            data/armor.asm \
            data/skip_node_tile.asm \
            data/ship_tile.asm \
            data/repair_mod.asm \
            data/special.asm \
            data/heal.asm \
            data/upgrade.asm \
            data/spy_tile.asm \
            data/ramka_rb.asm \
            data/ramka_lt.asm \
            -Cz--screen=data/grph/load_screen.scr \
            -o mvp -create-app && fuse mvp.tap
    ;;
    fight)
        zcc +zx -vn -SO2 -startup=31 -clib=sdcc_iy --max-allocs-per-node200000 \
            fight_debug.c \
            data/attack_icon.asm \
            data/flee_icon.asm \
            data/special_icon.asm \
            data/ship2.asm \
            data/ramka_lb.asm \
            data/ramka_rb.asm \
            data/ramka_lt.asm \
            data/ramka_rt.asm \
            -Cz--screen=data/grph/load_screen.scr \
            -o mvp -create-app && fuse mvp.tap
    ;;
    wing)
        generate_graphics
        zcc +zx -vn -SO2 -startup=31 -DWFRAMES=3 -clib=sdcc_iy --max-allocs-per-node200000 \
        draw_wing_debug.c \
        data/shield_sp.asm \
        data/b1.asm \
        data/b2.asm \
        data/i1.asm \
        data/i2.asm \
        data/s1.asm \
        data/s2.asm \
        data/d1.asm \
        data/d2.asm \
        data/boss_1.asm \
        data/boss_2.asm \
        data/dynamic_data.asm \
        data/rebirth.asm \
        data/ramka_lb.asm \
        data/ramka_rb.asm \
        data/ramka_lt.asm \
        data/ramka_rt.asm \
        -o draw_wing -create-app && fuse draw_wing.tap
    ;;
    inspect)
        generate_tiles
        zcc +zx -vn -SO2 -startup=31 -DWFRAMES=3 -clib=sdcc_iy --max-allocs-per-node200000 \
            inspect_ship_debug.c \
            data/ship2.asm \
            data/ramka_lb.asm \
            data/ramka_rb.asm \
            data/ramka_lt.asm \
            data/ramka_rt.asm \
            -o inspect_ship -create-app && fuse inspect_ship.tap
    ;;
    cursor)
        zcc +zx -vn -SO2 -startup=31 -clib=sdcc_iy --max-allocs-per-node200000 \
            cursor_debug.c \
            data/attack_icon.asm \
            data/flee_icon.asm \
            data/special_icon.asm \
            data/ship2.asm \
            data/ramka_lb.asm \
            data/ramka_rb.asm \
            data/ramka_lt.asm \
            data/ramka_rt.asm \
            -o cursor -create-app && fuse cursor.tap
    ;;
    glyph)
        zcc +zx -vn -SO2 -startup=0 -clib=sdcc_iy --max-allocs-per-node200000 \
            glyph_drawing.c \
            -o glph -create-app && fuse glph.tap
    ;;
    world)
        generate_tiles
        python3 ./generate_world.py > ./data/world_0.h
        zcc +zx -vn -SO2 -startup=31 -clib=sdcc_iy --max-allocs-per-node200000 \
            world_debug.c \
            data/b1.asm \
            data/b2.asm \
            data/i1.asm \
            data/i2.asm \
            data/s1.asm \
            data/s2.asm \
            data/d1.asm \
            data/d2.asm \
            data/boss_1.asm \
            data/boss_2.asm \
            data/dynamic_data.asm \
            data/static_data.asm \
            data/rebirth.asm \
            data/compact_arr_tile.asm \
            data/ramka_lb.asm \
            data/ramka_rb.asm \
            data/ramka_lt.asm \
            data/ramka_rt.asm \
            -o world -create-app && fuse world.tap
    ;;    
    example)
        zcc +zx -vn -SO2 -startup=31 -clib=sdcc_iy --max-allocs-per-node200000 sp1d.c data/graphics.asm -o demo -create-app && fuse demo.tap
    ;;
    gallery)
        generate_tiles
        zcc +zx -vn -SO2 -DWFRAMES=3 -startup=31 -clib=sdcc_iy --max-allocs-per-node200000 \
            gallery.c \
            data/attack_icon.asm \
            data/flee_icon.asm \
            data/special_icon.asm \
            data/ship2.asm \
            data/ramka_lb.asm \
            data/ramka_rb.asm \
            data/ramka_lt.asm \
            data/ramka_rt.asm \
            -o gallery -create-app && fuse gallery.tap
    ;;
    shop)
        generate_tiles
        zcc +zx -vn -SO2 -DWFRAMES=3 -startup=31 -clib=sdcc_iy --max-allocs-per-node200000 \
            shop_debug.c \
            data/shield_sp.asm \
            data/b1.asm \
            data/b2.asm \
            data/i1.asm \
            data/i2.asm \
            data/s1.asm \
            data/s2.asm \
            data/d1.asm \
            data/d2.asm \
            data/boss_1.asm \
            data/boss_2.asm \
            data/dynamic_data.asm \
            data/static_data.asm \
            data/rebirth.asm \
            data/compact_arr_tile.asm \
            data/ramka_lb.asm \
            data/ramka_rb.asm \
            data/ramka_lt.asm \
            data/ramka_rt.asm \
            -o shop -create-app && fuse shop.tap
    ;;
    bullet)
        generate_graphics
            zcc +zx -vn -SO2 -startup=31 -DWFRAMES=3 -clib=sdcc_iy --max-allocs-per-node200000 \
            draw_wing_with_bullet.c \
            data/b1.asm \
            data/i1.asm \
            data/s1.asm \
            data/d1.asm \
            -o draw_wing -create-app && fuse draw_wing.tap
    ;;
    rand)
        zcc +zx -vn -SO2 -startup=31 -DWFRAMES=3 -clib=sdcc_iy --max-allocs-per-node200000 \
            debug_rand.c \
            data/b1.asm \
            data/b2.asm \
            data/i1.asm \
            data/i2.asm \
            data/s1.asm \
            data/s2.asm \
            data/d1.asm \
            data/d2.asm \
            data/boss_1.asm \
            data/boss_2.asm \
            data/dynamic_data.asm \
            data/rebirth.asm \
            data/ramka_lb.asm \
            data/ramka_rb.asm \
            data/ramka_lt.asm \
            data/ramka_rt.asm \
            -o rand -create-app && fuse rand.tap
    ;;
    lasor)
        zcc +zx -vn -SO2 -startup=31 -DWFRAMES=3 -clib=sdcc_iy --max-allocs-per-node200000 \
            lasor_debug.c \
            -o rand -create-app && fuse rand.tap
    ;;
esac    
