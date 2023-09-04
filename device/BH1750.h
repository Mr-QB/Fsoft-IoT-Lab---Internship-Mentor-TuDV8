/*
 * bh1750.h
 *
 *  Created on: Jul 31, 2023
 *      Author: amin
 */

#ifndef BH1750_H_
#define BH1750_H_

#include "sl_i2cspm_instances.h"
#include "sl_sleeptimer.h"
#include "sl_status.h"
#include "stdint.h"
#include "app_log.h"

#define BH1750_ADDR_WRITE       0x46  // this is address of bh1750.
#define CMD_POWER_DOWN          0x00    // this is command power down.
#define CMD_POWER_ON            0x01  // this is command power on.
#define CMD_RESET               0x03  // command reset.
#define CMD_H_RES_MODE          0x10  // command start measurement at 1lx resolution. measurement Time is typically 120ms.
#define CMD_H_RES_MODE2         0x11    // start measurement at 0.5lx resolution. measurement Time is typically 120ms.
#define CMD_L_RES_MODE          0x13    // start measurement at 4lx resolution. measurement Time is typically 16ms.
#define CMD_ONE_H_RES_MODE      0x20  // start measurement at 1lx resolution.measurement Time is typically 120ms.
#define CMD_ONE_H_RES_MODE2     0x21    // start measurement at 0.5lx resolution
#define CMD_ONE_L_RES_MODE      0x23  // start measurement at 4lx resolution.
#define CMD_CNG_TIME_HIGH       0x30    // 3 LSB set time
#define CMD_CNG_TIME_LOW        0x60    // 5 LSB set time

typedef enum {
  BH1750_ERROR,
  BH1750_OK
} bh1750_status_t;

/***************************************************************************//**
 * @brief
 *   Write a byte to the sensor via I2C.
 * @param[in] address
 *   The register address of the sensor.
 * @param[in] data
 *   Data in transfer.
 * @return
 *    @ref SL_STATUS_OK on success or @ref SL_STATUS_TRANSMIT on failure
 ******************************************************************************/
bh1750_status_t bh1750_i2c_init(void);
bh1750_status_t bh1750_i2c_write(uint8_t data);
bh1750_status_t bh1750_i2c_read(uint8_t mode, uint8_t *data);

#endif /* BH1750_H_ */
