/*
 * blink.c
 *
 *  Created on: 2 thg 8, 2023
 *      Author: ADMIN
 */


#include "em_gpio.h"

void blink_init(void)
{
  GPIO_PinModeSet(0, 4, _GPIO_P_MODEL_MODE0_PUSHPULL, 0);
}

void blink_process_action(unsigned int status)
{
  if (status == 1)
    {
      GPIO_PinOutSet(0, 4);
    }
  else
    {
      GPIO_PinOutClear(0, 4);
    }
}
