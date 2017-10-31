#!/bin/sh

#  merge.sh
#  LConnection
#
#
#@注意  chmod 755 merge.sh
#  lipo -info exe 查看库支持的架构
#
#  优先生成真机库,然后再生成模拟器库
#
#  Created by glp on 2017/10/31.
#  Copyright © 2017年 lc. All rights reserved.

echo "${PLATFORM_NAME}"
if [ "${PLATFORM_NAME}" = "iphonesimulator" ]
then

if [ "${ACTION}" = "build" ]
then
INSTALL_DIR=${BUILD_ROOT}/Merge-iphoneos/${PROJECT_NAME}.framework

DEVICE_DIR=${BUILD_ROOT}/${CONFIGURATION}-iphoneos/${PROJECT_NAME}.framework

SIMULATOR_DIR=${BUILD_ROOT}/${CONFIGURATION}-iphonesimulator/${PROJECT_NAME}.framework


if [ -d "${INSTALL_DIR}" ]
then
rm -rf "${INSTALL_DIR}"
fi

mkdir -p "${INSTALL_DIR}"

cp -R "${DEVICE_DIR}/" "${INSTALL_DIR}/"
#ditto "${DEVICE_DIR}/Headers" "${INSTALL_DIR}/Headers"

lipo -create "${DEVICE_DIR}/${PROJECT_NAME}" "${SIMULATOR_DIR}/${PROJECT_NAME}" -output "${INSTALL_DIR}/${PROJECT_NAME}"

#open "${DEVICE_DIR}"
#open "${SRCROOT}/Products"
fi

fi

