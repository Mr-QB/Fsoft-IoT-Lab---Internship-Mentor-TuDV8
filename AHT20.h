/*
 * AHT20.h
 *
 *  Created on: 30 thg 7, 2023
 *      Author: ADMIN
 */
#include <stdio.h>
#include "sl_i2cspm.h"
#include "sl_i2cspm_AHT20_config.h"
#include "sl_sleeptimer.h"
#include "em_i2c.h"

#ifndef AHT20_H_
#define AHT20_H_

sl_status_t AHT20_begin(void);
sl_status_t AHT20_cmd(void);
sl_status_t startSensor(void);
//bool getSensor();

sl_status_t  getSensor( uint32_t *temperature_raw, float *temperature_s,uint32_t *humidity_raw, float *humidity_s) ;

float getTemperature(void);
float getHumidity(void);
void printf_temp_humi_data();

#endif /* AHT20_H_ */
