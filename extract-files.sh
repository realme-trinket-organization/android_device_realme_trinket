#!/bin/bash
#
# Copyright (C) 2016 The CyanogenMod Project
# Copyright (C) 2017-2020 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#

set -e

DEVICE=realme_trinket
VENDOR=realme

# Load extract_utils and do some sanity checks
MY_DIR="${BASH_SOURCE%/*}"
if [[ ! -d "${MY_DIR}" ]]; then MY_DIR="${PWD}"; fi

ANDROID_ROOT="${MY_DIR}/../../.."

HELPER="${ANDROID_ROOT}/tools/extract-utils/extract_utils.sh"
if [ ! -f "${HELPER}" ]; then
    echo "Unable to find helper script at ${HELPER}"
    exit 1
fi
source "${HELPER}"

# Default to sanitizing the vendor folder before extraction
CLEAN_VENDOR=true

KANG=
SECTION=

while [ "${#}" -gt 0 ]; do
    case "${1}" in
        -n | --no-cleanup )
                CLEAN_VENDOR=false
                ;;
        -k | --kang )
                KANG="--kang"
                ;;
        -s | --section )
                SECTION="${2}"; shift
                CLEAN_VENDOR=false
                ;;
        * )
                SRC="${1}"
                ;;
    esac
    shift
done

if [ -z "${SRC}" ]; then
    SRC="adb"
fi

function blob_fixup() {
    case "${1}" in

    # Fix xml version
    product/etc/permissions/vendor.qti.hardware.data.connection-V1.0-java.xml)
        sed -i 's/xml version="2.0"/xml version="1.0"/' "${2}"
        ;;
    product/etc/permissions/vendor.qti.hardware.data.connection-V1.1-java.xml)
        sed -i 's/xml version="2.0"/xml version="1.0"/' "${2}"
        ;;

    # Rename Fingerprint Blobs For RMX2030
    odm/bin/hw/android.hardware.fingerprint@2.1-service.realme_trinket)
        "${PATCHELF}" --set-soname "android.hardware.biometrics.fingerprint@2.1-service.realme_trinket" "${2}"
        ;;
    odm/etc/init/android.hardware.biometrics.fingerprint@2.1-service.realme_trinket.rc)
        sed -i 's/ets_hal/egis_hal/' "${2}"
        sed -i 's/-rbs/.realme_trinket/' "${2}"
        ;;

    esac
}

    # Reinitialize the helper for device
    setup_vendor "${DEVICE}" "${VENDOR}" "${ANDROID_ROOT}" false "${CLEAN_VENDOR}"

    extract "${MY_DIR}/../${DEVICE}/proprietary-files.txt" "${SRC}" "${KANG}" --section "${SECTION}"

"${MY_DIR}/setup-makefiles.sh"
