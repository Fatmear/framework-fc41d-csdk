#! /bin/bash

export Start_at=`date +%H:%M:%S`
export PROJECT_ROOT=$PWD
export curtitle="cmd:"$0
export ret="0"
export def_proj="FC41D"

doneseg(){
	echo " "
	date +%Y/%m/%d 
	echo "START TIME: $Start_at"
	echo "END TIME: `date +%H:%M:%S`"
	exit $ret
}

helpinfo(){
	echo " "
	echo "Usage: $0 r/new Project Version [debug/release]"
	echo "       $0 clean"
	echo "       $0 h/-h"
	echo "Example:"
	echo "       $0 new $def_proj your_firmware_version"
	echo " "
	echo "Supported projects include but are not limited to:"
	echo "       FC41D/FCM100D/FLM040D/FLM140D/FLM240D/FLM340D/FCM740D"
	echo " "
	echo If you have any question, please contact Quectel.
	echo " "
	echo " "
	exit $ret
}

cleanall(){
	if [ -d "$1" ]; then
		rm -rf $1 > /dev/null
		echo "clean \"$1\" done"
	fi
	if [ -d "$2" ]; then
		rm -rf $2 > /dev/null
		echo "clean \"$2\" done"
	fi
	if [ -d "$3" ]; then
		rm -rf $3 > /dev/null
		echo "clean \"$3\" done"
	fi
}

export build_oper=$1
if test "$build_oper" = ""; then
	oper=new
elif test "$build_oper" = "r"; then
	oper=r
elif test "$build_oper" = "new"; then
	oper=new
elif test "$build_oper" = "n"; then
	oper=new
elif test "$build_oper" = "clean"; then
	oper=clean
elif test "$build_oper" = "c"; then
	oper=clean
elif test "$build_oper" = "h"; then
	oper=h
elif test "$build_oper" = "-h"; then
	oper=h
elif test "$build_oper" = "help"; then
	oper=h
elif test "$build_oper" = "/h"; then
	oper=h
elif test "$build_oper" = "/?"; then
	oper=h
elif test "$build_oper" = "?"; then
	oper=h
else 
	echo " "
	echo "ERR: unknown build operation: $build_oper, should be r/n/new/clean/h/-h"
	echo " "
	export ret=1
	helpinfo	
fi

if test "$oper" = "h"; then
	helpinfo
fi

if test "$oper" = "clean"; then
	cleanall $(pwd)/ql_build/gccout $(pwd)/ql_out
	doneseg
fi

if test "$2" = ""; then
	echo " "
	echo "We need the Project to build the firmware..."
	echo " "
	export ret=1
	helpinfo
fi
export buildproj=$2
#if test "$buildproj" = ""; then
#	buildproj=$def_proj
#fi
buildproj=${buildproj^^}

if test "$3" = ""; then
	echo " "
	echo "We need the Version to build the firmware..."
	echo " "
	export ret=1
	helpinfo
fi
export buildver=$3
buildver=${buildver^^}
export ql_app_ver=${buildver}_APP

export ql_gcc_name=gcc-arm-none-eabi-5_4-2016q3
if [ ! -d ./ql_tools/gcc-arm-none-eabi-5_4-2016q3 ];then 
   tar -xvf ./ql_tools/gcc-arm-none-eabi-5_4-2016q3.tar.bz2 -C ./ql_tools
fi

if [ ! -d ./ql_tools/cmake-3.25.1-linux-x86_64 ];then 
   tar -xf ./ql_tools/cmake-3.25.1-linux-x86_64.tar.gz -C ./ql_tools
fi

export PATH=$(pwd)/ql_tools/gcc-arm-none-eabi-5_4-2016q3/bin/:$(pwd)/ql_tools/cmake-3.25.1-linux-x86_64/bin/:$PATH
echo $PATH

if test "$oper" = "new"; then
	cleanall $(pwd)/ql_build/gccout $(pwd)/ql_out
fi

export KCONFIG_CONFIG=$(pwd)/ql_build/.config
export KCONFIG_AUTOHEADER=$(pwd)/ql_build/config.h

if [ ! -e ${KCONFIG_CONFIG} ];
then 
    cp $(pwd)/ql_build/defconfig ${KCONFIG_CONFIG}
fi

if [ "${if_ubuntu20_env}" != "" ]; # for Ubuntu20 and Ubuntu22
then
   $(pwd)/ql_tools/kconfig_tool/menuconfig_ubuntu20_22 ./Kconfig 
else  # for Ubuntu16 and Ubuntu18
   $(pwd)/ql_tools/kconfig_tool/menuconfig_ubuntu16_18 ./Kconfig
fi

$(pwd)/ql_tools/kconfig_tool/genconfig --config-out=${KCONFIG_CONFIG} --header-path=${KCONFIG_AUTOHEADER}

cd ql_build

if [ -f build_version.c ];then 
   rm build_version.c
fi

#ocpu_sdk_rev=$(<ocpu_sdk_rev.txt)
#echo -e "\n\nconst char ocpu_sdk_version[64] =\""$ocpu_sdk_rev"\";\n" >  build_version.c
build_time=$(date "+%H:%M:%S %Y-%m-%d")
echo -e "\n\nconst char usr_build_date[64] =\""$build_time"\";\n\n" >>  build_version.c

if [ ! -d "gccout" ]; then
    mkdir gccout
else
    rm -rf gccout
    mkdir gccout
fi

echo "Enter gccout dir ......"
cd gccout

cmake ../..

make

# according to the build return to see success or not
if [ $? != 0 ]; then
	echo ""
	echo "xxxxxxxxxxxxxxxxxx   ERROR   xxxxxxxxxxxxxxxxxxxxxxx"
	echo "            Firmware building is ERROR!             "
	echo "xxxxxxxxxxxxxxxxxx   ERROR   xxxxxxxxxxxxxxxxxxxxxxx"
	echo ""
	export ret=1
	cd ../..
	doneseg
fi

cd ../..

if [ ! -d "ql_out" ]; then
    mkdir ql_out
fi

cp $(pwd)/ql_build/gccout/$buildproj.map $(pwd)/ql_out/$buildproj.map
cp $(pwd)/ql_build/gccout/$buildproj.bin $(pwd)/ql_out/$buildproj.bin
cp $(pwd)/ql_build/gccout/$buildproj.elf $(pwd)/ql_out/$buildproj.elf

python3 ql_tools/codesize.py --map $(pwd)/ql_out/$buildproj.map --outobj ql_out/outobj.csv --outlib ql_out/outlib.csv  --outsect ql_out/outsect.csv

cd ql_tools/beken_packager
python ocpu_bk_packager.py $buildproj
if [ $? != 0 ]; then
	echo ""
	echo "xxxxxxxxxxxxxxxxxx   ERROR   xxxxxxxxxxxxxxxxxxxxxxx"
	echo "       Firmware package generation is ERROR!        "
	echo "xxxxxxxxxxxxxxxxxx   ERROR   xxxxxxxxxxxxxxxxxxxxxxx"
	echo ""
	export ret=1
	cd ../..
	doneseg
fi

echo ""
echo "********************        PASS         ***********************"
echo "              Firmware package is ready for you.                "
echo "********************        PASS         ***********************"
echo ""

cd ../..

doneseg

