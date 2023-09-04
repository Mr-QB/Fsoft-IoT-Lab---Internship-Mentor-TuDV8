#include <AHT20.h>
#include "em_i2c.h"

// Define
#define AHT20_ADDRESS 0x38
#define AHT20_ADDRESS_WRITE 0x70
#define AHT20_ADDRESS_READ 0x71
#define I2C_WRITE_SIZE 3
#define I2C_READ_SIZE_1 1
#define I2C_READ_SIZE_2 7

//#define SL_I2CSPM_AHT20_PERIPHERAL               I2C1





// Read - write data
uint8_t initialization[I2C_WRITE_SIZE] = {0xBE, 0x08, 0x00};
uint8_t triggerMeasurement[I2C_WRITE_SIZE] = {0xAC, 0x33, 0x00};
//volatile uint8_t readData1[I2C_READ_SIZE_1];
//volatile uint8_t readData2[I2C_READ_SIZE_2];

AHT20_status_t AHT20_begin(void)
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

  result = I2C_TransferInit(SL_I2CSPM_MIKROE_PERIPHERAL, &i2cTransferBegin);
//  if (result != i2cTransferDone)
//    {
//      return SL_STATUS_TRANSMIT;
//    }
//  return  SL_STATUS_OK;
  while (result == i2cTransferInProgress)
       {
      result = I2C_Transfer(SL_I2CSPM_MIKROE_PERIPHERAL);
       }
     if(result != i2cTransferDone) return AHT20_ERROR;
     else return AHT20_OK;
}

AHT20_status_t AHT20_cmd(void)
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
  result = I2C_TransferInit(SL_I2CSPM_MIKROE_PERIPHERAL, &i2cTransferBegin);
    while (result == i2cTransferInProgress)
         {
        result = I2C_Transfer(SL_I2CSPM_MIKROE_PERIPHERAL);
         }
       if(result != i2cTransferDone) return AHT20_ERROR;
       else return AHT20_OK;


}

AHT20_status_t  Read_value_Sensor( uint32_t *temperature_raw, float *temperature_s,uint32_t *humidity_raw, float *humidity_s, uint8_t *readData2)          //Read_cmd
{
  //startSensor();

  sl_sleeptimer_delay_millisecond(100);
  I2C_TransferSeq_TypeDef i2cTransfer;
  I2C_TransferReturn_TypeDef result;

  // Initialize I2C transfer

  //uint8_t read_command[1] = {0x71};
  i2cTransfer.addr = AHT20_ADDRESS<<1;
  i2cTransfer.flags = I2C_FLAG_READ;
  i2cTransfer.buf[0].data = readData2;
  i2cTransfer.buf[0].len = 7;
  i2cTransfer.buf[1].data = NULL;
  i2cTransfer.buf[1].len = 0;

  result =  I2C_TransferInit(SL_I2CSPM_MIKROE_PERIPHERAL, &i2cTransfer);
  while (result == i2cTransferInProgress) {
      result = I2C_Transfer(SL_I2CSPM_MIKROE_PERIPHERAL);
     }
     if (result != i2cTransferDone) {
         return AHT20_ERROR;
     }
     else {

         sl_sleeptimer_delay_millisecond(75);

            *humidity_raw = (((uint32_t)readData2[1]) << 16) |
                            (((uint32_t)readData2[2]) << 8) |
                            (((uint32_t)readData2[3]) << 0);
            *humidity_raw = (*humidity_raw) >> 4;
            *humidity_s = (uint8_t)((float)(*humidity_raw) / 1048576.0f * 100.0f - 38.65); // convert the humidity
            sl_sleeptimer_delay_millisecond(1);
            *temperature_raw = (((uint32_t)readData2[3]) << 16) |
                               (((uint32_t)readData2[4]) << 8) |
                               (((uint32_t)readData2[5]) << 0);
           // *temperature_raw = (*temperature_raw) & 0xFFFFF; // cut the temperature part
            *temperature_raw = (*temperature_raw) << 12;
            *temperature_raw = (*temperature_raw) >> 12;
            *temperature_s = (float)(*temperature_raw)/ 1048576.0f * 200.0f - 50.0f;




    return AHT20_OK;
     }
     sl_sleeptimer_delay_millisecond(2000);

}

AHT20_status_t AHT20_init(void)
{
  AHT20_status_t sc;
  sc = AHT20_begin();
  if(sc == AHT20_ERROR)
    {
      return AHT20_ERROR;
    }
  sl_sleeptimer_delay_millisecond(200);
  sc = AHT20_cmd();
  if(sc ==  AHT20_ERROR)
    {
      return AHT20_ERROR;
    }
//  sc =  Read_value_Sensor(&temperature_raw,&temperature_s,&humidity_raw,&humidity_s,&data_humi_temp);
//  if(sc == AHT20_ERROR)
//    {
//      return AHT20_ERROR;
//    }
  return AHT20_OK;
}

