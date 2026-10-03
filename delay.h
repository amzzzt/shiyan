#ifndef DELAY_H
#define DELAY_H

#include <msp430.h>

// CPU 主频 1MHz。改了时钟这里要跟着改
#define CPU_F 1000000UL

// 毫秒：CPU_F * x / 1000
#define delay_ms(x) __delay_cycles(CPU_F / 1000UL * (x))

// 微秒：CPU_F * x / 1000000
#define delay_us(x) __delay_cycles(CPU_F / 1000000UL * (x))

#endif
