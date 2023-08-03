#ifndef PIN_CONFIG_H
#define PIN_CONFIG_H

// $[CMU]
// [CMU]$

// $[PRS.ASYNCH0]
// [PRS.ASYNCH0]$

// $[PRS.ASYNCH1]
// [PRS.ASYNCH1]$

// $[PRS.ASYNCH2]
// [PRS.ASYNCH2]$

// $[PRS.ASYNCH3]
// [PRS.ASYNCH3]$

// $[PRS.ASYNCH4]
// [PRS.ASYNCH4]$

// $[PRS.ASYNCH5]
// [PRS.ASYNCH5]$

// $[PRS.ASYNCH6]
// [PRS.ASYNCH6]$

// $[PRS.ASYNCH7]
// [PRS.ASYNCH7]$

// $[PRS.ASYNCH8]
// [PRS.ASYNCH8]$

// $[PRS.ASYNCH9]
// [PRS.ASYNCH9]$

// $[PRS.ASYNCH10]
// [PRS.ASYNCH10]$

// $[PRS.ASYNCH11]
// [PRS.ASYNCH11]$

// $[PRS.SYNCH0]
// [PRS.SYNCH0]$

// $[PRS.SYNCH1]
// [PRS.SYNCH1]$

// $[PRS.SYNCH2]
// [PRS.SYNCH2]$

// $[PRS.SYNCH3]
// [PRS.SYNCH3]$

// $[GPIO]
// GPIO SWV on PA03
#ifndef GPIO_SWV_PORT                           
#define GPIO_SWV_PORT                            gpioPortA
#endif
#ifndef GPIO_SWV_PIN                            
#define GPIO_SWV_PIN                             3
#endif

// [GPIO]$

// $[TIMER0]
// [TIMER0]$

// $[TIMER1]
// [TIMER1]$

// $[TIMER2]
// [TIMER2]$

// $[TIMER3]
// [TIMER3]$

// $[TIMER4]
// [TIMER4]$

// $[USART0]
// [USART0]$

// $[USART1]
// [USART1]$

// $[I2C1]
// I2C1 SCL on PD02
#ifndef I2C1_SCL_PORT                           
#define I2C1_SCL_PORT                            gpioPortD
#endif
#ifndef I2C1_SCL_PIN                            
#define I2C1_SCL_PIN                             2
#endif

// I2C1 SDA on PD03
#ifndef I2C1_SDA_PORT                           
#define I2C1_SDA_PORT                            gpioPortD
#endif
#ifndef I2C1_SDA_PIN                            
#define I2C1_SDA_PIN                             3
#endif

// [I2C1]$

// $[PDM]
// [PDM]$

// $[LETIMER0]
// [LETIMER0]$

// $[IADC0]
// [IADC0]$

// $[I2C0]
// [I2C0]$

// $[EUART0]
// [EUART0]$

// $[PTI]
// [PTI]$

// $[MODEM]
// [MODEM]$

// $[CUSTOM_PIN_NAME]
#ifndef BSP_GPIO_LED0_PORT_PORT                 
#define BSP_GPIO_LED0_PORT_PORT                  gpioPortA
#endif
#ifndef BSP_GPIO_LED0_PORT_PIN                  
#define BSP_GPIO_LED0_PORT_PIN                   4
#endif

#ifndef BSP_GPIO_PB0_PORT_PORT                  
#define BSP_GPIO_PB0_PORT_PORT                   gpioPortB
#endif
#ifndef BSP_GPIO_PB0_PORT_PIN                   
#define BSP_GPIO_PB0_PORT_PIN                    0
#endif

#ifndef BSP_GPIO_LED1_PORT_PORT                 
#define BSP_GPIO_LED1_PORT_PORT                  gpioPortB
#endif
#ifndef BSP_GPIO_LED1_PORT_PIN                  
#define BSP_GPIO_LED1_PORT_PIN                   2
#endif

#ifndef SL_I2CSPM_AHT20_SCL_PORT_PORT           
#define SL_I2CSPM_AHT20_SCL_PORT_PORT            gpioPortD
#endif
#ifndef SL_I2CSPM_AHT20_SCL_PORT_PIN            
#define SL_I2CSPM_AHT20_SCL_PORT_PIN             2
#endif

#ifndef SL_I2CSPM_AHT20_SDA_PORT_PORT           
#define SL_I2CSPM_AHT20_SDA_PORT_PORT            gpioPortD
#endif
#ifndef SL_I2CSPM_AHT20_SDA_PORT_PIN            
#define SL_I2CSPM_AHT20_SDA_PORT_PIN             3
#endif

// [CUSTOM_PIN_NAME]$

#endif // PIN_CONFIG_H

