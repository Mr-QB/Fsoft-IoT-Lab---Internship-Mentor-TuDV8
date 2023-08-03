#include <AHT20.h>
#include "sl_emlib_gpio_init_AHT20_config.h"
#include "sl_emlib_gpio_init_Led0_config.h"
#include "sl_emlib_gpio_init_Led1_config.h"

// Define
#define AHT20_ADDRESS 0x38
#define AHT20_ADDRESS_WRITE 0x70
#define AHT20_ADDRESS_READ 0x71
#define I2C_WRITE_SIZE 3
#define I2C_READ_SIZE_1 1
#define I2C_READ_SIZE_2 6
#define SL_I2CSPM_AHT20_PERIPHERAL               I2C1



uint32_t temperature_raw = 0;
extern float temperature_s = 0;
uint32_t humidity_raw = 0;
extern float humidity_s = 0;

// Read - write data
uint8_t initialization[I2C_WRITE_SIZE] = {0xBE, 0x08, 0x00};
uint8_t triggerMeasurement[I2C_WRITE_SIZE] = {0xAC, 0x33, 0x00};
volatile uint8_t readData1[I2C_READ_SIZE_1];
volatile uint8_t readData2[I2C_READ_SIZE_2];

sl_status_t AHT20_begin(void)
{
  sl_sleeptimer_delay_millisecond(100);

  I2C_TransferSeq_TypeDef i2cTransferBegin;
  I2C_TransferReturn_TypeDef result;

  uint8_t Initialization[3] = {0xBE, 0x08, 0x00};
  // Initialize I2C transfer
  i2cTransferBegin.addr = AHT20_ADDRESS_WRITE;
  i2cTransferBegin.flags = I2C_FLAG_WRITE;
  i2cTransferBegin.buf[0].data = Initialization;
  i2cTransferBegin.buf[0].len = 3;
  i2cTransferBegin.buf[1].data = NULL;
  i2cTransferBegin.buf[1].len = 0;

  result = I2C_TransferInit(SL_I2CSPM_AHT20_PERIPHERAL, &i2cTransferBegin);
//  if (result != i2cTransferDone)
//    {
//      return SL_STATUS_TRANSMIT;
//    }
//  return  SL_STATUS_OK;
  while (result == i2cTransferInProgress)
       {
      result = I2C_Transfer(SL_I2CSPM_AHT20_PERIPHERAL);
       }
     if(result != i2cTransferDone) return SL_STATUS_FAIL;
     else return 0;
}

sl_status_t AHT20_cmd(void)
{
  sl_sleeptimer_delay_millisecond(100);

  I2C_TransferSeq_TypeDef i2cTransferBegin;
  I2C_TransferReturn_TypeDef result;

  uint8_t Trigger[3] = {0xAC, 0x33, 0x00};
  // Initialize I2C transfer
  i2cTransferBegin.addr = AHT20_ADDRESS_WRITE;
  i2cTransferBegin.flags = I2C_FLAG_WRITE;
  i2cTransferBegin.buf[0].data = Trigger;
  i2cTransferBegin.buf[0].len = 3;
  i2cTransferBegin.buf[1].data = NULL;
  i2cTransferBegin.buf[1].len = 0;

//  result = I2CSPM_Transfer(SL_I2CSPM_AHT20_PERIPHERAL, &i2cTransferBegin);
//  if (result != i2cTransferDone)
//    {
//      return SL_STATUS_TRANSMIT;
//    }
//  return  SL_STATUS_OK;
  result = I2C_TransferInit(SL_I2CSPM_AHT20_PERIPHERAL, &i2cTransferBegin);
    while (result == i2cTransferInProgress)
         {
        result = I2C_Transfer(SL_I2CSPM_AHT20_PERIPHERAL);
         }
       if(result != i2cTransferDone) return SL_STATUS_FAIL;
       else return 0;


}

sl_status_t startSensor(void)    //write_cmd
{
//  uint8_t Trigger[3] = {0xAC, 0x33, 0x00};
    I2C_TransferSeq_TypeDef i2cTransfer;
    I2C_TransferReturn_TypeDef result;


  i2cTransfer.addr = AHT20_ADDRESS << 1;
  i2cTransfer.flags = I2C_FLAG_READ;
  i2cTransfer.buf[0].data = readData1;
  i2cTransfer.buf[0].len = 1;
  i2cTransfer.buf[1].data = NULL;
  i2cTransfer.buf[1].len = 0;
//  int counter = 0;
//  uint16_t counter  = 0;
//    while(1)
//      {
//        sl_sleeptimer_delay_millisecond(1);
//        if (counter ++ > 100) return 0;
//        while (result == i2cTransferInProgress)
//          {
//            result = I2CSPM_Transfer(SL_I2CSPM_AHT20_PERIPHERAL, &i2cTransfer);
//            if (readData1[0] && 0x80 != 0) return 1;
//          }
//        if (result != i2cTransferDone) {
//           // LED1 ON and infinite while loop to indicate I2C transmission problem
//           GPIO_PinOutSet(SL_EMLIB_GPIO_INIT_LED0_PORT, SL_EMLIB_GPIO_INIT_LED0_PIN);
//          // while(1);
//         }
//       }
//    sl_sleeptimer_delay_millisecond(20);
      result = I2C_TransferInit(SL_I2CSPM_AHT20_PERIPHERAL, &i2cTransfer);
      uint16_t counter  = 0;
      while(1)
        {
          sl_sleeptimer_delay_millisecond(1);
          if (counter ++ > 100) return 0;
          while (result == i2cTransferInProgress)
            {
              result =  I2C_Transfer(SL_I2CSPM_AHT20_PERIPHERAL);
              GPIO_PinOutSet(SL_EMLIB_GPIO_INIT_LED0_PORT, SL_EMLIB_GPIO_INIT_LED0_PIN);
              if (readData1[0] && 0x80 != 0) return 1;
            }
          if (result != i2cTransferDone) {
             // LED1 ON and infinite while loop to indicate I2C transmission problem
             //GPIO_PinOutSet(SL_EMLIB_GPIO_INIT_LED0_PORT, SL_EMLIB_GPIO_INIT_LED0_PIN);
            // while(1);
             return SL_STATUS_FAIL;
           }else return 0;
         }
      sl_sleeptimer_delay_millisecond(20);
}

sl_status_t  getSensor( uint32_t *temperature_raw, float *temperature_s,uint32_t *humidity_raw, float *humidity_s)          //Read_cmd
{
  startSensor();
  sl_sleeptimer_delay_millisecond(100);
  I2C_TransferSeq_TypeDef i2cTransfer;
  I2C_TransferReturn_TypeDef result;

  // Initialize I2C transfer

  //uint8_t read_command[1] = {0x71};
  i2cTransfer.addr = AHT20_ADDRESS<<1;
  i2cTransfer.flags = I2C_FLAG_READ;
  i2cTransfer.buf[0].data = readData2;
  i2cTransfer.buf[0].len = 6;
  i2cTransfer.buf[1].data = NULL;
  i2cTransfer.buf[1].len = 0;

//  result = I2CSPM_Transfer(SL_I2CSPM_AHT20_PERIPHERAL, &i2cTransfer);
//
//    if (readData2[0] & 0x80 ) return 0;



//    __humi = readData2[1];
//    __humi <<= 8;
//    __humi += readData2[2];
//    __humi <<= 4;
//    __humi += readData2[3] >> 4;
//
//    h = (float)__humi / 1048576.0;
//    __temp = readData2[3] & 0x0f;
//    __temp <<= 8;
//    __temp += readData2[4];
//    __temp <<= 8;
//    __temp += readData2[5];
//
//    t = (float)__temp / 1048576.0 * 200.0 - 50.0;


  result =  I2C_TransferInit(SL_I2CSPM_AHT20_PERIPHERAL, &i2cTransfer);
  while (result == i2cTransferInProgress) {
      result = I2C_Transfer(SL_I2CSPM_AHT20_PERIPHERAL);
     }
     if (result != i2cTransferDone) {
         return SL_STATUS_FAIL;
     }
     else{
            *humidity_raw = (((uint32_t)readData2[1]) << 16) |
                            (((uint32_t)readData2[2]) << 8) |
                            (((uint32_t)readData2[3]) << 0);
            *humidity_raw = (*humidity_raw) >> 4;
            *humidity_s = ((float)(*humidity_raw) / 1048576.0f * 100.0f); // convert the humidity

            *temperature_raw = (((uint32_t)readData2[3]) << 16) |
                               (((uint32_t)readData2[4]) << 8) |
                               (((uint32_t)readData2[5]) << 0);
            *temperature_raw = (*temperature_raw) & 0xFFFFF; // cut the temperature part
            *temperature_s = (float)(*temperature_raw)/ 1048576.0f * 200.0f - 50.0f;


    return SL_STATUS_OK;
     }
}


void printf_temp_humi_data(){
  sl_status_t status1 = getSensor(&temperature_raw,&temperature_s,&humidity_raw,&humidity_s) ;

  if(status1 != SL_STATUS_OK  ){
  printf("Failed to read data from AHT20.\n");

  }
  else{

      getSensor(&temperature_raw,&temperature_s,&humidity_raw,&humidity_s) ;

//      printf("Humidity = %u, Temperature = %lu\n", humidity_s, (temperature_raw/1048576 * 200
//             - 50));
      printf("Humidity = %.2f, Temperature = .%2f\n", humidity_s, (temperature_s));
  }
  }


