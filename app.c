/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
#include <app.h>




//#define I2C_ADDRESS                     0x09 << 1
//#define I2C_BUFFER_SIZE                 10
//// Transmission flags
//// Buffers
//uint8_t i2c_Buffer[I2C_BUFFER_SIZE];
//uint8_t i2c_BufferIndex;
//uint8_t i2c_command = 0;
//volatile bool i2c_gotTargetAddress;
//double initialValue = 8282201;

//void setupI2C(void) {
//
//
//
//  // Use default settings
//  I2C_Init_TypeDef i2cInit = I2C_INIT_DEFAULT;
//
//  // Configure to be addressable as follower
//  i2cInit.master = false;
//
//  // Enable GPIO pins SDA and SCL
//  GPIO_PinModeSet(gpioPortD, 2, gpioModeWiredAndPullUpFilter, 1);
//  GPIO_PinModeSet(gpioPortD, 3, gpioModeWiredAndPullUpFilter, 1);
//
//  // Route I2C pins to GPIO
//  GPIO->I2CROUTE[0].SDAROUTE = (GPIO->I2CROUTE[0].SDAROUTE & ~_GPIO_I2C_SDAROUTE_MASK)
//                        | (gpioPortD << _GPIO_I2C_SDAROUTE_PORT_SHIFT
//                        | (3 << _GPIO_I2C_SDAROUTE_PIN_SHIFT));
//  GPIO->I2CROUTE[0].SCLROUTE = (GPIO->I2CROUTE[0].SCLROUTE & ~_GPIO_I2C_SCLROUTE_MASK)
//                        | (gpioPortD << _GPIO_I2C_SCLROUTE_PORT_SHIFT
//                        | (2 << _GPIO_I2C_SCLROUTE_PIN_SHIFT));
//  GPIO->I2CROUTE[0].ROUTEEN = GPIO_I2C_ROUTEEN_SDAPEN | GPIO_I2C_ROUTEEN_SCLPEN;
//
//  // Initialize the I2C
 // I2C_Init(I2C0, &i2cInit);
//
//  // Set up to enable follower mode
//  I2C_SlaveAddressSet(I2C0, I2C_ADDRESS);
//  I2C_SlaveAddressMaskSet(I2C0, 0xFE); // must match exact address
//
//  // Configure interrupts
//  I2C_IntClear(I2C0, _I2C_IF_MASK);
//  I2C_IntEnable(I2C0, I2C_IEN_ADDR | I2C_IEN_RXDATAV | I2C_IEN_ACK | I2C_IEN_SSTOP | I2C_IEN_BUSERR | I2C_IEN_ARBLOST);
//  NVIC_EnableIRQ(I2C0_IRQn);
//}




//void I2C1_IRQHandler(void)
//{
//  uint32_t pending;
//  uint32_t rxData;
//
//  pending = I2C1->IF;
//
//  // If some sort of fault, abort transfer.
//  if (pending & (I2C_IF_BUSERR | I2C_IF_ARBLOST)) {
//  } else {
//    if (pending & I2C_IF_ADDR) {
//        // Address Match, indicating that reception is started
//        rxData = I2C1->RXDATA;
////        printf("rxData1 = %ld\n", rxData);
//        I2C1->CMD = I2C_CMD_ACK;
//
//        if (rxData & 0x1) { // read bit set
//          if (i2c_BufferIndex < I2C_BUFFER_SIZE) {
//            // Transfer data
//            I2C1->TXDATA     = i2c_Buffer[i2c_BufferIndex++];
//          } else {
//            // Invalid buffer index; transfer data as if follower non-responsive
//            I2C1->TXDATA     = 0xFF;
//          }
//        } else {
//          i2c_gotTargetAddress = false;
//        }
//
//        I2C_IntClear(I2C1, I2C_IF_ADDR | I2C_IF_RXDATAV);
//    }
//    else if (pending & I2C_IF_RXDATAV) {
//      rxData = I2C1->RXDATA;
////      printf("rxData2 = %ld\n", rxData);
//
//      if (!i2c_gotTargetAddress) {
//        /******************************************************/
//        /* Read target address from leader                    */
//        /******************************************************/
//        // Verify that target address is valid
//        if (rxData < I2C_BUFFER_SIZE) {
//          // Store target address
//          i2c_BufferIndex = rxData;
//          I2C1->CMD = I2C_CMD_ACK;
//          i2c_gotTargetAddress = true;
//        } else {
//          I2C1->CMD = I2C_CMD_NACK;
//        }
//      } else {
//        /******************************************************/
//        /* Read new data and write to target address          */
//        /******************************************************/
//        // Verify that target address is valid
//        if (i2c_BufferIndex < I2C_BUFFER_SIZE) {
//          // Write new data to target address; auto increment target address
//          i2c_Buffer[i2c_BufferIndex++] = rxData;
//          //printf("data written = %ld\n", rxData);  // only test. uncomment when release
//          i2c_command = rxData;
//          I2C1->CMD = I2C_CMD_ACK;
//        } else {
//          I2C1->CMD = I2C_CMD_NACK;
//        }
//      }
//
//      I2C_IntClear(I2C1, I2C_IF_RXDATAV);
//    }
//
//    if (pending & I2C_IF_ACK) {
//      /******************************************************/
//      /* Leader ACK'ed, so requesting more data             */
//      /******************************************************/
//      if (i2c_BufferIndex < I2C_BUFFER_SIZE) {
//        // Trnsfer data
//        I2C1->TXDATA = i2c_Buffer[i2c_BufferIndex++];
//      } else {
//        // Invalid buffer index; transfer data as if follower non-responsive
//        I2C1->TXDATA = 0xFF;
//      }
//
//      I2C_IntClear(I2C1, I2C_IF_ACK);
//    }
//
//    if (pending & I2C_IF_SSTOP) {
//      // End of transaction
//      I2C_IntClear(I2C1, I2C_IF_SSTOP);
////      printf("End of transaction\n");
//    }
//  }
//}



void app_init(void)
{

  sl_i2cspm_init_instances();
  //i2c_Buffer[1] = 0;// do am
  //i2c_Buffer[2] = 0;// nhiet do
 // i2c_Buffer[3] = 0;// status
  AHT20_begin();
  AHT20_cmd();
  printf("Start get initial value\n");
     //initialValue = getInitialValue();
    // printf("Initial Value = %d\n", (int) initialValue);
     printf("Done!\n");
     printf("Start!\n");


}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{


//  sl_status_t status = AHT20_begin();
//      if (status != SL_STATUS_OK) {
//        // Handle initialization error
//        // Add your error handling code here
//          GPIO_PinOutToggle(SL_EMLIB_GPIO_INIT_LED1_PORT, SL_EMLIB_GPIO_INIT_LED1_PIN);
//
//        while (1);
//      }else{
//          GPIO_PinOutSet(SL_EMLIB_GPIO_INIT_LED0_PORT, SL_EMLIB_GPIO_INIT_LED0_PIN);
//      }
//      if(getSensor() == 1){
//
//          GPIO_PinOutSet(SL_EMLIB_GPIO_INIT_LED0_PORT, SL_EMLIB_GPIO_INIT_LED0_PIN);
//  // Read temperature and humidity from AHT20 sensor
//       temp = getTemperature();
//       humi = getHumidity();
//       i2c_Buffer[1] = (int)humi;// do am
//       i2c_Buffer[2] = (int)temp;// nhiet do
//       app_log("Humidity = %d, Temperature = %d\n", (int) humi, (int) temp);
//
//            // Add a delay if needed before reading again
//       sl_sleeptimer_delay_millisecond(1000);
//      }
//      else{
//          GPIO_PinOutToggle(SL_EMLIB_GPIO_INIT_LED1_PORT, SL_EMLIB_GPIO_INIT_LED1_PIN);
//            }


    printf_temp_humi_data();
}
