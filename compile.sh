source $HOME/.bashrc
# zcc +zx -vn -clib=sdcc_iy -startup=31 glyph_drawing.c -o glph -create-app
# zcc +zx -vn -clib=sdcc_iy -startup=31 mvp.c -o mvp -create-app && fuse mvp.tap
# zcc +zx -vn -startup=31 -clib=new sp1d.c graphics.asm -o demo -create-app
# zcc +zx -vn -SO2 -startup=31 -clib=sdcc_iy --max-allocs-per-node200000 sp1d.c graphics.asm -o demo -create-app
# zcc +zx -vn -SO2 -startup=0 -clib=sdcc_iy --max-allocs-per-node200000 draw_wing.c data/ship2.asm  -o draw_wing -create-app && fuse draw_wing.tap
zcc +zx -vn -SO2 -startup=0 -clib=sdcc_iy --max-allocs-per-node200000 inspect_ship.c -o inspect_ship -create-app && fuse inspect_ship.tap