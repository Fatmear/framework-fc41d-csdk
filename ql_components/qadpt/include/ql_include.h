#ifndef __QL_INCLUDE_H_
#define __QL_INCLUDE_H_

#include "include.h"
#include <string.h>
#include <stdlib.h>

#include "stdio.h"
#include "str_pub.h"

#include "quectel_demo_config.h"
#include "app_comm.h"
#include "sys_ctrl.h"
#include "power_save_pub.h"

#include "stdarg.h"
#include "mem_pub.h"
#include "sys_rtos.h"
#include "rtos_pub.h"
#include "error.h"
#include "sys_ctrl_pub.h"
#include "arm_mcu_pub.h"
#include "drv_model_pub.h"
#include "arm_arch.h"
#include "common.h"
#include "param_config.h"

#include "intc_pub.h"
#include "icu_pub.h"

#include "generic.h"
#include "fake_clock_pub.h"

//peri
#include "uart_pub.h"
#include "uart.h"
#include "gpio_pub.h"
#include "gpio.h"
#include "saradc_pub.h"
#include "saradc_intf.h"
#include "flash_pub.h"
#include "timers.h"
#include "bk_timer_pub.h"

#include "i2c_pub.h"
#include "low_voltage_ps.h"
#include "manual_ps_pub.h"
#include "power_save_pub.h"
#include "pwm_pub.h"
#include "pwm_bk7231n.h"
#include "spi_pub.h"
#include "general_dma_pub.h"
#include "general_dma.h"
#include "spi_flash.h"
#include "BkDriverTimer.h"
#include "BkDriverWdg.h"
#include "BkDriverUart.h"
#include "BkDriverFlash.h"
#include "BkDriverPwm.h"


//wlan

#include "wlan_cli_pub.h"
#include "wlan_ui_pub.h"
#include "rw_msg_pub.h"
#include "param_config.h"
#include "rwnx_config.h"
#include "app.h"
#include "ps.h"
#include "sa_ap.h"
#include "sa_station.h"
#include "main_none.h"
#include "sm.h"
#include "rw_pub.h"
#include "ethernet_intf.h"
#include "ieee802_11_defs.h"
#include "airkiss_main.h"




//ble
#include "ble_ui.h"
#include "app_ble.h"
#include "app_sdp.h"
#if (BLE_APP_PRESENT && (BLE_CENTRAL))
#include "app_ble_init.h"
#if BLE_SDP_CLIENT
#include "app_sdp.h"
#endif
#endif
#include "gattc.h"
#include "ble_api_5_x.h"
#include "app_task.h"
#include "app_sec.h"

//net
#include "net.h"
#include "sockets.h"
#include "tcp_mqtt_client_port.h"
#include "mqtt_client_core.h"
#include "mqtt_client_com_port.h"
#include "sl_tls.h"
#include "lwip/opt.h"
#include "lwip/apps/httpd.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/ip_addr.h"
#include "lwip/inet.h"


//quectel api
#include "ql_ble.h"
#include "ql_adc.h"
#include "ql_api_osi.h"
#include "ql_flash.h"
#include "ql_gpio.h"
#include "ql_i2c1_eeprom.h"
#include "ql_lowpower.h"
#include "ql_pwm.h"
#include "ql_spi.h"
#include "ql_timer.h"
#include "ql_spi_flash.h"
#include "ql_uart.h"
#include "ql_watchdog.h"
#include "ql_wlan.h"



#endif

