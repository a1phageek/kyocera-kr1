# /bin/sh

#
# Shell script to integrate madwifi sources into a Linux
# source tree so it can be built statically.  Typically this
# is done to simplify debugging with tools like kgdb.
#
KERNEL_VERSION=`uname -r`
KERNEL_PATH=${1:-/lib/modules/${KERNEL_VERSION}/build}

DEPTH=../..

MKDIR()
{
	DIR=$1
	test -d $DIR || { echo "Creating $DIR"; mkdir $DIR; }
}

PATCH()
{
	cmp -s $1 $2 || patch -N $1 < $2.patch
}

INSTALL()
{
	DEST=$1; shift
	cp $* $DEST
}

INSTALLX()
{
	DEST=$1; shift
	sed -e 's/^##2.4##//' $1 > $DEST
}

WIRELESS=${KERNEL_PATH}/drivers/net/wireless
ATH=${WIRELESS}/ath
MKDIR ${ATH}
echo "Copy ath driver bits..."
FILES=`ls ${DEPTH}/ath/*.[ch] | sed '/mod.c/d'`
INSTALL ${ATH} ${FILES}
INSTALLX ${ATH}/Makefile ${DEPTH}/ath/Makefile.kernel

ATH_HAL=${WIRELESS}/_ath_hal
MKDIR ${ATH_HAL}
echo "Copy ath_hal bits..."
INSTALLX ${ATH_HAL}/Makefile ${DEPTH}/ath_hal/Makefile.kernel

MKDIR ${WIRELESS}/hal
MKDIR ${WIRELESS}/hal/linux
echo "Copy hal bits..."
INSTALL ${WIRELESS}/hal ${DEPTH}/hal/*
INSTALL ${WIRELESS}/hal/linux ${DEPTH}/hal/linux/*

if [ -f Config.in.wireless-${KERNEL_VERSION}.patch ]; then
	PATCH ${WIRELESS}/Config.in Config.in.wireless-${KERNEL_VERSION}
else
	echo "No patch file for your ${KERNEL_VERSION} kernel."
	echo "Look at Config.in.wireless to see how to change"
	echo "${WIRELESS}/Config.in to suit."
fi
PATCH ${WIRELESS}/Makefile Makefile.wireless

NET=${KERNEL_PATH}/net
NET80211=${NET}/net80211
DOC=${KERNEL_PATH}/Documentation
MKDIR ${NET80211}
echo "Copy net80211 bits..."
FILES=`ls ${DEPTH}/net80211/*.[ch] | sed '/mod.c/d'`
INSTALL ${NET80211} ${FILES}
INSTALLX ${NET80211}/Makefile ${DEPTH}/net80211/Makefile.kernel
MKDIR ${NET80211}/compat
echo "Setting up compatibility bits..."
INSTALL ${NET80211}/compat ${DEPTH}/include/compat.h
MKDIR ${NET80211}/compat/sys
INSTALL ${NET80211}/compat/sys ${DEPTH}/include/sys/*.h

if [ -f Config.in.net-${KERNEL_VERSION}.patch ]; then
	PATCH ${NET}/Config.in Config.in.net-${KERNEL_VERSION}
else
	echo "No patch file for your ${KERNEL_VERSION} kernel."
	echo "Look at Config.in.net to see how to change"
	echo "${NET}/Config.in to suit."
fi
PATCH ${NET}/Makefile Makefile.net
PATCH ${DOC}/Configure.help Configure.help
