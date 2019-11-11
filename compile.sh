#!/bin/bash

source $HOME/.bashrc

function generate_graphics {
    python3 ./png2sp1sprite.py data/grph/ship2.png -i ship2 > data/ship2.asm
    python3 ./png2sp1sprite.py data/grph/attack.png -i attack_ic > data/attack_icon.asm
    python3 ./png2sp1sprite.py data/grph/flee.png -i flee_ic > data/flee_icon.asm
    python3 ./png2sp1sprite.py data/grph/special.png -i special_ic > data/special_icon.asm
    
    python3 ./png2ugd.py data/grph/boss.png -a -i boss_ic > data/boss_tile.asm
    # python3 ./png2ugd.py data/grph/credit.png -a -i money_ic > data/money_tile.asm
    
    python3 ./png2sp1sprite.py data/grph/ramka_lb.png -i ramka_lb > data/ramka_lb.asm
    python3 ./png2sp1sprite.py data/grph/ramka_rb.png -i ramka_rb > data/ramka_rb.asm
    python3 ./png2sp1sprite.py data/grph/ramka_lt.png -i ramka_lt > data/ramka_lt.asm
    python3 ./png2sp1sprite.py data/grph/ramka_rt.png -i ramka_rt > data/ramka_rt.asm
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
        zcc +zx -vn -SO2 -startup=31 -clib=sdcc_iy --max-allocs-per-node200000 \
            mvp.c \
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
        zcc +zx -vn -SO2 -startup=0 -clib=sdcc_iy --max-allocs-per-node200000 inspect_ship_debug.c -o inspect_ship -create-app && fuse inspect_ship.tap
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
        python3 ./png2ugd.py data/grph/credit.png -i money_ic > data/money_tile.h
        python3 ./png2ugd.py data/grph/compact_arr.png -i compact_arr > data/compact_arr_tile.h
        python3 ./png2ugd.py data/grph/skip_node.png -i skip_node > data/skip_node_tile.h
        python3 ./png2ugd.py data/grph/boss.png -i boss > data/boss_tile.h
        python3 ./png2ugd.py data/grph/morbid-humour.png -i norm > data/norm_tile.h
        python3 ./png2ugd.py data/grph/piece-skull.png -i easy > data/easy_tile.h
        python3 ./png2ugd.py data/grph/pirate-skull.png -i hard > data/hard_tile.h
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
esac
