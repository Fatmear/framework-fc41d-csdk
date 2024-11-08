
list(APPEND mqtt_includes
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application/quectel_demo/tcp_server
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application/quectel_demo/5500
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application/quectel_demo/5500/w5500
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application/quectel_demo
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application/quectel_demo/tcp_client
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/curl/include
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/curl/include/curl
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/mbedtls/mbedtls/include
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/mbedtls/mbedtls/include/mbedtls
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/mbedtls/mbedtls-port/inc
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/mbedtls/mbedtls_ui
	${CMAKE_CURRENT_LIST_DIR}/../../ql_components/lwip_intf/lwip-2.0.2/arch
	${CMAKE_CURRENT_LIST_DIR}/../../ql_components/lwip_intf/lwip-2.0.2/port
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/lwip_intf/lwip-2.0.2/src/include/lwip
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/lwip_intf/lwip-2.0.2/src/include/posix
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/lwip_intf/lwip-2.0.2/src/include/netif
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/lwip_intf/lwip-2.0.2/src/include
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/lwip_intf/dhcpd
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/wolfssl
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/ip/mac
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/ip/ke
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/ip/umac/src/rxu
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/ip/umac/src/bam
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/ip/umac/src/txu
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/ip/umac/src/llc
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/ip/umac/src/mesh
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/ip/umac/src/mfp
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/ip/umac/src/apm
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/ip/umac/src/sm
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/ip/umac/src/scanu
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/ip/umac/src/me
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/ip/umac/src/rc
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/ip/lmac/src/rwnx
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/ip/lmac/src/bfr
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/ip/lmac/src/vif
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/ip/lmac/src/mm
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/ip/lmac/src/tpc
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/ip/lmac/src/p2p
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/ip/lmac/src/td
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/ip/lmac/src/rx
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/ip/lmac/src/rx/rxl
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/ip/lmac/src/sta
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/ip/lmac/src/ps
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/ip/lmac/src/tx
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/ip/lmac/src/tx/txl
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/ip/lmac/src/rd
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/ip/lmac/src/scan
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/ip/lmac/src/chan
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/ip/lmac/src/hal
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/ip/lmac/src/tdls
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/ip/common
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/os/include
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/os/FreeRTOSv9.0.0
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/os/FreeRTOSv9.0.0/Source/include
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/os/FreeRTOSv9.0.0/Source/portable/Keil/ARM968es
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/app/led
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/app/net_work
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/app/standalone-ap
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/app/tftp
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/app
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/app/ftp
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/app/http
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/app/standalone-station
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/app/config
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/include
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/sys_ctrl
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/wdt
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/codec
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/uart
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/rc_beken
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/spidma
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/general_dma
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/i2c
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/sdcard
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/flash
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/phy
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/saradc
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/i2s
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/sdio
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/jpeg
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/fft
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/rw_pub
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/irda
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/qspi
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/intc
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/icu
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/entry
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/dma
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/gpio
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/macphy_bypass
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/security
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/spi
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/audio
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/pwm
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/common/reg
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/common
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/calendar
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/common
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/include
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/temp_detect
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/misc
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/video_transfer
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/force_sleep
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/base64
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/user_driver
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/saradc_intf
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/ethernet_intf
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/sensor
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/rwnx_intf
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/wlan_ui
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/airkiss
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/power_save
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/joint_up
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/bk7011_cal
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/security
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/camera_intf
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/spidma_intf
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/sim_uart
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/hostapd_intf
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/easy_flash/inc
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/easy_flash
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/easy_flash/port
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/ble_wifi_exchange
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/qadpt/include
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/paho-mqtt/client
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/paho-mqtt/client/src
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/paho-mqtt/mqtt_ui
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/paho-mqtt/mqtt_ui/ssl_mqtt
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/paho-mqtt/mqtt_ui/tcp_mqtt
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/paho-mqtt/packet
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/paho-mqtt/packet/src
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/wpa_supplicant_2_9/src/utils
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/wpa_supplicant_2_9/src/ap
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/wpa_supplicant_2_9/src/common
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/wpa_supplicant_2_9/src/drivers
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/wpa_supplicant_2_9/src/wps
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/wpa_supplicant_2_9/src
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/wpa_supplicant_2_9/bk_patch
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/wpa_supplicant_2_9/hostapd
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_build
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application
)


file(GLOB_RECURSE mqtt_client_sources "${CMAKE_CURRENT_LIST_DIR}/../../ql_components/paho-mqtt/client/src/*.c")

file(GLOB_RECURSE mqtt_ui_sources "${CMAKE_CURRENT_LIST_DIR}/../../ql_components/paho-mqtt/mqtt_ui/*.c")

file(GLOB_RECURSE mqtt_packet_sources "${CMAKE_CURRENT_LIST_DIR}/../../ql_components/paho-mqtt/packet/src/*.c")

list(APPEND mqtt_sources
   ${mqtt_client_sources}
   ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/paho-mqtt/client/paho_mqtt_udp.c
   ${mqtt_packet_sources}
   ${mqtt_ui_sources}
)

include_directories(BEFORE  ${mqtt_includes})

add_library(paho-mqtt STATIC ${mqtt_sources})

