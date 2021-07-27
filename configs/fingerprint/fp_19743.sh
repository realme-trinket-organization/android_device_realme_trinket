#!/sbin/sh
#
# Fingerprint fix in 19743

mount /dev/block/bootdevice/by-name/vendor /vendor

if [ "$(cat /proc/fp_id)" == "E_520" ]; then
    rm /vendor/bin/hw/android.hardware.biometrics.fingerprint@2.1-service.realme_trinket
    rm /vendor/etc/init/android.hardware.biometrics.fingerprint@2.1-service.realme_trinket.rc
    mv /vendor/odm/bin/hw/vendor.oppo.hardware.biometrics.fingerprint@2.1-service /vendor/bin/hw/vendor.oppo.hardware.biometrics.fingerprint@2.1-service
    mv /vendor/odm/bin/hw/android.hardware.biometrics.fingerprint@2.1-service.realme_trinket /vendor/bin/hw/android.hardware.biometrics.fingerprint@2.1-service.realme_trinket
    mv /vendor/odm/etc/init/vendor.oppo.hardware.biometrics.fingerprint@2.1-service.rc /vendor/etc/init/vendor.oppo.hardware.biometrics.fingerprint@2.1-service.rc
    mv /vendor/odm/etc/init/android.hardware.biometrics.fingerprint@2.1-service.realme_trinket.rc /vendor/etc/init/android.hardware.biometrics.fingerprint@2.1-service.realme_trinket.rc
    mv /vendor/odm/etc/permissions/com.fingerprints.extension.xml /vendor/etc/permissions/com.fingerprints.extension.xml
    mv /vendor/odm/lib/vendor.oppo.hardware.commondcs@1.0.so /vendor/lib/vendor.oppo.hardware.commondcs@1.0.so
    mv /vendor/odm/lib/vendor.qti.hardware.fingerprint@1.0.so /vendor/lib/vendor.qti.hardware.fingerprint@1.0.so
    mv /vendor/odm/lib64/android.frameworks.fingerprintservice@1.0.so /vendor/lib64/android.frameworks.fingerprintservice@1.0.so
    mv /vendor/odm/lib64/libRbsFlow.so /vendor/lib64/libRbsFlow.so
    mv /vendor/odm/lib64/vendor.egistec.hardware.fingerprint@4.0.so /vendor/lib64/vendor.egistec.hardware.fingerprint@4.0.so
    mv /vendor/odm/lib64/vendor.oppo.hardware.biometrics.fingerprint@2.1.so /vendor/lib64/vendor.oppo.hardware.biometrics.fingerprint@2.1.so
    mv /vendor/odm/lib64/vendor.oppo.hardware.commondcs@1.0.so /vendor/lib64/vendor.oppo.hardware.commondcs@1.0.so
    mv /vendor/odm/lib64/vendor.qti.hardware.fingerprint@1.0.so /vendor/lib64/vendor.qti.hardware.fingerprint@1.0.so
    mv /vendor/odm/lib64/hw/libets_teeclient_v2.so /vendor/lib64/hw/libets_teeclient_v2.so
fi
rm -rf /vendor/odm/bin /vendor/odm/etc/init /vendor/odm/etc/permissions/com.fingerprints.extension.xml /vendor/odm/lib /vendor/odm/lib64

umount /vendor
