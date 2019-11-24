#!/bin/bash

source $HOME/.bashrc

function generate_graphics {
    python3 ./png2sp1sprite.py data/grph/ship2.png -i ship2 > data/ship2.asm
    # python3 ./png2sp1sprite.py data/grph/attack.png -i attack_ic > data/attack_icon.asm
    # python3 ./png2sp1sprite.py data/grph/flee.png -i flee_ic > data/flee_icon.asm
    # python3 ./png2sp1sprite.py data/grph/special.png -i special_ic > data/special_icon.asm
    
    # python3 ./png2ugd.py data/grph/boss.png -a -i boss_ic > data/boss_tile.asm
    # python3 ./png2ugd.py data/grph/credit.png -a -i money_ic > data/money_tile.asm
    
    python3 ./png2sp1sprite.py data/grph/ramka_lb.png -i ramka_lb > data/ramka_lb.asm
    python3 ./png2sp1sprite.py data/grph/ramka_rb.png -i ramka_rb > data/ramka_rb.asm
    python3 ./png2sp1sprite.py data/grph/ramka_lt.png -i ramka_lt > data/ramka_lt.asm
    python3 ./png2sp1sprite.py data/grph/ramka_rt.png -i ramka_rt > data/ramka_rt.asm
}

function generate_tiles {
        python3 ./png2ugd.py data/grph/icons/angel-wings.png -i rebirth_ic > data/rebirth.h
        python3 ./png2ugd.py data/grph/icons/attack.png -i attack_ic > data/attack.h
        python3 ./png2ugd.py data/grph/icons/boss.png -i boss > data/boss_tile.h
        python3 ./png2ugd.py data/grph/icons/compact_arr.png -i compact_arr > data/compact_arr_tile.h
        python3 ./png2ugd.py data/grph/icons/credit.png -i money_ic > data/money_tile.h
        python3 ./png2ugd.py data/grph/icons/cycle.png -i retreat_ic > data/retreat.h
        python3 ./png2ugd.py data/grph/icons/forward-field.png -i hp_mod_ic > data/hp_mod.h
        python3 ./png2ugd.py data/grph/icons/morbid-humour.png -i norm > data/norm_tile.h
        python3 ./png2ugd.py data/grph/icons/piece-skull.png -i easy > data/easy_tile.h
        python3 ./png2ugd.py data/grph/icons/pirate-skull.png -i hard > data/hard_tile.h
        python3 ./png2ugd.py data/grph/icons/rocket.png -i torpedo_ic > data/torpedo.h
        python3 ./png2ugd.py data/grph/icons/shield.png -i armor_ic > data/armor.h
        python3 ./png2ugd.py data/grph/icons/skip_node.png -i skip_node > data/skip_node_tile.h
        python3 ./png2ugd.py data/grph/icons/spaceship.png -i ship_ic > data/ship_tile.h
        python3 ./png2ugd.py data/grph/icons/spanner.png -i repair_mod_ic > data/repair_mod.h
        python3 ./png2ugd.py data/grph/icons/special.png -i special_ic > data/special.h
        python3 ./png2ugd.py data/grph/icons/techno-heart.png -i heal_ic > data/heal.h
        python3 ./png2ugd.py data/grph/icons/upgrade.png -i upgrade_ic > data/upgrade.h
        python3 ./png2ugd.py data/grph/icons/semi-closed-eye.png -i spy_ic > data/spy_tile.h
}

# zcc +zx -vn -clib=sdcc_iy -startup=31 glyph_drawing.c -o glph -create-app
# zcc +zx -vn -clib=sdcc_iy -startup=31 mvp.c -o mvp -create-app && fuse mvp.tap
# zcc +zx -vn -startup=31 -clib=new sp1d.c graphics.asm -o demo -create-app
# zcc +zx -vn -SO2 -startup=31 -clib=sdcc_iy --max-allocs-per-node200000 sp1d.c graphics.asm -o demo -create-app
# zcc +zx -vn -SO2 -startup=0 -clib=sdcc_iy --max-allocs-per-node200000 draw_wing.c data/ship2.asm  -o draw_wing -create-app && fuse draw_wing.tap
# zcc +zx -vn -SO2 -startup=0 -clib=sdcc_iy --max-allocs-per-node200000 inspect_ship.c -o inspect_ship -create-app && fuse inspect_ship.tap

# generate_graphics

case $1 in 
    main)
        zcc +zx -vn -SO2 -startup=31 -DWFRAMES=3 -clib=sdcc_iy --max-allocs-per-node200000 \
            mvp.c \
            data/ship2.asm \
            data/ramka_lb.asm \
            data/ramka_rb.asm \
            data/ramka_lt.asm \
            data/ramka_rt.asm \
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
        zcc +zx -vn -SO2 -startup=0 -clib=sdcc_iy --max-allocs-per-node200000 draw_wing_debug.c data/ship2.asm  -o draw_wing -create-app && fuse draw_wing.tap
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
        zcc +zx -vn -SO2 -startup=31 -clib=sdcc_iy --max-allocs-per-node200000 \
            world_debug.c \
            data/attack_icon.asm \
            data/flee_icon.asm \
            data/special_icon.asm \
            data/ship2.asm \
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
            data/ship2.asm \
            data/ramka_lb.asm \
            data/ramka_rb.asm \
            data/ramka_lt.asm \
            data/ramka_rt.asm \
            -o shop -create-app && fuse shop.tap
    ;;
esac
