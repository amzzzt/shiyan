/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************
 *
 *                       MSP430 CODE EXAMPLE DISCLAIMER
 *
 * MSP430 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
 * for an API functional library-approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/
//******************************************************************************
//   MSP430F552x Demo - LED1 / LED2 轮流亮灭
//
//   Description: LED1(P1.0) 与 LED2(P4.7) 始终保持相反状态，每隔 1s 同时翻转。
//   ACLK = 32.768kHz, MCLK = SMCLK = default DCO~1MHz
//
//                MSP430F552x
//             -----------------
//         /|\|                 |
//          | |                 |
//          --|RST              |
//            |             P1.0|-->LED1
//            |             P4.7|-->LED2
//
//   Built with CCS Theia
//******************************************************************************
#include <msp430.h>
#include "key.h"

// ---- 延时函数（原来放在 delay.h，现在搬到这里，让本文件自成一体）----
#define CPU_F 1000000UL                   // MCLK 主频 1MHz
#define delay_ms(x) __delay_cycles(CPU_F / 1000UL * (x))
#define delay_us(x) __delay_cycles(CPU_F / 1000000UL * (x))

// LED 亮灭控制函数
//   on = 1 → LED1(P1.0) 亮、LED2(P4.7) 灭
//   on = 0 → LED1 灭、LED2 亮
void light(int on)
{
  if (on)
  {
    P1OUT &= ~BIT0;                   // LED1 亮（低电平点亮）
    P4OUT |=  BIT7;                   // LED2 灭
  }
  else
  {
    P1OUT |=  BIT0;                   // LED1 灭
    P4OUT &= ~BIT7;                   // LED2 亮
  }
}

int main(void)
{
  int t;                              // 节拍计数
  int i;                              // 流水循环用

  WDTCTL = WDTPW+WDTHOLD;             // 关闭看门狗

  P1DIR |= BIT0;                      // P1.0 输出（LED1）
  P4DIR |= BIT7;                      // P4.7 输出（LED2）
  P3DIR |= 0xFF;                      // P3.0 ~ P3.7 全部输出（8 个流水灯）
  P3OUT |= 0xFF;                      // 低电平点亮，先全部熄灭

  key3_init();                        // 三个独立按键初始化（P4.0~P4.2）

  while (1)
  {
    /* ---- 两个 LED 每 1 秒交替一次（不按键时也闪）---- */
    if ((t / 2) % 2 == 0)
    {
      light(0);                      // 断点1：LED1 灭、LED2 亮
    }
    else
    {
      light(1);                      // 断点2：LED1 亮、LED2 灭
    }

    switch (key3_scan())
    {
      case S1:                                      // 从右向左流水
        for (i = 7; i >= 0; i--)
        {
          P3OUT |= 0xFF;                            // 先全部灭掉
          P3OUT &= ~(1 << i);                       // 只点亮这一个
          light((i / 2) % 2);                       // 流水期间两个 LED 继续交替
          delay_ms(500);
        }
        P3OUT |= 0xFF;                              // 流完把最后一个也灭掉
        break;

      case S2:                                      // 从左向右流水
        for (i = 0; i < 8; i++)
        {
          P3OUT |= 0xFF;                            // 先全部灭掉
          P3OUT &= ~(1 << i);                       // 只点亮这一个
          light((i / 2) % 2);
          delay_ms(500);
        }
        P3OUT |= 0xFF;                              // 流完把最后一个也灭掉
        break;

      case S3:                                      // 左→右依次亮起，再右→左依次熄灭
        for (i = 0; i < 8; i++)
        {
          P3OUT &= ~(1 << i);                       // 累积点亮：P3.0 → P3.7
          light((i / 2) % 2);
          delay_ms(500);
        }
        for (i = 7; i >= 0; i--)
        {
          P3OUT |= (1 << i);                        // 依次熄灭：P3.7 → P3.0
          light((i / 2) % 2);
          delay_ms(500);
        }
        break;
    }

    delay_ms(500);                    // 公共节拍 500ms
    t++;
  }
}
