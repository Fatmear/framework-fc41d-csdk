
SET(CMAKE_SYSTEM_NAME Generic)
cmake_minimum_required(VERSION 3.18)

if(WIN32)
    message(STATUS "windows build .........")
elseif(UNIX)
    message(STATUS "Linux build ...........")
endif()

set(CMAKE_C_COMPILER_FORCED TRUE)
set(CMAKE_CXX_COMPILER_FORCED TRUE)
set(ql_proj $ENV{buildproj})
include(${CMAKE_CURRENT_SOURCE_DIR}/quec_bk_7231n.cmake)
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR}/ql_build/gccout)
set(target ${ql_proj})
set(ld_file "${CMAKE_CURRENT_LIST_DIR}/ql_build/bk7231n_bsp.lds")
set(macro_def "
    -DCFG_OS_FREERTOS=1
")

set(CMAKE_C_LINK_FLAGS "-g -Wl,--gc-sections -marm -mcpu=arm968e-s -mthumb-interwork -nostdlib  -Xlinker -Map=${TARGET}.map -Wl,-wrap,malloc -Wl,-wrap,_malloc_r -Wl,-wrap,free -Wl,-wrap,_free_r -Wl,-wrap,zalloc -Wl,-wrap,calloc -Wl,-wrap,realloc  -Wl,-wrap,_realloc_r -Wl,-wrap,printf -Wl,-wrap,vsnprintf -Wl,-wrap,snprintf -Wl,-wrap,sprintf -Wl,-wrap,puts -Wl,-wrap,mbedtls_ssl_handshake_server_step -Wl,-wrap,mbedtls_ssl_handshake_client_step -Wl,-wrap,mbedtls_ssl_write_client_hello -Wl,-wrap,mbedtls_ssl_read -Wl,-wrap,mbedtls_ssl_write -Wl,-wrap,mbedtls_ssl_free -Wl,-wrap,mbedtls_ssl_session_reset -Wl,-wrap,mbedtls_ssl_setup -Wl,-wrap,mbedtls_ssl_send_alert_message -Wl,-wrap,mbedtls_ssl_close_notify")
set(CMAKE_C_LINK_EXECUTABLE "<CMAKE_LINKER> <CMAKE_C_LINK_FLAGS> -o <TARGET> <OBJECTS> <LINK_LIBRARIES> <LINK_FLAGS>")
set(ASM_OPTIONS "-g -marm -mthumb-interwork -mcpu=arm968e-s -march=armv5te -x assembler-with-cpp")
set(CMAKE_C_FLAGS "-g -mthumb -mcpu=arm968e-s -march=armv5te -mthumb-interwork -mlittle-endian -Os -std=c99 -ffunction-sections -Wall -Wno-implicit-function-declaration  -Wno-format -Wno-unknown-pragmas -fsigned-char -fdata-sections -nostdlib -fno-strict-aliasing")
set(CMAKE_CXX_FLAGS "-g -mthumb -mcpu=arm968e-s -march=armv5te -mthumb-interwork -mlittle-endian -Os -std=c99 -ffunction-sections -Wall -Wno-implicit-function-declaration  -Wno-format -Wno-unknown-pragmas -fsigned-char -fdata-sections -nostdlib -fno-strict-aliasing")

project(${ql_proj} VERSION 1.0)
enable_language(ASM C)
add_definitions(${macro_def})
add_link_options(-T${ld_file} -Xlinker -Map=${target}.map)


list(APPEND include_files
)


list(APPEND source_files
    ${CMAKE_CURRENT_LIST_DIR}/ql_build/build_version.c
)

include_directories(${include_files})
add_definitions(-DSYS_CONFIG_FILE="sys_config.h")

add_definitions(-DMBEDTLS_CONFIG_FILE="tls_config.h")

link_directories(BEFORE ${CMAKE_CURRENT_LIST_DIR}/ql_build/prebuilds)
add_executable(${target}
    ${source_files}
)

add_subdirectory(ql_application)

add_subdirectory(ql_components)

target_link_libraries(${target}
                 PUBLIC
                 -Wl,--start-group
                 driver
                 airkiss
                 bk_app
                 sensor
                 uart_debug
                 supplicant
                 rf_test
                 func
                 os
                 rf_use
                 rwnx
                 rtos
                 ble
                 quectel_api
                 cal
                 curl
                 paho-mqtt
                 mbedtls
		 lwip
                 ql_app
                 -Wl,--end-group
)

set_target_properties(${target} PROPERTIES OUTPUT_NAME "${target}.elf")

add_custom_command(
    TARGET ${target}
    POST_BUILD
    COMMAND ${arm_gcc_objcopy} -O binary ${CMAKE_CURRENT_BINARY_DIR}/${target}.elf ${CMAKE_CURRENT_BINARY_DIR}/${target}.bin
)

