#!/sbin/sh
#
# Fingerprint fix in 19743

mount /dev/block/bootdevice/by-name/vendor /vendor

if [ "$(cat /proc/fp_id)" == "E_520" ]; then
    rm /vendor/bin/hw/android.hardware.biometrics.fingerprint@2.1-service.realme_trinket
    rm /vendor/bin/hw/vendor.oppo.hardware.biometrics.fingerprint@2.1-service
    mv /vendor/odm/bin/hw/vendor.oppo.hardware.biometrics.fingerprint@2.1-service /vendor/bin/hw/vendor.oppo.hardware.biometrics.fingerprint@2.1-service
    mv /vendor/odm/bin/hw/android.hardware.biometrics.fingerprint@2.1-service.realme_trinket /vendor/bin/hw/android.hardware.biometrics.fingerprint@2.1-service.realme_trinket
    chmod +x /vendor/bin/hw/vendor.oppo.hardware.biometrics.fingerprint@2.1-service /vendor/bin/hw/android.hardware.biometrics.fingerprint@2.1-service.realme_trinket
fi
rm -rf /vendor/odm/bin
umount /vendor
