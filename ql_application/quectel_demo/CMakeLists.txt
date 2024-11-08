
list(APPEND ql_demo_include_files
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application/quectel_demo
)


list(APPEND ql_demo_source_files
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application/quectel_demo/ql_lowpower_demo.c
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application/quectel_demo/ql_flash_demo.c
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application/quectel_demo/ql_uart_ymodem_upgrade_demo.c
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application/quectel_demo/tcp_server/mongoose.c
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application/quectel_demo/tcp_server/tcp_websocket_server.c
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application/quectel_demo/ql_ble_config_network_demo.c
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application/quectel_demo/ql_mqtt_demo.c
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application/quectel_demo/ql_ble_demo.c
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application/quectel_demo/ql_ble_multi_service_demo.c
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application/quectel_demo/ql_watchdog_demo.c
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application/quectel_demo/ql_wlan_config_network_demo.c
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application/quectel_demo/ql_adc_demo.c
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application/quectel_demo/5500/wizchip_test.c
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application/quectel_demo/5500/wizchip_conf.c
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application/quectel_demo/5500/w5500/w5500.c
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application/quectel_demo/5500/wizchip_port.c
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application/quectel_demo/5500/wizchip_ethnetif.c
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application/quectel_demo/ql_ftp_demo_c.c
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application/quectel_demo/ql_uart_demo.c
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application/quectel_demo/ql_timer_demo.c
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application/quectel_demo/ql_osi_demo.c
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application/quectel_demo/ql_wlan_demo.c
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application/quectel_demo/ql_i2c_eeprom_demo.c
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application/quectel_demo/ql_tls_demo.c
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application/quectel_demo/ql_gpio_demo.c
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application/quectel_demo/tcp_client/tcp_client_demo.c
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application/quectel_demo/ql_spi_flash_demo.c
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application/quectel_demo/ql_pwm_demo.c
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application/quectel_demo/ql_spi_demo.c
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application/quectel_demo/ql_http_demo.c
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application/quectel_demo/ql_ble_ams_ancs_demo.c
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application/quectel_demo/ql_ble_pair_demo.c
)

include_directories(BEFORE  ${ql_demo_include_files})
add_library(ql_demo STATIC ${ql_demo_source_files})
