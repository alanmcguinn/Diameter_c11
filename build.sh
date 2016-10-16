#!/bin/bash

BUILD_DIRS="Diameter
DiameterDictionary
Tests"

ROOT=`pwd`

export CC=/usr/bin/gcc-5
export CXX=/usr/bin/g++-5

clean() {
	for build_dir in ${BUILD_DIRS}; do
		rm -rf ${build_dir}.build
	done
}

build() {
	clean

	for build_dir in ${BUILD_DIRS}; do
		mkdir -p ${build_dir}.build
		cd ${build_dir}.build
		cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MODULE_PATH=${ROOT}/Diameter/Modules ../${build_dir}
		make -j12 clean install
		cd - > /dev/null
	done
}

build
