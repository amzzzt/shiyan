#include <msp430.h>
#include "delay.h"

// MCLK 约 1MHz，1ms = 1000 个 CPU 周期。改了时钟这个数要跟着改
#define CYCLES_PER_MS 1000

void delay_ms(unsigned int ms)
{
  while(ms--) {
    __delay_cycles(CYCLES_PER_MS);
  }
}
