/***************************************************************************//**
 * @file
 * @brief Core application logic.
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/
#include "em_common.h"
#include "app_assert.h"
#include "sl_bluetooth.h"
#include "app.h"
#include "gatt_db.h"
#include "app_log.h"
#include "AHT20.h"
#include "BH1750.h"
#include "ring_buffer.h"
#include "sl_simple_led_instances.h"
#include "sl_simple_button_instances.h"
#include "sl_sleeptimer.h"
#include "app_timer.h"

// The advertising set handle allocated from Bluetooth stack.
static uint8_t advertising_set_handle = 0xff;
//static uint8_t connection_handle = 0xff;
static uint8_t connection_handle = 0;
// flag of sensor.
static bool flag_of_sensor = false;
//static bool flag_of_sensor = true;


//
//status_device status_of_device = PENDING_TRANSMISSION; // đầu tiên ở trạng thái chờ dữ liệu.
#define   DATA_TRANSMISSION  0  // trạng thái gửi dữ liệu
#define   PENDING_TRANSMISSION 1 // trạng thái chờ truyền dữ liệu.
//#define uint8_t SL_BT_HT_MEASUREMENT_INTERVAL_SEC  = 5;
static uint8_t set_timer = 1;
volatile uint8_t status_of_device = DATA_TRANSMISSION; // đầu tiên ở trạng thái chờ dữ liệu.

static ring_buffer_types queue;
static app_timer_t app_periodic_timer;

// Periodic timer callback.
static void app_periodic_timer_cb(app_timer_t *timer, void *data);

/**************************************************************************//**
 * Application Init.
 *****************************************************************************/
uint32_t temperature_raw = 0;
float temperature_s = 0;
uint32_t humidity_raw = 0;
float humidity_s = 0;

volatile  uint8_t data_humi_temp[7];
volatile uint8_t data_light[2];





SL_WEAK void app_init(void)
{
  /////////////////////////////////////////////////////////////////////////////
  // Put your additional application init code here!                         //
  // This is called once during start-up.                                    //
  /////////////////////////////////////////////////////////////////////////////
  AHT20_init();
  bh1750_i2c_init();
  queue.front = 0;
  queue.rear = 0;
  //sl_sleeptimer_delay_millisecond(3000);

  sl_bt_system_set_lazy_soft_timer(32768 * 2, 0, 0, 0);
  sl_bt_system_set_lazy_soft_timer(32768 / 5, 0, 1, 0);
  //  //sl_bt_evt_system_soft_timer_id;

}

/**************************************************************************//**
 * Application Process Action.
 *****************************************************************************/

SL_WEAK void app_process_action(void)
{
  /////////////////////////////////////////////////////////////////////////////
  // Put your additional application code here!                              //
  // This is called infinitely.                                              //
  // Do not call blocking functions from here!                               //
  /////////////////////////////////////////////////////////////////////////////
  if (flag_of_sensor ==  true)
    {
      flag_of_sensor = false;
      AHT20_status_t sc1;
      AHT20_init();
      sl_sleeptimer_delay_millisecond(10);
      //uint8_t readData2[7] = 0;
      sc1 = Read_value_Sensor(&temperature_raw,&temperature_s,&humidity_raw,&humidity_s,&data_humi_temp);

      sl_sleeptimer_delay_millisecond(10);
      bh1750_status_t sc2;
     // bh1750_i2c_init();
      sl_sleeptimer_delay_millisecond(10);
      sc2 = bh1750_i2c_read(CMD_H_RES_MODE ,&data_light);

      if(sc1 == AHT20_OK && sc2 == BH1750_OK)
        {
          uint8_t data[9] = {data_humi_temp[0],data_humi_temp[1],data_humi_temp[2],data_humi_temp[3],data_humi_temp[4],data_humi_temp[5],data_humi_temp[6],data_light[0],data_light[1]};
          status_ring_buffer status_ring_buffer;
          status_ring_buffer = ring_buffer_push_data(&queue, data);
          if(status_ring_buffer) {
              app_log("push data in ring buffer\n");
          }
          for(int i = 0;i < 9;i++)
            {
              app_log("%d  ",data[i]);
            }
          app_log("\n");
        }

      //          if(status_ring_buffer == RING_BUFFER_ERROR){
      //              printf("ERROR\n");
      //              return 1;
      //          }else{
      //
      //          }
     // sl_sleeptimer_delay_millisecond(5000);
    }
  //sl_sleeptimer_delay_millisecond(5000);



}

/**************************************************************************//**
 * Bluetooth stack event handler.
 * This overrides the dummy weak implementation.
 *
 * @param[in] evt Event coming from the Bluetooth stack.
 *****************************************************************************/


void sl_bt_on_event(sl_bt_msg_t *evt)
{
  {
    sl_status_t sc;
    bd_addr address;
    uint8_t address_type;
    uint8_t system_id[8];

    switch (SL_BT_MSG_ID(evt->header)) {
      // -------------------------------
      // This event indicates the device has started and the radio is ready.
      // Do not call any stack command before receiving this boot event!
      //addd
      case sl_bt_evt_system_soft_timer_id:
        if (evt->data.evt_system_soft_timer.handle == 0) {
            flag_of_sensor = true;
        }
        if (evt->data.evt_system_soft_timer.handle == 1 ) {
            if ((status_of_device & 0x1) == DATA_TRANSMISSION) {
                sl_led_toggle(&sl_led_led0);
            }
        }
        break;
        //add
      case sl_bt_evt_system_boot_id:

        // Extract unique ID from BT Address.
        sc = sl_bt_system_get_identity_address(&address, &address_type);
        app_assert_status(sc);
        //ass
                // Pad and reverse unique ID to get System ID.
                    system_id[0] = address.addr[5];
                    system_id[1] = address.addr[4];
                    system_id[2] = address.addr[3];
                    system_id[3] = 0xFF;
                    system_id[4] = 0xFE;
                    system_id[5] = address.addr[2];
                    system_id[6] = address.addr[1];
                    system_id[7] = address.addr[0];

        sc = sl_bt_gatt_server_write_attribute_value(gattdb_system_id,
                                                     0,
                                                     sizeof(system_id),
                                                     system_id);
        app_assert_status(sc);

        app_log("\nBLE stack booted\r\nStack version: %d.%d.%d\r\n",
                evt->data.evt_system_boot.major,
                evt->data.evt_system_boot.minor,
                evt->data.evt_system_boot.patch);
        app_log("Bluetooth %s address: %02X:%02X:%02X:%02X:%02X:%02X\n",
                address_type ? "static random" : "public device",
                    address.addr[5],
                    address.addr[4],
                    address.addr[3],
                    address.addr[2],
                    address.addr[1],
                    address.addr[0]);

        // Create an advertising set.
        sc = sl_bt_advertiser_create_set(&advertising_set_handle);
        app_assert_status(sc);

        // Generate data for advertising
        sc = sl_bt_legacy_advertiser_generate_data(advertising_set_handle,
                                                   sl_bt_advertiser_general_discoverable);
        app_assert_status(sc);
        // Set advertising interval to 100ms.
        sc = sl_bt_advertiser_set_timing(
            advertising_set_handle,
            160, // min. adv. interval (milliseconds * 1.6)
            160, // max. adv. interval (milliseconds * 1.6)
            0,   // adv. duration
            0);  // max. num. adv. events
        app_assert(sc == SL_STATUS_OK,
                   "[E: 0x%04x] Failed to set advertising timing\n",
                   (int)sc);
        // Start general advertising and enable connections.
        sc = sl_bt_legacy_advertiser_start(
            advertising_set_handle,
            //s sl_bt_advertiser_general_discoverable,//add
            sl_bt_advertiser_connectable_scannable);
        app_assert_status(sc);

        app_log_info("boot event - starting advertising\r\n");
        break;

        // -------------------------------
        // This event indicates that a new connection was opened.
      case sl_bt_evt_connection_opened_id:
        app_log_info("Connection opened\r\n");
        // Save connection handle for future reference
        connection_handle = evt->data.evt_connection_opened.connection;
        sc = app_timer_start(&app_periodic_timer,
                             set_timer * 1000,
                app_periodic_timer_cb,
                NULL,
                true);
        app_assert_status(sc);
            // Send first indication.
        app_periodic_timer_cb(&app_periodic_timer, NULL);
        break;

        // -------------------------------
        // This event indicates that a connection was closed.
      case sl_bt_evt_connection_closed_id:
        app_log_info("connection closed, reason: 0x%2.2x\r\n",
                     evt->data.evt_connection_closed.reason);
        connection_handle = 0xff;

        /* Stop timer in case client disconnected before indications were turned
         *   off */
        sc = sl_bt_system_set_lazy_soft_timer(0, 0, 0, 0);
        app_assert(sc == SL_STATUS_OK,
                   "[E: 0x%04x] Failed to stop a software timer\n",
                   (int)sc);

        // Restart advertising after client has disconnected.
        sc = sl_bt_legacy_advertiser_start(
            advertising_set_handle,
            sl_bt_advertiser_connectable_scannable);
        app_assert(sc == SL_STATUS_OK,
                   "[E: 0x%04x] Failed to start advertising\n",
                   (int)sc);
        break;

        /* This event is generated when a connected client has either
         * 1) changed a Characteristic Client Configuration, meaning that
         * they have enabled or disabled Notifications or Indications, or
         * 2) sent a confirmation upon a successful reception of the indication. */
      case sl_bt_evt_gatt_server_characteristic_status_id:
        /* Check that the characteristic in question is temperature -
         * its ID is defined in gatt_configuration.btconf as
         *   "temperature_measurement".
         * Also check that status_flags = 1, meaning that the characteristic
         * client configuration was changed (notifications or indications
         * enabled or disabled). */
//        if (evt->data.evt_gatt_server_characteristic_status.status_flags
//            != gatt_server_client_config) {
//            break;
//        }
//        if (evt->data.evt_gatt_server_characteristic_status.characteristic
//            != gattdb_value_sensor) {
//            break;
//        }
//
//        if (evt->data.evt_gatt_server_characteristic_status.client_config_flags
//            == gatt_indication) {
//            /* Indications have been turned ON - start the repeating timer.
//             * The 1st parameter '32768' tells the timer to run for
//             * 1 second (32.768 kHz oscillator), the 2nd parameter is
//             * the timer handle and the 3rd parameter '0' tells
//             * the timer to repeat continuously until stopped manually.*/
//            sc = sl_bt_system_set_lazy_soft_timer(32768, 0, 0, 0);
//            app_assert(sc == SL_STATUS_OK,
//                       "[E: 0x%04x] Failed to start a software timer\n",
//                       (int)sc);
//        } else if (evt->data.evt_gatt_server_characteristic_status.
//            client_config_flags
//            == gatt_disable) {
//            /* Indications have been turned OFF - stop the timer. */
//            sc = sl_bt_system_set_lazy_soft_timer(0, 0, 0, 0);
//            app_assert(sc == SL_STATUS_OK,
//                       "[E: 0x%04x] Failed to stop a software timer\n",
//                       (int)sc);
//        }
        break;

        /* This event is generated when the software timer has ticked.
         * In this example the temperature is read after every 1 second
         * and then the indication of that is sent to the listening client. */
        //      case sl_bt_evt_system_soft_timer_id:
        //        /* Measure the temperature as defined in the function
        //         *   measure_temperature() */
        //        //measure_temperature()
        //
        //        app_periodic_timer_cb();
        //        sc = sl_bt_system_set_lazy_soft_timer(32768 * 2, 0, 0, 0);
        //        break;

        ///////////////////////////////////////////////////////////////////////////
        // Add additional event handlers here as your application requires!      //
        ///////////////////////////////////////////////////////////////////////////
      case sl_bt_evt_gatt_server_attribute_value_id:
        // The value of the gattdb_led_control characteristic was changed.
        if (gattdb_button_characteristic == evt->data.evt_gatt_server_characteristic_status.characteristic) {
            //uint8_t size = evt->data.evt_gatt_server_attribute_value.value.len;
            uint8_t data = evt->data.evt_gatt_server_attribute_value.value.data[0];

            //            // Xử lý sau khi nhận nút bấm từ Gateway.
            //            app_log("da nhan duoc tin hieu nut nhan - data :%d\n", data);
            //            if(data) {
            //                status_of_device = (status_of_device == PENDING_TRANSMISSION) ? DATA_TRANSMISSION : PENDING_TRANSMISSION;
            //            }
            if(data) {
                status_of_device =  DATA_TRANSMISSION;
                app_log("turn on device\n");
            } else {
                sl_led_turn_off(&sl_led_led0);
                status_of_device = PENDING_TRANSMISSION;
                app_log("turn off device\n");
            }

            if (status_of_device == PENDING_TRANSMISSION) {
                //sl_bt_system_set_lazy_soft_timer(0, 0, 0, 0);
                app_timer_stop(&app_periodic_timer);
            } else {
                sc = app_timer_start(&app_periodic_timer,
                                         set_timer * 1000,
                                         app_periodic_timer_cb,
                                         NULL,
                                         true);
                // Send first indication.
//                app_periodic_timer_cb();
//                sl_bt_system_set_lazy_soft_timer(32768 * 2, 0, 0, 0);
                app_assert_status(sc);
                //app_periodic_timer_cb();
                // Send first indication.
                app_periodic_timer_cb(&app_periodic_timer, NULL);

            }
        }
        break;
        //      case sl_bt_evt_system_soft_timer_id:
        //        app_periodic_timer_cb();
        //        sl_bt_system_set_lazy_soft_timer(32768 * 2, 0, 0, 0);
        //        break;
        // -------------------------------
        // Default event handler.
      default:
        break;
    }
  }
}

uint8_t data_s[9];
static void app_periodic_timer_cb(app_timer_t *timer, void *data)
{
  (void)data;
  (void)timer;
  if (status_of_device ==  DATA_TRANSMISSION) {
      app_log("Sent data!\n");
      status_ring_buffer status_ring_buffer_pop;
      status_ring_buffer_pop = ring_buffer_pop_data(&queue, data_s);
      if (status_ring_buffer_pop != RING_BUFFER_ERROR || data_s[0] != 0 || data_s[2] != 0 || data_s[4] != 0 || data_s[6] != 0 || data_s[8] != 0) {
          sl_status_t sc;
          sc = sl_bt_gatt_server_send_indication(connection_handle, gattdb_value_sensor, 9, data_s);
          if (sc) {
              app_log_warning("Failed to send temperature measurement indication\n");
          }
      }
  }
}
