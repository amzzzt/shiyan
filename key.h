#ifndef KEY_H
#define KEY_H
#include <msp430.h>

#define KEY_NONE  0
#define KEY1      1                            // P2.1
#define KEY2      2                            // P1.1

void key_init(void);
unsigned char key_scan(void);                  // 返回 KEY_NONE / KEY1 / KEY2

#endif
