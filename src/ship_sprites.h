#ifndef __SUP_SP_SHIP_SPR__
#define __SUP_SP_SHIP_SPR__

extern unsigned char interceptor_11[];
extern unsigned char interceptor_12[];
extern unsigned char interceptor_13[];

extern unsigned char interceptor_21[];
extern unsigned char interceptor_22[];
extern unsigned char interceptor_23[];

extern unsigned char bomber_11[];
extern unsigned char bomber_12[];
extern unsigned char bomber_13[];

extern unsigned char bomber_21[];
extern unsigned char bomber_22[];
extern unsigned char bomber_23[];

extern unsigned char support_11[];
extern unsigned char support_12[];
extern unsigned char support_13[];

extern unsigned char support_21[];
extern unsigned char support_22[];
extern unsigned char support_23[];

extern unsigned char destroyer_11[];
extern unsigned char destroyer_12[];
extern unsigned char destroyer_13[];

extern unsigned char destroyer_21[];
extern unsigned char destroyer_22[];
extern unsigned char destroyer_23[];



extern unsigned char boss_11[];
extern unsigned char boss_12[];
extern unsigned char boss_13[];

extern unsigned char boss_21[];
extern unsigned char boss_22[];
extern unsigned char boss_23[];

// extern unsigned char shield_sp1[];
// extern unsigned char shield_sp2[];
// extern unsigned char shield_sp3[];

// extern unsigned char flip_buffer[5][96];
extern unsigned char flip_buffer[96];

#define column2_offset ((int)interceptor_12 - (int)interceptor_11) 
#define column3_offset ((int)interceptor_13 - (int)interceptor_11) 

#endif