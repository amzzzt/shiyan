#include <msp430.h>
#include "key.h"
#include "delay.h"

// P2.1 和 P1.1 各接一个按键，按键另一端接地，所以松开=1、按下=0
void key_init(void)
{
  P2DIR &= ~BIT1;                              // P2.1 设为输入
  P2REN |=  BIT1;                              // 使能内部上下拉电阻
  P2OUT |=  BIT1;                              // 选上拉

  P1DIR &= ~BIT1;                              // P1.1 设为输入
  P1REN |=  BIT1;
  P1OUT |=  BIT1;
}

unsigned char key_scan(void)
{
  // 括号不能省！& 的优先级比 == 低
  if ((P2IN & BIT1) == 0)                      // P2.1 按下
  {
    delay_ms(20);                              // 等抖动过去
    if ((P2IN & BIT1) == 0)                    // 再确认一次
    {
      return KEY1;
    }
  }
  else if ((P1IN & BIT1) == 0)                 // P1.1 按下
  {
    delay_ms(20);
    if ((P1IN & BIT1) == 0)
    {
      return KEY2;
    }
  }

  return KEY_NONE;                             // 没按键
}
