:: Copyright (C) 2023 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
:: Quectel Wireless Solution Proprietary and Confidential.
::

@echo off

set Start_at=%time%
set curtitle=cmd: %~n0 %*
title %curtitle% @ %time%
set ret=0
set def_proj=FC41D

set build_oper=%1
if "%build_oper%"=="" (
	set oper=new
) else if /i "%build_oper%"=="r" (
	set oper=r
) else if /i "%build_oper%"=="new" (
	set oper=new
) else if /i "%build_oper%"=="n" (
	set oper=new
) else if /i "%build_oper%"=="clean" (
	set oper=clean
) else if /i "%build_oper%"=="c" (
	set oper=clean
) else if /i "%build_oper%"=="h" (
	set oper=h
) else if /i "%build_oper%"=="-h" (
	set oper=h
) else if /i "%build_oper%"=="help" (
	set oper=h
) else if /i "%build_oper%"=="/h" (
	set oper=h
) else if /i "%build_oper%"=="/?" (
	set oper=h
) else if /i "%build_oper%"=="?" (
	set oper=h
) else (
	echo=
	echo ERR: unknown build operation: %build_oper%, should be r/n/new/clean/h/-h
	echo=
	set ret=1
	goto helpinfo
)
echo=

if /i "%oper%"=="h" (
	goto helpinfo
)

if /i "%oper%"=="clean" (
	call :cleanall %CD%\ql_build\gccout %CD%\ql_out
	goto doneseg
)

if /i "%2" == "" (
	echo=
	echo We need the Project to build the firmware...
	echo=
	set ret=1
	goto helpinfo
)
set buildproj=%2
::if "%buildproj%"=="" (
::	set buildproj=%def_proj%
::)
for %%i in (A B C D E F G H I J K L M N O P Q R S T U V W X Y Z) do call set buildproj=%%buildproj:%%i=%%i%%


if /i "%3" == "" (
	echo=
	echo We need the Version to build the firmware...
	echo=
	set ret=1
	goto helpinfo
)
set buildver=%3
for %%i in (A B C D E F G H I J K L M N O P Q R S T U V W X Y Z) do call set buildver=%%buildver:%%i=%%i%%
set ql_app_ver=%buildver%_APP

set ql_tool_dir=%CD%\ql_tools
set ql_gcc_name=gcc-arm-none-eabi-5_4-2016q3-20160926-win32
set cmd_7z=%ql_tool_dir%\7z\7z.exe

if not exist %ql_tool_dir%/%ql_gcc_name% ( 
	%cmd_7z% x -y %ql_tool_dir%/%ql_gcc_name%.zip -o%ql_tool_dir%/%ql_gcc_name% || ( echo ------------unzip gcc failed------------- & set ret=1 & goto exit)
	echo unzip gcc done.
)

set ql_cmake_name=cmake-3.28.0-rc3-windows-x86_64
if not exist %ql_tool_dir%/%ql_cmake_name% ( 
	%cmd_7z% x -y %ql_tool_dir%/%ql_cmake_name%.zip -o%ql_tool_dir% || ( echo ------------unzip cmake failed------------- & set ret=1 & goto exit)
	echo unzip cmake done.
)

set ql_mingw_name=winlibs-x86_64-posix-seh-gcc-13.2.0-mingw-w64msvcrt-11.0.1-r2
if not exist %ql_tool_dir%/mingw64 ( 
	%cmd_7z% x -y %ql_tool_dir%/%ql_mingw_name%.7z -o%ql_tool_dir% || ( echo ------------unzip mingw failed------------- & set ret=1 & goto exit)
	echo unzip mingw done.
)

::call :add_path %ql_tool_dir%/%ql_gcc_name%/bin/
call :add_path %ql_tool_dir%/%ql_cmake_name%/bin/
call :add_path %ql_tool_dir%/mingw64/bin/
echo=
echo %PATH%

if /i "%oper%"=="new" (
	call :cleanall %CD%\ql_build\gccout %CD%\ql_out
)

set KCONFIG_CONFIG=%CD%\ql_build\.config
set KCONFIG_AUTOHEADER=%CD%/ql_build/config.h

if not exist %KCONFIG_CONFIG% (
    copy /y %CD%\ql_build\defconfig %KCONFIG_CONFIG%
)

::%ql_tool_dir%/kconfig_tool/menuconfig ./Kconfig
%CD%/ql_tools/kconfig_tool/genconfig --config-out=%KCONFIG_CONFIG% --header-path=%KCONFIG_AUTOHEADER%

cd ql_build

del /f /Q build_version.c

::ocpu_sdk_rev=$(<ocpu_sdk_rev.txt)
::echo -e "\n\nconst char ocpu_sdk_version[64] =\""$ocpu_sdk_rev"\";\n" >  build_version.c
set build_time=%date% %time%
echo /*const char usr_build_date[64] ="%build_time%";*/>  build_version.c

if not exist "gccout" (mkdir gccout)

echo "Enter gccout dir ......"
cd gccout

::cmake ../.. -G "MinGW Makefiles"
::mingw32-make

cmake ../.. -G Ninja
ninja

:: according to the build return to see success or not
if not %errorlevel% == 0 (
	echo.
	echo xxxxxxxxxxxxxxxxxx   ERROR   xxxxxxxxxxxxxxxxxxxxxxx
	echo             Firmware building is ERROR!
	echo xxxxxxxxxxxxxxxxxx   ERROR   xxxxxxxxxxxxxxxxxxxxxxx
	echo.
	set ret=1
	cd ../..
	goto doneseg
)

cd ../..

if not exist "ql_out" (
    mkdir ql_out
)
xcopy /y "%cd%\ql_build\gccout\%buildproj%.map" "%cd%\ql_out\"
xcopy /y "%cd%\ql_build\gccout\%buildproj%.bin" "%cd%\ql_out\"
xcopy /y "%cd%\ql_build\gccout\%buildproj%.elf" "%cd%\ql_out\"

call pythonw.exe ql_tools\codesize.py --map %cd%\ql_out\\%buildproj%.map --outobj ql_out\outobj.csv --outlib ql_out\outlib.csv  --outsect ql_out\outsect.csv

cd ql_tools/beken_packager
call python ocpu_bk_packager.py %buildproj%
if not %errorlevel% == 0 (
	echo.
	echo xxxxxxxxxxxxxxxxxx   ERROR   xxxxxxxxxxxxxxxxxxxxxxx
	echo         Firmware package generation is ERROR!
	echo xxxxxxxxxxxxxxxxxx   ERROR   xxxxxxxxxxxxxxxxxxxxxxx
	echo.
	set ret=1
	cd ../..
	goto doneseg
)

echo.
echo ********************        PASS         ***********************
echo               Firmware package is ready for you.
echo ********************        PASS         ***********************
echo.

cd ../..

:doneseg
echo=
echo %date%
echo START TIME:  %Start_at%
echo END TIME:    %time%
echo=
goto exit

:helpinfo
echo=
echo.Usage: %~n0 r/new/n Project Version [debug/release]
echo.       %~n0 clean
echo.       %~n0 h/-h
echo.Example:
echo.       %~n0 new %def_proj% your_firmware_version
echo=
echo.Supported projects include but are not limited to:
echo        FC41D/FCM100D/FLM040D/FLM140D/FLM240D/FLM340D/FCM740D
echo=
echo.If you have any question, please contact Quectel.
echo=
echo=

:exit
exit /B %ret%

:add_path
(echo ";%PATH%;" | find /C /I ";%1;" > nul) || set "PATH=%1;%PATH%"
goto :eof

:cleanall
if exist "%1" (
	del /s/q/f %1 > nul
	rd /s/q %1 > nul
	echo clean "%1" done
)
if exist "%2" (
	del /s/q/f %2 > nul
	rd /s/q %2 > nul
	echo clean "%2" done
)
if exist "%3" (
	del /s/q/f %3 > nul
	rd /s/q %3 > nul
	echo clean "%3" done
)

echo=
goto :eof
