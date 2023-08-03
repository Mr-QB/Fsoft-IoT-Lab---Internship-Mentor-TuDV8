#include "sl_emlib_gpio_simple_init.h"
#include "sl_emlib_gpio_init_AHT20_config.h"
#include "sl_emlib_gpio_init_Led0_config.h"
#include "sl_emlib_gpio_init_Led1_config.h"
#include "em_gpio.h"
#include "em_cmu.h"

void sl_emlib_gpio_simple_init(void)
{
  CMU_ClockEnable(cmuClock_GPIO, true);
  GPIO_PinModeSet(SL_EMLIB_GPIO_INIT_AHT20_PORT,
                  SL_EMLIB_GPIO_INIT_AHT20_PIN,
                  SL_EMLIB_GPIO_INIT_AHT20_MODE,
                  SL_EMLIB_GPIO_INIT_AHT20_DOUT);

  GPIO_PinModeSet(SL_EMLIB_GPIO_INIT_LED0_PORT,
                  SL_EMLIB_GPIO_INIT_LED0_PIN,
                  SL_EMLIB_GPIO_INIT_LED0_MODE,
                  SL_EMLIB_GPIO_INIT_LED0_DOUT);

  GPIO_PinModeSet(SL_EMLIB_GPIO_INIT_LED1_PORT,
                  SL_EMLIB_GPIO_INIT_LED1_PIN,
                  SL_EMLIB_GPIO_INIT_LED1_MODE,
                  SL_EMLIB_GPIO_INIT_LED1_DOUT);
}
