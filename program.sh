#!/bin/bash

# Skrypt do wgrywania programu z ustawieniem fusebitów

echo "test"

is_bootloader=0
while getopts ":bh" o; do
	case "${o}" in
		b)
			is_bootloader=1
	        	;;
		h)
			echo -e "Usage: $0 [-b] flash_file.hex"
			echo -e "Options:"
			echo -e "-b\tEnable bootloader, disable otherwise"
			exit 0
			;;
	esac
	shift $((OPTIND-1))
done

flash_file=$1
if [ -z $flash_file ]; then
	echo -e "\033[0;31m\033[1mERROR: Flash file not specified\033[0m"
	echo -e "See $0 -h for help"
	exit 1
fi
if [ ! -f $flash_file ]; then
	echo -e "\033[0;31m\033[1mERROR: Flash file doesn't exist\033[0m"
	exit 1
fi

echo -e "\033[0;35m\033[1mReading fusebits...\033[0m"
fuses=`avrdude -c usbasp -p m32 -U hfuse:r:-:h -U lfuse:r:-:h`
if [[ $? != 0 ]]; then
	echo -e "\033[0;31m\033[1mERROR: Failed to read fusebit configuration\033[0m"
	exit 1
fi
fuses=`echo $fuses | tr '\n' ' '`
fuses=${fuses%% } # trim space at the end
echo -e "\033[0;34mCurrent fuse bit settings: \033[1m$fuses\033[0m"

expected_fuses="0xc9 0xaf"
previous_fuses="0xc8 0xaf"
if [[ $is_bootloader == 1 ]]; then
	expected_fuses="0xc8 0xaf"
	previous_fuses="0xc9 0xaf"
fi

fuse_params=""
if [[ "$fuses" == "$previous_fuses" ]]; then
	echo -e "\033[0;33m\033[1mWill change fuses\033[0m"
	fusearray=($expected_fuses)
	fuse_params="-U hfuse:w:${fusearray[0]}:m -U lfuse:w:${fusearray[1]}:m"
elif [[ "$fuses" == "$expected_fuses" ]]; then
	echo -e "\033[0;34m\033[1mFuses already configured\033[0m"
else
	echo -e "\033[0;31m\033[1mERROR: Unknown fusebit configuration, please verify manually\033[0m"
	exit 1
fi

echo -e "\033[0;35m\033[1mRunning avrdude...\033[0m"
avrdude -c usbasp -p m32 $fuse_params -U flash:w:$flash_file:i

if [[ $? != 0 ]]; then
	echo -e "\033[0;31m\033[1mERROR: avrdude failed\033[0m"
	exit 1
else
	echo -e "\033[0;32m\033[1mOK\033[0m"
fi
