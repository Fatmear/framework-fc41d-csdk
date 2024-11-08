
#include "include.h"
#include "rtos_pub.h"
#include "error.h"
#include "uart_pub.h"
#include "drv_model_pub.h"
#include "quectel_demo_config.h"

#if CFG_ENABLE_QUECTEL_DEMO
#include "ql_uart.h"
#include "ql_timer.h"
#include "ql_i2c1_eeprom.h"
#include "ql_watchdog.h"
#include "ql_spi.h"
#include "ql_spi_flash.h"
#include "ql_pwm.h"
#include "ql_gpio.h"
#include "ql_flash.h"
#include "ql_adc.h"
#include "ql_ble.h"
#include "ql_api_osi.h"
#include "ql_lowpower.h"
#include "wizchip_port.h"
#include "ql_ftp_client_c.h"




void ql_init_demo_start()
{
#if CFG_ENABLE_QUECTEL_MQTT
   ql_mqtt_demo_thread_creat();
#endif

#if CFG_ENABLE_QUECTEL_FTP
   ql_ftp_demo_thread_creat();
#endif

#if CFG_ENABLE_QUECTEL_UART
   ql_uart_demo_thread_creat();
#endif

#if CFG_ENABLE_QUECTEL_I2C1
   ql_i2c_demo_thread_creat();
#endif

#if CFG_ENABLE_QUECTEL_TIMER
   ql_timer_demo_thread_creat();
#endif

#if CFG_ENABLE_QUECTEL_SPI
   ql_spi_demo_thread_creat();
#endif

#if CFG_ENABLE_QUECTEL_SPI_FLASH
   ql_spi_flash_demo_thread_creat();
#endif

#if CFG_ENABLE_QUECTEL_PWM
   ql_pwm_demo_thread_creat();
#endif

#if CFG_ENABLE_QUECTEL_GPIO
   ql_gpio_demo_thread_creat();
#endif

#if CFG_ENABLE_QUECTEL_FLASH
  ql_flash_demo_thread_creat();
#endif

#if CFG_ENABLE_QUECTEL_ADC
  ql_adc_demo_thread_creat();
#endif

#if CFG_ENABLE_QUECTEL_BLE_PERIPHERA
   extern void ql_ble_demo_periphera_thread_creat();
   ql_ble_demo_periphera_thread_creat();
#endif

#if CFG_ENABLE_QUECTEL_BLE_CENTRAL
   extern void ql_ble_demo_central_thread_creat();
   ql_ble_demo_central_thread_creat();
#endif

#if CFG_ENABLE_QUECTEL_BLE_MULTI_SERVICE
   extern void ql_ble_demo_multi_service_thread_creat();
   ql_ble_demo_multi_service_thread_creat();
#endif

#if CFG_ENABLE_QUECTEL_BLE_PAIR
   extern void ql_ble_demo_pair_thread_creat(void);
   ql_ble_demo_pair_thread_creat();
#endif

#if CFG_ENABLE_QUECTEL_BLE_AMS_ANCS
   extern void ql_ble_demo_ams_ancs_thread_creat(void);
   ql_ble_demo_ams_ancs_thread_creat();
#endif

#if CFG_ENABLE_QUECTEL_OSI
   ql_osi_demo_thread_creat();
#endif

#if CFG_ENABLE_QUECTEL_LOWPOWER
   ql_lowpower_demo_thread_creat();
#endif

#if CFG_ENABLE_QUECTEL_BLE_CONFIG_NETWORK
   ql_ble_demo_config_network_thread_creat();
#endif

#if CFG_ENABLE_QUECTEL_WLAN_CONFIG_NETWORK
   ql_wlan_cfgnet_demo_thread_creat();
#endif

#if CFG_ENABLE_QUECTEL_W5500
    w5500_ethernet_test();
#endif
#if CFG_ENABLE_QUECTEL_AIRKISS
   ql_airkiss_demo_thread_creat();
#endif
#if CFG_ENABLE_QUECTEL_UART_UPGRADE
   ql_uart_upgrade_demo_thread_creat();
#endif
}

extern void  main(void);
void __attribute__((weak)) main(void){ }

void ql_demo_main()
{
   main();
}
#endif
