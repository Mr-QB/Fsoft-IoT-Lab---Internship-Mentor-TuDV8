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
#include <stdbool.h>
#include <math.h>
#include "em_common.h"
#include "app_log.h"
#include "app_assert.h"
#include "sl_bluetooth.h"
#ifdef SL_COMPONENT_CATALOG_PRESENT
#include "sl_component_catalog.h"
#endif // SL_COMPONENT_CATALOG_PRESENT
#ifdef SL_CATALOG_CLI_PRESENT
#include "sl_cli.h"
#endif // SL_CATALOG_CLI_PRESENT
#include "app.h"

#include "stdint.h"
#include "sl_simple_button_instances.h"

// connection parameters
#define CONN_INTERVAL_MIN             80   //100ms
#define CONN_INTERVAL_MAX             80   //100ms
#define CONN_RESPONDER_LATENCY        0    //no latency
#define CONN_TIMEOUT                  100  //1000ms
#define CONN_MIN_CE_LENGTH            0
#define CONN_MAX_CE_LENGTH            0xffff

#define SCAN_INTERVAL                 16   //10ms
#define SCAN_WINDOW                   16   //10ms
#define SCAN_PASSIVE                  0

#define TEMP_INVALID                  NAN
#define HUMI_INVALID                  NAN
#define UNIT_INVALID                  ('?')
#define UNIT_CELSIUS                  ('C')
#define UNIT_FAHRENHEIT               ('F')
#define RSSI_INVALID                  ((int8_t)0x7F)
#define CONNECTION_HANDLE_INVALID     ((uint8_t)0xFFu)
#define SERVICE_HANDLE_INVALID        ((uint32_t)0xFFFFFFFFu)
#define CHARACTERISTIC_HANDLE_INVALID ((uint16_t)0xFFFFu)
#define TABLE_INDEX_INVALID           ((uint8_t)0xFFu)
#define TX_POWER_INVALID              ((uint8_t)0x7C)
#define TX_POWER_CONTROL_ACTIVE       ((uint8_t)0x00)
#define TX_POWER_CONTROL_INACTIVE     ((uint8_t)0x01)
#define PRINT_TX_POWER_DEFAULT        (false)

#if SL_BT_CONFIG_MAX_CONNECTIONS < 1
#error At least 1 connection has to be enabled!
#endif

typedef enum {
  scanning,
  opening,
  discover_services,
  discover_button_characteristics,
  discover_button_characteristics_stop,
  discover_sensor_characteristics,
  enable_characteristics,
  running
} conn_state_t;

typedef struct {
  uint8_t  connection_handle;
  uint8_t  connection_char;
  int8_t   rssi;
  bool     power_control_active;
  int8_t   tx_power;
  int8_t   remote_tx_power;
  uint16_t server_address;
  uint32_t my_service_handle;  // my service.
  uint16_t button_characteristic_handle; // button characteristic.
  uint16_t sensor_characteristic_handle;  // sensor characteristic.
  uint16_t device_control_characteristic_handle; // device control characteristic.
  //uint16_t device_control_characteristic_handle; // device control characteristic.
  uint8_t  value_humi_value_0;
  uint8_t  value_humi_value_1;
  uint8_t  value_humi_value_2;
  uint8_t  value_humi_value_3;
  uint8_t  value_humi_value_4;
  uint8_t  value_humi_value_5;
  uint8_t  value_humi_value_6;
  uint8_t light_value_hight;
  uint8_t light_value_low;
} conn_properties_t;

// device status.
typedef enum {
  WAIT_DATA, // trạng thái chờ nhận dữ liệu.
  ACTIVE,     // trạng thái nhận dữ liệu.
  DISCONNECT   // trạng thái khi không có thiết bị kết nối.
}status_device;

status_device status_of_device = DISCONNECT; // đầu tiên ở trạng thái chờ dữ liệu gửi.

const uint8_t oledbw_img[] = {
    0xFF, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x81, 0x81, 0x81, 0x81, 0x81, 0x01, 0x41,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x81, 0x81, 0x81, 0x81, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x81, 0x81, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0xFF,
    0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xE0, 0xF0, 0xF0, 0xF8, 0xF8, 0x3C,
    0x1C, 0x0C, 0x06, 0x06, 0x02, 0x02, 0x03, 0x01, 0x01, 0x61, 0xF0, 0xF8, 0xFC, 0xFC, 0x7E, 0x7E,
    0x7F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x7E, 0x76,
    0x60, 0xE0, 0xE0, 0xE0, 0xF0, 0xF0, 0xF8, 0xFD, 0xFF, 0xFF, 0xFE, 0xFC, 0x78, 0x00, 0x00, 0xFF,
    0xFF, 0x00, 0x00, 0x00, 0x3C, 0x7E, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x1F, 0x1F, 0x0E,
    0x0C, 0x1C, 0x1C, 0xFC, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF9, 0xF9, 0xF9, 0xF9,
    0xF9, 0xF9, 0xF9, 0xFD, 0xFF, 0x7F, 0x7E, 0x3E, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x40,
    0x40, 0x60, 0x30, 0x39, 0x1F, 0x1F, 0x0F, 0x0F, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0xFF,
    0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x03, 0x06, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
    0xFF, 0x00, 0xCC, 0x12, 0x12, 0x12, 0xE4, 0x00, 0xFE, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 0xFE,
    0x00, 0x78, 0x84, 0x02, 0x02, 0x02, 0xCC, 0x00, 0x78, 0x84, 0x02, 0x02, 0x84, 0x78, 0x00, 0xFE,
    0x04, 0x18, 0x60, 0x80, 0xFE, 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x80, 0x7C, 0x42,
    0x7C, 0x80, 0x00, 0xFE, 0x12, 0x12, 0x12, 0xEC, 0x00, 0xCC, 0x12, 0x12, 0x12, 0xE4, 0x00, 0xFF,
    0xFF, 0x80, 0x80, 0x81, 0x81, 0x81, 0x80, 0x80, 0x81, 0x80, 0x81, 0x81, 0x81, 0x81, 0x80, 0x81,
    0x80, 0x80, 0x80, 0x81, 0x81, 0x81, 0x80, 0x80, 0x80, 0x80, 0x81, 0x81, 0x80, 0x80, 0x80, 0x81,
    0x80, 0x80, 0x80, 0x80, 0x81, 0x80, 0x80, 0x80, 0x81, 0x81, 0x81, 0x81, 0x80, 0x81, 0x80, 0x80,
    0x80, 0x81, 0x80, 0x81, 0x81, 0x81, 0x81, 0x80, 0x80, 0x80, 0x81, 0x81, 0x81, 0x80, 0x80, 0xFF
};
// Array for holding properties of multiple (parallel) connections
static conn_properties_t conn_properties[SL_BT_CONFIG_MAX_CONNECTIONS];
// Counter of active connections
static uint8_t active_connections_num;
// State of the connection under establishment
static conn_state_t conn_state;
// my service UUID defined by Bluetooth SIG
static const uint8_t my_service[2] = { 0x00, 0x00 };  // chỉnh ở đây dịch vụ cần gửi ở đây.
// button characteristic UUID defined by Bluetooth SIG
static const uint8_t button_char[2] = { 0x11, 0x11 };   // chỉnh sửa đặc trưng nút bấm cần gửi ở đây.
// sensor characteristic UUID defined by Bluetooth SIG
static const uint8_t sensor_char[2] = { 0x22, 0x22 };   // chỉnh sửa đặc trưng cảm biến cần gửi ở đây.
// Print out tx power value
static bool print_tx_power = PRINT_TX_POWER_DEFAULT;

static void init_properties(void);
static uint8_t find_service_in_advertisement(uint8_t *data, uint8_t len);
static uint8_t find_index_by_connection_handle(uint8_t connection);
static void add_connection(uint8_t connection, uint16_t address);
// Remove a connection from the connection_properties array
static void remove_connection(uint8_t connection);
static bd_addr *read_and_cache_bluetooth_address(uint8_t *address_type_out);
static void print_bluetooth_address(void);
// Print RSSI and temperature values
static void print_values(void);

static void oled_app_init(void);

// The advertising set handle allocated from Bluetooth stack.
static uint8_t advertising_set_handle = 0xff;

// flag print LCD.
static bool flag_print_LCD = false;
static bool flag_of_button = false;
sl_status_t sc;

/**************************************************************************//**
 * Application Init.
 *****************************************************************************/
SL_WEAK void app_init(void)
{
  sl_cli_instances_init();
  init_properties();
  oled_app_init();
    glib_init();
  //  st7735_init();
  //  displays_init();
  sl_bt_system_set_lazy_soft_timer(32768 * 10, 0, 0, 0);
  app_log_info("soc_thermometer_client initialised\n");
}

/**************************************************************************//**
 * Application Process Action.
 *****************************************************************************/
SL_WEAK void app_process_action(void)
{
  if (flag_print_LCD == true) {
      flag_print_LCD = false;
  uint8_t i;
  if(mode_display == DISPLAY_PICTURE)
    {
      mode_display = DISPLAY_TEXT;
      glib_draw_bmp(&glib_context, oledbw_img);
      sl_udelay_wait(500000);
      glib_set_invert_color();
      sl_udelay_wait(500000);
      glib_set_normal_color();
      sl_udelay_wait(500000);
      glib_set_invert_color();
      sl_udelay_wait(500000);
      glib_set_normal_color();

      for (i = 0x8F; i > 0x00; i--) {
          glib_set_contrast(i);
          sl_udelay_wait(5000);
      }

      for (i = 0x00; i < 0x8F; i++) {
          glib_set_contrast(i);
          sl_udelay_wait(5000);
      }

      glib_scroll_right(0x00, 0x06);
      sl_udelay_wait(1000000);
      glib_stop_scroll();
      glib_draw_bmp(&glib_context, oledbw_img);

      glib_scroll_left(0x00, 0x06);
      sl_udelay_wait(1000000);
      glib_stop_scroll();
      glib_draw_bmp(&glib_context, oledbw_img);

      glib_scroll_diag_right(0x00, 0x06);
      sl_udelay_wait(1000000);
      glib_stop_scroll();
      glib_draw_bmp(&glib_context, oledbw_img);

      glib_scroll_diag_left(0x00, 0x06);
      sl_udelay_wait(1000000);
      glib_stop_scroll();
    }
  //printf_temp_humi_data();
    oled_app_init();
    sl_sleeptimer_delay_millisecond(5000);
  }

  if (flag_of_button == true) {
      flag_of_button = false;

      // Xử lý sự kiện nhấn nút tại đây.
      for (int i = 0; i < active_connections_num; i++) {
          sc = sl_bt_gatt_write_characteristic_value(conn_properties[i].connection_char,
                                                     conn_properties[i].button_characteristic_handle, sizeof(status_of_device), &status_of_device);
          app_assert_status(sc);
          sl_sleeptimer_delay_millisecond(10);
      }
  }
}


/**************************************************************************//**
 * Bluetooth stack event handler.
 * This overrides the dummy weak implementation.
 *
 * @param[in] evt Event coming from the Bluetooth stack.
 *****************************************************************************/
void sl_bt_on_event(sl_bt_msg_t *evt)
{
  sl_status_t sc;
  uint8_t *char_value;
  uint16_t addr_value;
  uint8_t table_index;

  switch (SL_BT_MSG_ID(evt->header)) {
    case sl_bt_evt_system_soft_timer_id:
      if (evt->data.evt_system_soft_timer.handle == 0) {

          flag_print_LCD = true;
          mode_display = DISPLAY_PICTURE;
      }
      break;
      // -------------------------------
      // This event indicates the device has started and the radio is ready.
      // Do not call any stack command before receiving this boot event!
    case sl_bt_evt_system_boot_id:
      // Print boot message.
      app_log_info("Bluetooth stack booted: v%d.%d.%d-b%d\n",
                   evt->data.evt_system_boot.major,
                   evt->data.evt_system_boot.minor,
                   evt->data.evt_system_boot.patch,
                   evt->data.evt_system_boot.build);
      // Print bluetooth address.
      print_bluetooth_address();
      //    // Set passive scanning on 1Mb PHY
      // Set passive scanning on 1Mb PHY
      //        sc = sl_bt_scanner_set_mode(sl_bt_gap_1m_phy, SCAN_PASSIVE);
      //        app_assert_status(sc);
      //        // Set scan interval and scan window
      //        sc = sl_bt_scanner_set_timing(sl_bt_gap_1m_phy, SCAN_INTERVAL, SCAN_WINDOW);
      //        app_assert_status(sc);
      // Set the default connection parameters for subsequent connections
      sc = sl_bt_connection_set_default_parameters(CONN_INTERVAL_MIN,
                                                   CONN_INTERVAL_MAX,
                                                   CONN_RESPONDER_LATENCY,
                                                   CONN_TIMEOUT,
                                                   CONN_MIN_CE_LENGTH,
                                                   CONN_MAX_CE_LENGTH);
      app_assert_status(sc);
      // Start scanning - looking for thermometer devices
      sc = sl_bt_scanner_start(sl_bt_gap_1m_phy, sl_bt_scanner_discover_generic);
      app_assert_status_f(sc,
                          "Failed to start discovery #1\n");
      conn_state = scanning;
      break;

      // -------------------------------
      // This event is generated when an advertisement packet or a scan response
      // is received from a responder
      //  case sl_bt_evt_scanner_scan_report_id:
      //    // Parse advertisement packets
      //    if (evt->data.evt_scanner_scan_report.packet_type == 0) {
      //      // If a thermometer advertisement is found...
      //      if (find_service_in_advertisement(&(evt->data.evt_scanner_scan_report.data.data[0]),
      //          evt->data.evt_scanner_scan_report.data.len) != 0) {
      //        // then stop scanning for a while
      //        sc = sl_bt_scanner_stop();
      //        app_assert_status(sc);
      //        // and connect to that device
      //        if (active_connections_num < SL_BT_CONFIG_MAX_CONNECTIONS) {
      //          sc = sl_bt_connection_open(evt->data.evt_scanner_scan_report.address,
      //              evt->data.evt_scanner_scan_report.address_type,
      //              sl_bt_gap_1m_phy,
      //              NULL);
      //          app_assert_status(sc);
      //          status_of_device = ACTIVE;
      //          conn_state = opening;
      //        }
      //      }
      //    }
      //    break;

    case sl_bt_evt_scanner_legacy_advertisement_report_id:
      // Parse advertisement packets
      if (evt->data.evt_scanner_legacy_advertisement_report.event_flags
          == (SL_BT_SCANNER_EVENT_FLAG_CONNECTABLE | SL_BT_SCANNER_EVENT_FLAG_SCANNABLE)) {
          // If a thermometer advertisement is found...
          if (find_service_in_advertisement(&(evt->data.evt_scanner_legacy_advertisement_report.data.data[0]),
                                            evt->data.evt_scanner_legacy_advertisement_report.data.len) != 0) {
              // then stop scanning for a while
              sc = sl_bt_scanner_stop();
              app_assert_status(sc);
              // and connect to that device
              if (active_connections_num < SL_BT_CONFIG_MAX_CONNECTIONS) {
                  sc = sl_bt_connection_open(evt->data.evt_scanner_legacy_advertisement_report.address,
                                             evt->data.evt_scanner_legacy_advertisement_report.address_type,
                                             sl_bt_gap_phy_1m,
                                             NULL);
                  app_assert_status(sc);
                  conn_state = opening;
              }
          }
      }
      break;

      // -------------------------------
      // This event is generated when a new connection is established
      //  case sl_bt_evt_connection_opened_id:
      //    // Get last two bytes of sender address
      //    addr_value = (uint16_t)(evt->data.evt_connection_opened.address.addr[1] << 8) + evt->data.evt_connection_opened.address.addr[0];
      //
      //    // Add connection to the connection_properties array *** thấy được một kết nối sau đó lưu nó vào mảng kết nối, trong mảng này mới chỉ có 2 sự kiện được lưu.
      //    add_connection(evt->data.evt_connection_opened.connection, addr_value);
      //    // Discover Health Thermometer service on the responder device
      //
      //
      //    // chinh code o day. *** => Ở đây nó kiểm tra ID mà dịch vụ đang quảng cáo, nếu có dịch vụ nó cần thiết thì nó sẽ kết nối.
      //    sc = sl_bt_gatt_discover_primary_services_by_uuid(evt->data.evt_connection_opened.connection,
      //        sizeof(my_service),
      //        (const uint8_t*)my_service);
      //    app_assert_status(sc);
      //
      //    conn_state = discover_services;
      //    break;

    case sl_bt_evt_connection_opened_id:
      // Discover Health Thermometer service on the responder device
      sc = sl_bt_gatt_discover_primary_services_by_uuid(evt->data.evt_connection_opened.connection,
                                                        sizeof(my_service),
                                                        (const uint8_t*)my_service);

      if (sc == SL_STATUS_INVALID_HANDLE) {
          // Failed to open connection, restart scanning
          app_log_warning("Primary service discovery failed with invalid handle, dropping client\n");
          sc = sl_bt_scanner_start(sl_bt_gap_phy_1m, sl_bt_scanner_discover_generic);
          app_assert_status(sc);
          conn_state = scanning;
          break;
      } else {
          app_assert_status(sc);
      }

      // Get last two bytes of sender address
      addr_value = (uint16_t)(evt->data.evt_connection_opened.address.addr[1] << 8) + evt->data.evt_connection_opened.address.addr[0];
      // Add connection to the connection_properties array
      add_connection(evt->data.evt_connection_opened.connection, addr_value);

      // Set remote connection power reporting - needed for Power Control
      sc = sl_bt_connection_set_remote_power_reporting(
          evt->data.evt_connection_opened.connection,
          sl_bt_connection_power_reporting_enable);
      app_assert_status(sc);
      app_log_info("Connection opened\n");

      conn_state = discover_services;
      break;

      // -------------------------------
      // This event is generated when a new service is discovered. Hàm này khi có một dịch vụ gửi đến nó sẽ kiểm tra trong mảng các thiết bị kết nối xem có phải thiết bị dã kết nối hay không? nếu có nó lưu lại dịch vụ.
    case sl_bt_evt_gatt_service_id:
      table_index = find_index_by_connection_handle(evt->data.evt_gatt_service.connection);
      if (table_index != TABLE_INDEX_INVALID) {
          // Save service handle for future reference
          conn_properties[table_index].my_service_handle = evt->data.evt_gatt_service.service;
      }
      break;
    case sl_bt_evt_gatt_characteristic_id:
      table_index = find_index_by_connection_handle(evt->data.evt_gatt_characteristic.connection);
      if (table_index != TABLE_INDEX_INVALID && conn_state == discover_sensor_characteristics) {
          // Save characteristic handle for future reference
          conn_properties[table_index].sensor_characteristic_handle = evt->data.evt_gatt_characteristic.characteristic;
          app_log("sensor_characteristic_handle = %d\n", conn_properties[table_index].sensor_characteristic_handle);
      }
      if (table_index != TABLE_INDEX_INVALID && conn_state == discover_button_characteristics_stop) {
          // Save characteristic handle for future reference
          conn_properties[table_index].button_characteristic_handle = evt->data.evt_gatt_characteristic.characteristic;
          app_log("button_characteristic_handle = %d\n", conn_properties[table_index].button_characteristic_handle);
      }
      break;

      // -------------------------------
      // This event is generated (được tạo ra) for various (nhiều) procedure (thủ tục) completions, e.g. when a
      // write procedure is completed, or service discovery is completed
    case sl_bt_evt_gatt_procedure_completed_id:
      table_index = find_index_by_connection_handle(evt->data.evt_gatt_procedure_completed.connection);
      if (table_index == TABLE_INDEX_INVALID) {
          break;
      }
      // If service discovery (khám phá) finished
      if (conn_state == discover_services && conn_properties[table_index].my_service_handle != SERVICE_HANDLE_INVALID) {
          // Discover (phát hiện) button characteristic on the responder device
          conn_properties[table_index].connection_char = evt->data.evt_gatt_procedure_completed.connection;
          sc = sl_bt_gatt_discover_characteristics_by_uuid(conn_properties[table_index].connection_char,
                                                           conn_properties[table_index].my_service_handle,
                                                           sizeof(sensor_char),
                                                           (const uint8_t*)sensor_char);
          app_assert_status(sc);
          conn_state = discover_sensor_characteristics;
          break;
      }
      // If characteristic discovery finished
      if (conn_state == discover_sensor_characteristics && conn_properties[table_index].sensor_characteristic_handle != CHARACTERISTIC_HANDLE_INVALID) {
          // enable indications
          sc = sl_bt_gatt_set_characteristic_notification(conn_properties[table_index].connection_char,
                                                          conn_properties[table_index].sensor_characteristic_handle,
                                                          sl_bt_gatt_indication);
          app_assert_status(sc);
          conn_state = discover_button_characteristics;
          break;
      }

      if (conn_state == discover_button_characteristics && conn_properties[table_index].my_service_handle != SERVICE_HANDLE_INVALID) {
          // Discover (phát hiện) button characteristic on the responder device
          sc = sl_bt_gatt_discover_characteristics_by_uuid(conn_properties[table_index].connection_char,
                                                           conn_properties[table_index].my_service_handle,
                                                           sizeof(button_char),
                                                           (const uint8_t*)button_char);
          app_assert_status(sc);
          conn_state = discover_button_characteristics_stop;
          break;
      }
      // If characteristic discovery finished
      if (conn_state == discover_button_characteristics_stop && conn_properties[table_index].button_characteristic_handle != CHARACTERISTIC_HANDLE_INVALID) {
          // stop discovering
          sl_bt_scanner_stop();
          conn_state = enable_characteristics;
          break;
      }

      // If indication enable process finished
      if (conn_state == enable_characteristics) {
          // and we can connect to more devices
          if (active_connections_num < SL_BT_CONFIG_MAX_CONNECTIONS) {
              // start scanning again to find new devices
              sc = sl_bt_scanner_start(sl_bt_gap_1m_phy, sl_bt_scanner_discover_generic);
              app_assert_status_f(sc,
                                  "Failed to start discovery #2\n");
              conn_state = scanning;
          } else {
              conn_state = running;
          }
          break;
      }
      break;

      // -------------------------------
      // This event indicates that a connection was closed.
    case sl_bt_evt_connection_closed_id:
      // remove connection from active connections
      remove_connection(evt->data.evt_connection_closed.connection);
      if (conn_state != scanning) {
          // start scanning again to find new devices
          sc = sl_bt_scanner_start(sl_bt_gap_1m_phy, sl_bt_scanner_discover_generic);
          app_assert_status_f(sc,
                              "Failed to start discovery #3\n");
          conn_state = scanning;
      }
      break;

      ///////////////////////////////////////////////////////////////////////////
      // Add additional event handlers here as your application requires!      //
      ///////////////////////////////////////////////////////////////////////////
    case sl_bt_evt_gatt_characteristic_value_id:
      if (evt->data.evt_gatt_characteristic_value.value.len >= 6) {
          char_value = &(evt->data.evt_gatt_characteristic_value.value.data[0]);
          table_index = find_index_by_connection_handle(evt->data.evt_gatt_characteristic_value.connection);
          if (table_index != TABLE_INDEX_INVALID) {
              app_log_info("Da nhan thanh cong\n");
              conn_properties[table_index].value_humi_value_0 = *(char_value);
              conn_properties[table_index].value_humi_value_1 = *(char_value + 1);
              conn_properties[table_index].value_humi_value_2 = *(char_value + 2);
              conn_properties[table_index].value_humi_value_3 = *(char_value + 3);
              conn_properties[table_index].value_humi_value_4 = *(char_value + 4);
              conn_properties[table_index].value_humi_value_5 = *(char_value + 5);
              conn_properties[table_index].value_humi_value_6 = *(char_value + 6);
              conn_properties[table_index].light_value_hight = *(char_value + 7);
              conn_properties[table_index].light_value_low = *(char_value + 8);

          }
      } else {
          app_log_warning("Characteristic value too short: %d\n",
                          evt->data.evt_gatt_characteristic_value.value.len);
      }
      // Send confirmation for the indication
      sc = sl_bt_gatt_send_characteristic_confirmation(evt->data.evt_gatt_characteristic_value.connection);
      app_assert_status(sc);
      break;
    case sl_bt_evt_gatt_server_attribute_value_id:
      app_log_info("button\n");
      break;
    default:
      break;
      // -------------------------------
      // Default event handler.
  }
}

/**************************************************************************//**
 * Simple Button
 * Button state changed callback
 * @param[in] handle Button event handle
 *****************************************************************************/
void sl_button_on_change(const sl_button_t *handle)
{


  // Button pressed.
  if (sl_button_get_state(handle) == SL_SIMPLE_BUTTON_PRESSED) {
      if (&sl_button_btn0 == handle) {
          status_of_device = (status_of_device == WAIT_DATA) ? ACTIVE : WAIT_DATA;
          flag_of_button = true;
//          if (mode_display == DISPLAY_TEXT) {
//                       mode_display = DISPLAY_PICTURE;
//                       flag_print_LCD = true;
//                   }
//      }else if(&sl_button_btn1 == handle) {
//          if (mode_display == DISPLAY_TEXT) {
//              mode_display = DISPLAY_PICTURE;
//              flag_print_LCD = true;
//          }
//      }
  }
}
}





// Init connection properties
static void init_properties(void)
{
  uint8_t i;
  active_connections_num = 0;

  for (i = 0; i < SL_BT_CONFIG_MAX_CONNECTIONS; i++) {
      conn_properties[i].connection_handle = CONNECTION_HANDLE_INVALID;
      conn_properties[i].connection_char = CONNECTION_HANDLE_INVALID;
      conn_properties[i].my_service_handle = SERVICE_HANDLE_INVALID;
      conn_properties[i].button_characteristic_handle = CHARACTERISTIC_HANDLE_INVALID;
      conn_properties[i].sensor_characteristic_handle = CHARACTERISTIC_HANDLE_INVALID;
      conn_properties[i].value_humi_value_0 = 0;
      conn_properties[i].value_humi_value_1 = 0;
      conn_properties[i].value_humi_value_2 = 0;
      conn_properties[i].value_humi_value_3 = 0;
      conn_properties[i].value_humi_value_4 = 0;
      conn_properties[i].value_humi_value_5 = 0;
      conn_properties[i].value_humi_value_6 = 0;
      conn_properties[i].light_value_hight = 0;
      conn_properties[i].light_value_low = 0;
      conn_properties[i].rssi = RSSI_INVALID;
      conn_properties[i].power_control_active = TX_POWER_CONTROL_INACTIVE;
      conn_properties[i].tx_power = TX_POWER_INVALID;
      conn_properties[i].remote_tx_power = TX_POWER_INVALID;
      app_log_info("%u",conn_properties[i].value_humi_value_0);
  }
}

/**************************************************************************//**
 * @brief
 *   Function to Print Bluetooth Address.
 * @return
 *   None
 *****************************************************************************/
static void print_bluetooth_address(void)
{
  uint8_t address_type;
  bd_addr *address = read_and_cache_bluetooth_address(&address_type);

  app_log_info("Bluetooth %s address: %02X:%02X:%02X:%02X:%02X:%02X\n",
               address_type ? "static random" : "public device",
                   address->addr[5],
                   address->addr[4],
                   address->addr[3],
                   address->addr[2],
                   address->addr[1],
                   address->addr[0]);
}

/**************************************************************************//**
 * @brief
 *   Function to Read and Cache Bluetooth Address.
 * @param address_type_out [out]
 *   A pointer to the outgoing address_type. This pointer can be NULL.
 * @return
 *   Pointer to the cached Bluetooth Address
 *****************************************************************************/
static bd_addr *read_and_cache_bluetooth_address(uint8_t *address_type_out)
{
  static bd_addr address;
  static uint8_t address_type;
  static bool cached = false;

  if (!cached) {
      sl_status_t sc = sl_bt_system_get_identity_address(&address, &address_type);
      app_assert_status(sc);
      cached = true;
  }

  if (address_type_out) {
      *address_type_out = address_type;
  }

  return &address;
}

// Parse advertisements looking for advertised Health Thermometer service
static uint8_t find_service_in_advertisement(uint8_t *data, uint8_t len)
{
  uint8_t ad_field_length;
  uint8_t ad_field_type;
  uint8_t i = 0;
  // Parse advertisement packet
  while (i < len) {
      ad_field_length = data[i];
      ad_field_type = data[i + 1];
      // Partial ($02) or complete ($03) list of 16-bit UUIDs
      if (ad_field_type == 0x02 || ad_field_type == 0x03) {
          // compare UUID to Health Thermometer service UUID
          if (memcmp(&data[i + 2], my_service, 2) == 0) {
              return 1;
          }
      }
      // advance to the next AD struct
      i = i + ad_field_length + 1;
  }
  return 0;
}

// Add a new connection to the connection_properties array
static void add_connection(uint8_t connection, uint16_t address)
{
  conn_properties[active_connections_num].connection_handle = connection;
  conn_properties[active_connections_num].server_address    = address;
  active_connections_num++;
}

// Find the index of a given connection in the connection_properties array
static uint8_t find_index_by_connection_handle(uint8_t connection)
{
  for (uint8_t i = 0; i < active_connections_num; i++) {
      if (conn_properties[i].connection_handle == connection) {
          return i;
      }
  }
  return TABLE_INDEX_INVALID;
}

// Remove a connection from the connection_properties array
static void remove_connection(uint8_t connection)
{
  uint8_t i;
  uint8_t table_index = find_index_by_connection_handle(connection);

  if (active_connections_num > 0) {
      active_connections_num--;
  }
  // Shift entries after the removed connection toward 0 index
  for (i = table_index; i < active_connections_num; i++) {
      conn_properties[i] = conn_properties[i + 1];
  }
  // Clear the slots we've just removed so no junk values appear
  for (i = active_connections_num; i < SL_BT_CONFIG_MAX_CONNECTIONS; i++) {
      conn_properties[i].connection_handle = CONNECTION_HANDLE_INVALID;
      conn_properties[i].connection_char = CONNECTION_HANDLE_INVALID;
      conn_properties[i].my_service_handle = SERVICE_HANDLE_INVALID;
      conn_properties[i].button_characteristic_handle = CHARACTERISTIC_HANDLE_INVALID;
      conn_properties[i].sensor_characteristic_handle = CHARACTERISTIC_HANDLE_INVALID;
      conn_properties[i].value_humi_value_0 = 0;
      conn_properties[i].value_humi_value_1 = 0;
      conn_properties[i].value_humi_value_2 = 0;
      conn_properties[i].value_humi_value_3 = 0;
      conn_properties[i].value_humi_value_4 = 0;
      conn_properties[i].value_humi_value_5 = 0;
      conn_properties[i].value_humi_value_6 = 0;
      conn_properties[i].light_value_hight = 0;
      conn_properties[i].light_value_low = 0;
      conn_properties[i].power_control_active = TX_POWER_CONTROL_INACTIVE;
      conn_properties[i].tx_power = TX_POWER_INVALID;
      conn_properties[i].remote_tx_power = TX_POWER_INVALID;
  }
}
static void oled_app_init(void)
{
  /* Initialize the display */

  uint32_t humi_raw = (((uint32_t)conn_properties[0].value_humi_value_1) << 16) |
      (((uint32_t)conn_properties[0].value_humi_value_2) << 8) |
      (((uint32_t)conn_properties[0].value_humi_value_3) << 0);
  humi_raw = (humi_raw) >> 4;
  float humi_value = (uint8_t)((float)(humi_raw) / 1048576.0f * 100.0f + 70.00); // convert the humidity

  uint32_t temp_raw = (((uint32_t)conn_properties[0].value_humi_value_3) << 16) |
      (((uint32_t)conn_properties[0].value_humi_value_4) << 8) |
      (((uint32_t)conn_properties[0].value_humi_value_5) << 0);
  temp_raw = (temp_raw) << 12;
  temp_raw = (temp_raw) >> 12;
  float temp_value = (float)(temp_raw)/ 1048576.0f * 200.0f - 50.0f;

  app_log_info("humi = %.2f%%, temp =  %.2f*C\n", humi_value,temp_value);
  uint16_t light = (uint16_t) (conn_properties[0].light_value_hight << 8 | conn_properties[0].light_value_low);
  app_log_info("light = %d\n", light);


  glib_context.backgroundColor = Black;
  glib_context.foregroundColor = White;


  char strTemp[20];   // Đảm bảo chuỗi đủ lớn để chứa kết quả
  char strHumidity[20]; // Đảm bảo chuỗi đủ lớn để chứa kết quả
  char strlight[16];

  sprintf(strTemp, "%.2f", temp_value);
  sprintf(strHumidity, "%.2f", humi_value);
  sprintf(strlight, "%.d", light);
  /* Fill lcd with background color */
  glib_clear(&glib_context);

  /* Use Narrow font */
  glib_set_font(&glib_context, (glib_font_t *)&glib_font_6x8);

  glib_draw_string(&glib_context, "Temp:", 0, 0);
  glib_draw_string(&glib_context, strTemp, 40, 0); // Vẽ temp ở bên phải của "t :"
  glib_draw_string(&glib_context, "*C", 80, 0);

  glib_draw_string(&glib_context, "humi:", 0, 10);
  glib_draw_string(&glib_context, strHumidity, 40, 10); // Vẽ humidity ở bên phải của "humidity:"
  glib_draw_string(&glib_context, "%", 80, 10);

  glib_draw_string(&glib_context, "light:", 0, 20);
  glib_draw_string(&glib_context, strlight, 50, 20);
  glib_draw_string(&glib_context, "lux", 70, 20);

  glib_update_display();

}


#ifdef SL_CATALOG_CLI_PRESENT
void hello(sl_cli_command_arg_t *arguments)
{
  (void) arguments;
  print_bluetooth_address();
}

void toggle_print_tx_power(sl_cli_command_arg_t *arguments)
{
  (void) arguments;
  print_tx_power = !print_tx_power;
}


void cmd_set_timer_handler(sl_cli_command_arg_t *arguments) {
  int argc = sl_cli_get_argument_count(arguments);
  if (argc < 2) {
      printf("Not enough arguments. Expected 2! \n");
      return;
  } else {
      uint8_t device = sl_cli_get_argument_uint8(arguments, 0);
      uint8_t timer = sl_cli_get_argument_uint8(arguments, 1);

      //set timer for device.
      sc = sl_bt_gatt_write_characteristic_value(conn_properties[device].connection_char,
                                                 conn_properties[device].device_control_characteristic_handle, sizeof(timer), &timer);
      app_assert_status(sc);
      printf("timer setting for device %d is successful!\n", device);
  }
}

void cmd_set_connect_device_handler(sl_cli_command_arg_t *arguments) {
  int argc = sl_cli_get_argument_count(arguments);
  if (argc < 2) {
      printf("Not enough arguments. Expected 2! \n");
      return;
  } else {
      uint8_t device = sl_cli_get_argument_uint8(arguments, 0);
      uint8_t stage_sent = sl_cli_get_argument_uint8(arguments, 1); // 1 - on, 0 - off.

      sc = sl_bt_gatt_write_characteristic_value(conn_properties[device].connection_char,
                                                 conn_properties[device].button_characteristic_handle, sizeof(stage_sent), &stage_sent);
      app_assert_status(sc);
      printf("control signal sent successfully to device %d!\n", device);
  }
}
#endif // SL_CATALOG_CLI_PRESENT
