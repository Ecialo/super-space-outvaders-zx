#ifndef _ICON_H
#define _ICON_H

extern unsigned char attack_ic1[];
extern unsigned char attack_ic2[];
extern unsigned char flee_ic1[];
extern unsigned char flee_ic2[];
extern unsigned char special_ic1[];
extern unsigned char special_ic2[];
extern unsigned char ramka1[];
extern unsigned char ramka2[];

// int icon_offset;
#define icon_offset ((int)attack_ic2 - (int)attack_ic1)
#endif
