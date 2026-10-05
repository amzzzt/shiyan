#include <msp430.h>
#include "key.h"
#include "delay.h"

volatile char blink1 = 0;
volatile char blink2 = 0;


// P2.1 和 P1.1 各接一个按键，按键另一端接地，所以松开=1、按下=0
void key_init(void)
{
  P2DIR &= ~BIT1;                              // P2.1 设为输入
  P2REN |=  BIT1;                              // 使能内部上下拉电阻
  P2OUT |=  BIT1;                              // 选上拉

  P1DIR &= ~BIT1;                              // P1.1 设为输入
  P1REN |=  BIT1;
  P1OUT |=  BIT1;

  P1IES |=  BIT1;    //下降沿
  P1IFG &= ~BIT1;    //清中断标志
  P1IE  |=  BIT1;    //使能

  P2IE  |=  BIT1;
  P2IES &= ~BIT1;
  P2IFG |=  BIT1;

  __enable_interrupt();
}


#pragma vector = PORT2_VECTOR
__interrupt void Port_2(void)
{
    if(P2IFG & BIT1)
    {
        delay_ms(20);
        if((P2IN & BIT1) == 0)
        {
            blink1 = 1;
        }
        P2IFG &= ~BIT1;
    }
}

#pragma vector = PORT1_VECTOR
__interrupt void Port_1(void)
{
    if(P1IFG & BIT1)
    {
        delay_ms(20);
        if((P1IN & BIT1) == 0)
        {
            blink2 = 1;
        }
        P1IFG &= ~BIT1;
    }
}
