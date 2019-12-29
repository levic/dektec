#!/bin/bash

#grep _VERSION LinuxSDK/DTAPI/Include/DTAPI.h
egrep '^#define.*_VERSION_' $( find . -name \*.h )
egrep _VERSION_ $( find . -name Version.inc )

function getdefine() {
	local file="$1"
	local define="$2"
	egrep "^#define $2" "$file" | sed -E 's/#define [^ \t]+[ \t]+//'
}

function getassign() {
	local file="$1"
	local var="$2"
	egrep "^${var} = " "$file" | sed -E 's/.* = //'
}

function print_ver() {
	printf '.%s' "$@" | sed 's/[.]//'
}


DTAPI=(
	"$(getdefine ./LinuxSDK/DTAPI/Include/DTAPI.h DTAPI_VERSION_MAJOR)"
	"$(getdefine ./LinuxSDK/DTAPI/Include/DTAPI.h DTAPI_VERSION_MINOR)"
	"$(getdefine ./LinuxSDK/DTAPI/Include/DTAPI.h DTAPI_VERSION_BUGFIX)"
	"$(getdefine ./LinuxSDK/DTAPI/Include/DTAPI.h DTAPI_VERSION_BUILD)"
)

DTA=(
	"$(getassign ./LinuxSDK/Drivers/Dta/Source/Version.inc DTA_VERSION_MAJOR)"
	"$(getassign ./LinuxSDK/Drivers/Dta/Source/Version.inc DTA_VERSION_MINOR)"
	"$(getassign ./LinuxSDK/Drivers/Dta/Source/Version.inc DTA_VERSION_MICRO)"
	"$(getassign ./LinuxSDK/Drivers/Dta/Source/Version.inc DTA_VERSION_BUILD)"
)

DTANW=(
	"$(getassign ./LinuxSDK/Drivers/DtaNw/Source/Version.inc DTANW_VERSION_MAJOR)"
	"$(getassign ./LinuxSDK/Drivers/DtaNw/Source/Version.inc DTANW_VERSION_MINOR)"
	"$(getassign ./LinuxSDK/Drivers/DtaNw/Source/Version.inc DTANW_VERSION_MICRO)"
	"$(getassign ./LinuxSDK/Drivers/DtaNw/Source/Version.inc DTANW_VERSION_BUILD)"
)

DTPCIE=(
	"$(getassign ./LinuxSDK/Drivers/DtPcie/Source/Version.inc DTPCIE_VERSION_MAJOR)"
	"$(getassign ./LinuxSDK/Drivers/DtPcie/Source/Version.inc DTPCIE_VERSION_MINOR)"
	"$(getassign ./LinuxSDK/Drivers/DtPcie/Source/Version.inc DTPCIE_VERSION_MICRO)"
	"$(getassign ./LinuxSDK/Drivers/DtPcie/Source/Version.inc DTPCIE_VERSION_BUILD)"
)

DTU=(
	"$(getassign ./LinuxSDK/Drivers/Dtu/Source/Version.inc DTU_VERSION_MAJOR)"
	"$(getassign ./LinuxSDK/Drivers/Dtu/Source/Version.inc DTU_VERSION_MINOR)"
	"$(getassign ./LinuxSDK/Drivers/Dtu/Source/Version.inc DTU_VERSION_MICRO)"
	"$(getassign ./LinuxSDK/Drivers/Dtu/Source/Version.inc DTU_VERSION_BUILD)"
)

echo "DTAPI $( print_ver "${DTAPI[@]}" )"
echo "Dta $( print_ver "${DTA[@]}" )"
echo "DtaNw $( print_ver "${DTANW[@]}" )"
echo "DtPcie $( print_ver "${DTPCIE[@]}" )"
echo "Dtu $( print_ver "${DTU[@]}" )"

