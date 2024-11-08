
list(APPEND curl_includes
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application/quectel_demo
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
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/include
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/sys_ctrl
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/wdt
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/rc_beken
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/common
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/rw_pub
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/irda
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/intc
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/icu
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/entry
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/macphy_bypass
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/security
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/calendar
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/common
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/include
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/temp_detect
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/misc
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/force_sleep
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/base64
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/user_driver
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/saradc_intf
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/ethernet_intf

    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/rwnx_intf
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/wlan_ui
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/airkiss
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/power_save
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/joint_up
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/bk7011_cal
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/security
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/hostapd_intf
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/easy_flash/inc
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/easy_flash
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/easy_flash/port
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/bk_func/ble_wifi_exchange
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/qadpt/include
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_build
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application
)


file(GLOB_RECURSE curl_lib_sources "${CMAKE_CURRENT_LIST_DIR}/../../ql_components/curl/src/*.c")

file(GLOB_RECURSE curl_vauth_sources "${CMAKE_CURRENT_LIST_DIR}/../../ql_components/curl/src/vauth/*.c")

file(GLOB_RECURSE curl_vquic_sources "${CMAKE_CURRENT_LIST_DIR}/../../ql_components/curl/src/vquic/*.c")

file(GLOB_RECURSE curl_vssh_sources "${CMAKE_CURRENT_LIST_DIR}/../../ql_components/curl/src/vssh/*.c")

file(GLOB_RECURSE curl_vtls_sources "${CMAKE_CURRENT_LIST_DIR}/../../ql_components/curl/src/vtls/*.c")

list(APPEND curl_sources
   ${curl_lib_sources}
   ${curl_vauth_sources}
   ${curl_vquic_sources}
   ${curl_vssh_sources}
   ${curl_vtls_sources}
)

include_directories(BEFORE  ${curl_includes})

add_library(curl STATIC ${curl_sources})

