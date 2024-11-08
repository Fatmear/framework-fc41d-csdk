
list(APPEND mbedtls_includes
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application/quectel_demo
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/curl/include
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/curl/include/curl
    #${CMAKE_CURRENT_LIST_DIR}/../../ql_components/mbedtls/mbedtls/include
    #${CMAKE_CURRENT_LIST_DIR}/../../ql_components/mbedtls/mbedtls/include/mbedtls
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/mbedtls/mbedtls-2.27.0/include
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/mbedtls/mbedtls-2.27.0/include/mbedtls
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
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/app
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/app/config
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/include
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/sys_ctrl
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/wdt

    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/rc_beken
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/rw_pub
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/irda
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/intc
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/icu
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/entry
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/macphy_bypass
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/security
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/common/reg
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/common
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/driver/calendar
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_kernel/common
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_components/qadpt/include
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_build
    ${CMAKE_CURRENT_LIST_DIR}/../../ql_application
)


#file(GLOB_RECURSE mbedtls_lib_sources "${CMAKE_CURRENT_LIST_DIR}/../../ql_components/mbedtls/mbedtls/*.c")
file(GLOB_RECURSE mbedtls_lib_sources "${CMAKE_CURRENT_LIST_DIR}/../../ql_components/mbedtls/mbedtls-2.27.0/library/*.c")

file(GLOB_RECURSE mbedtls_port_sources "${CMAKE_CURRENT_LIST_DIR}/../../ql_components/mbedtls/mbedtls-port/*.c")

file(GLOB_RECURSE mbedtls_ui_sources "${CMAKE_CURRENT_LIST_DIR}/../../ql_components/mbedtls/mbedtls_ui/*.c")

list(APPEND mbedtls_sources
   ${mbedtls_lib_sources}
   ${mbedtls_port_sources}
   ${mbedtls_ui_sources}
)

include_directories(BEFORE  ${mbedtls_includes})

add_library(mbedtls STATIC ${mbedtls_sources})

