/*
 * AHT20.h
 *
 *  Created on: 30 thg 7, 2023
 *      Author: ADMIN
 */

#ifndef AHT20_H_
#define AHT20_H_
#include <stdio.h>
#include "sl_i2cspm.h"
#include "sl_i2cspm_mikroe_config.h"
#include "sl_sleeptimer.h"
#include "em_i2c.h"

typedef enum{
  AHT20_ERROR,
  AHT20_OK
}AHT20_status_t;

AHT20_status_t AHT20_begin(void);
AHT20_status_t AHT20_cmd(void);

AHT20_status_t  Read_value_Sensor( uint32_t *temperature_raw, float *temperature_s,uint32_t *humidity_raw, float *humidity_s, uint8_t *readData2) ;
AHT20_status_t AHT20_init(void);


#endif /* AHT20_H_ */
