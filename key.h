#ifndef KEY_H
#define KEY_H
#include <msp430.h>

extern volatile char blink1;
extern volatile char blink2;

void key_init(void);

// ---- 三个独立按键：S1 = P4.0，S2 = P4.1，S3 = P4.2 ----
#define KEY_NONE  0
#define S1        1
#define S2        2
#define S3        3

void key3_init(void);
unsigned char key3_scan(void);

#endif
