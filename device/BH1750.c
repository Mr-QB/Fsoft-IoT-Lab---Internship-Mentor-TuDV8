/*
 * bh1750.c
 *
 *  Created on: 22 thg 8, 2023
 *      Author: ADMIN
 */


/*
 * bh1750.c
 *
 *  Created on: Aug 8, 2023
 *      Author: ha phong
 */


#include "sl_i2cspm.h"
#include "sl_i2cspm_mikroe_config.h"
#include "stdio.h"
#include "string.h"
#include "bh1750.h"

bh1750_status_t bh1750_i2c_init(void)
{
  bh1750_status_t sc;
  sc = bh1750_i2c_write(CMD_POWER_DOWN);
  if (sc == BH1750_ERROR) {
    return BH1750_ERROR;
  }
  sl_sleeptimer_delay_millisecond(2);
  sc = bh1750_i2c_write(CMD_POWER_ON);
  if (sc == BH1750_ERROR) {
    return BH1750_ERROR;
  }
  sc = bh1750_i2c_write(CMD_H_RES_MODE);
  if (sc == BH1750_ERROR) {
    return BH1750_ERROR;
  }
  sl_sleeptimer_delay_millisecond(200);
  return BH1750_OK;
}

/**************************************************************************//**
 * @brief Write a byte to Maxim register.
 *****************************************************************************/
bh1750_status_t bh1750_i2c_write(uint8_t data)
{
  I2C_TransferSeq_TypeDef    seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t i2c_write_data[1];
  uint8_t i2c_read_data[1];

  seq.addr = BH1750_ADDR_WRITE;
  seq.flags = I2C_FLAG_WRITE;
  /* Select register and data to write */
  i2c_write_data[0] = data ;
  //  i2c_write_data[1] = data;
  seq.buf[0].data = i2c_write_data;
  seq.buf[0].len  = 1;
  /* Select length of data to be read */
  seq.buf[1].data = i2c_read_data;
  seq.buf[1].len  = 0;

  ret = I2CSPM_Transfer(SL_I2CSPM_MIKROE_PERIPHERAL, &seq);
  if (ret != i2cTransferDone) {
    return BH1750_ERROR;
  }

  return BH1750_OK;
}


/**************************************************************************//**
 * @brief Write a byte to Maxim register.
 *****************************************************************************/
bh1750_status_t bh1750_i2c_read(uint8_t mode, uint8_t *data)
{
  I2C_TransferSeq_TypeDef    seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t i2c_write_data[1];
  uint8_t i2c_read_data[2];

  seq.addr = BH1750_ADDR_WRITE;
  seq.flags = I2C_FLAG_WRITE_READ;
  i2c_write_data[0] = mode;
  seq.buf[0].data = i2c_write_data;
  seq.buf[0].len  = 1;
  /* Select length of data to be read */
  seq.buf[1].data = i2c_read_data;
  seq.buf[1].len  = 2;

  ret = I2CSPM_Transfer(SL_I2CSPM_MIKROE_PERIPHERAL, &seq);
  if (ret != i2cTransferDone) {
    return BH1750_ERROR;
  }
  *data = i2c_read_data[0];
  *(data + 1) = i2c_read_data[1];
  return BH1750_OK;
}
