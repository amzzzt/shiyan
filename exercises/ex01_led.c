#include "app_config.h"

#if RUN_EX == 1

#include <msp430.h>
#include "ex01_led.h"

#define LED1_OUT   P1OUT
#define LED1_DIR   P1DIR
#define LED1_BIT   BIT0

/* 空循环延时，约 1ms @ MCLK=1MHz；换精确延时得改用 Timer_A */
static void delay_ms(unsigned int ms)
{
    volatile unsigned int i;

    while (ms--) {
        for (i = 500; i > 0; i--);
    }
}

void ex01_led_run(void)
{
    LED1_DIR |= LED1_BIT;              /* P1.0 设为输出 */

    while (1) {
        LED1_OUT ^= LED1_BIT;          /* 翻转 P1.0 */
        delay_ms(500);
    }
}

#endif /* RUN_EX == 1 */
