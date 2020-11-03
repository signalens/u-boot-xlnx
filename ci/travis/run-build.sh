#!/bin/bash
set -e

# cd to docker build dir if it exists
if [ -d /docker_build_dir ] ; then
	cd /docker_build_dir
fi

. ./ci/travis/lib.sh

if [ -f "${FULL_BUILD_DIR}/env" ] ; then
	echo_blue "Loading environment variables"
	cat "${FULL_BUILD_DIR}/env"
	. "${FULL_BUILD_DIR}/env"
fi

if [ -z "$NUM_JOBS" ] ; then
	NUM_JOBS=$(getconf _NPROCESSORS_ONLN)
	NUM_JOBS=${NUM_JOBS:-1}
fi

APT_LIST="make gcc libssl-dev"

if [ "$ARCH" = "arm64" ] ; then
	if [ -z "$CROSS_COMPILE" ] ; then
		CROSS_COMPILE=aarch64-linux-gnu-
		export CROSS_COMPILE
	fi

	APT_LIST="$APT_LIST gcc-aarch64-linux-gnu"
fi

if [ "$ARCH" = "arm" ] ; then
	if [ -z "$CROSS_COMPILE" ] ; then
		CROSS_COMPILE=arm-linux-gnueabihf-
		export CROSS_COMPILE
	fi

	APT_LIST="$APT_LIST gcc-arm-linux-gnueabihf"
fi

apt_install() {
	sudo apt-get install -y $@
}

apt_update_install() {
	apt_install $@
}

build_default() {
	[ -n "$DEFCONFIG" ] || {
		echo_red "No DEFCONFIG provided"
		return 1
	}

	[ -n "$ARCH" ] || {
		echo_red "No ARCH provided"
		return 1
	}

	apt_update_install $APT_LIST
	make ARCH="${ARCH}" "${DEFCONFIG}"
	make CROSS_COMPILE="${CROSS_COMPILE}" ARCH="${ARCH}" "-j$NUM_JOBS"
}

ORIGIN=${ORIGIN:-origin}

BUILD_TYPE=${BUILD_TYPE:-${1}}
BUILD_TYPE=${BUILD_TYPE:-default}

build_${BUILD_TYPE}
