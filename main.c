#include <msp430.h>
#include "app_config.h"

#if   RUN_EX == 1
  #include "exercises/ex01_led.h"
#else
  #error "RUN_EX 没有对应的题目，请在 app_config.h 里设置"
#endif

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;          /* 关看门狗，全工程只在这里做一次 */

#if   RUN_EX == 1
    ex01_led_run();
#endif

    return 0;
}
