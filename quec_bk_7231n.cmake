## 系统环境检查 ##
if (USE_SYS_TOOLCHAIN)
find_program(arm_gcc_file_path arm-none-eabi-gcc)
if(NOT IS_ABSOLUTE ${arm_gcc_file_path})
    message(FATAL_ERROR "arm-none-eabi-gcc is not found !!!!")
endif()

if(WIN32)
    STRING(REGEX REPLACE "^(.+)/arm-none-eabi-gcc.exe*$" "\\1" arm_gcc_path ${arm_gcc_file_path})
elseif(UNIX)
    STRING(REGEX REPLACE "^(.+)/arm-none-eabi-gcc*$" "\\1" arm_gcc_path ${arm_gcc_file_path})
endif()
set(tools ${arm_gcc_path}/..)
else()
set(tools ${CMAKE_CURRENT_LIST_DIR}/ql_tools/$ENV{ql_gcc_name}/bin)
endif()

STRING(REGEX REPLACE "\\\\" "/" tools "${tools}")

set(CMAKE_C_COMPILER_FORCED TRUE)
set(CMAKE_CXX_COMPILER_FORCED TRUE)
SET(TARGET_TRIPLET "arm-none-eabi-")


if(WIN32)
    set(arm_gcc_ar ${tools}/${TARGET_TRIPLET}ar.exe)
    set(arm_gcc_as ${tools}/${TARGET_TRIPLET}as.exe)
    set(arm_gcc_nm ${tools}/${TARGET_TRIPLET}nm.exe)
    set(arm_gcc_size ${tools}/${TARGET_TRIPLET}size.exe)
    set(arm_gcc_objcopy ${tools}/${TARGET_TRIPLET}objcopy.exe)
    set(arm_gcc_objdump ${tools}/${TARGET_TRIPLET}objdump.exe)
elseif(UNIX)
    set(arm_gcc_ar ${tools}/${TARGET_TRIPLET}ar)
    set(arm_gcc_as ${tools}/${TARGET_TRIPLET}as)
    set(arm_gcc_nm ${tools}/${TARGET_TRIPLET}nm)
    set(arm_gcc_size ${tools}/${TARGET_TRIPLET}size)
    set(arm_gcc_objcopy ${tools}/${TARGET_TRIPLET}objcopy)
    set(arm_gcc_objdump ${tools}/${TARGET_TRIPLET}objdump)
endif()

if (WIN32)
    # 设置gcc工具
    set(CMAKE_C_COMPILER ${tools}/${TARGET_TRIPLET}gcc.exe)
    set(CMAKE_ASM_COMPILER ${tools}/${TARGET_TRIPLET}gcc.exe)
    # 设置g++工具
    set(CMAKE_CXX_COMPILER ${tools}/${TARGET_TRIPLET}g++.exe)
	SET(CMAKE_OBJCOPY ${tools}/${TARGET_TRIPLET}objcopy CACHE INTERNAL "objcopy tool")
	SET(CMAKE_OBJDUMP ${tools}/${TARGET_TRIPLET}objdump CACHE INTERNAL "objdump tool")
    # 设置ld工具
    set(CMAKE_LINKER ${tools}/${TARGET_TRIPLET}gcc.exe)
elseif (UNIX)
    # 设置gcc工具
    set(CMAKE_C_COMPILER ${tools}/${TARGET_TRIPLET}gcc)
    # 设置g++工具
    set(CMAKE_CXX_COMPILER ${tools}/${TARGET_TRIPLET}g++)
    # 设置ld工具
    set(CMAKE_LINKER ${tools}/${TARGET_TRIPLET}gcc)
endif()

#指定交叉编译环境的目录
set(CMAKE_FIND_ROOT_PATH ${tools})
#从来不在指定目录(交叉编译)下查找工具程序。(编译时利用的是宿主的工具)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
#只在指定目录(交叉编译)下查找库文件
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
#只在指定目录(交叉编译)下查找头文件
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
#只在指定的目录(交叉编译)下查找依赖包
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

