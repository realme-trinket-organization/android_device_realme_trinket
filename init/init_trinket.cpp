/*
   Copyright (c) 2016, The Linux Foundation. All rights reserved.
   Copyright (C) 2016, The CyanogenMod Project
   Copyright (C) 2017-2021, The LineageOS Project

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <cstdlib>
#include <fstream>
#include <string>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>
#include <sys/sysinfo.h>

#include <android-base/properties.h>
#include <android-base/strings.h>

#include "vendor_init.h"

// copied from build/tools/releasetools/ota_from_target_files.py
// but with "." at the end and empty entry
std::vector<std::string> ro_product_props_default_source_order = {
    "",
    "product.",
    "product_services.",
    "odm.",
    "vendor.",
    "system_ext.",
    "system.",
};

void property_override(char const prop[], char const value[], bool add = true)
{
    auto pi = (prop_info *) __system_property_find(prop);

    if (pi != nullptr) {
        __system_property_update(pi, value, strlen(value));
    } else if (add) {
        __system_property_add(prop, strlen(prop), value, strlen(value));
    }
}

int is2GB()
{
    struct sysinfo sys;
    sysinfo(&sys);
    return sys.totalram > 1024ull * 1024 * 1024;
}

void vendor_load_properties()
{
    std::ifstream infile("/proc/oppoVersion/prjVersion");
    std::string prjVersion;
    getline(infile, prjVersion);

    std::ifstream fin;
    std::string buf;
    fin.open("/proc/cmdline");
    while (std::getline(fin, buf, ' '))
        if (buf.find("board_id") != std::string::npos)
            break;
    fin.close();

    const auto set_ro_build_prop = [](const std::string &source,
                                      const std::string &prop, const std::string &value) {
        auto prop_name = "ro." + source + "build." + prop;
        property_override(prop_name.c_str(), value.c_str(), false);
    };

    const auto set_ro_product_prop = [](const std::string &source,
            const std::string &prop, const std::string &value) {
        auto prop_name = "ro.product." + source + prop;
        property_override(prop_name.c_str(), value.c_str(), false);
    };

    property_override("ro.build.description", "redfin-user 11 RQ3A.210805.001.A1 7474174 release-keys");
    for (const auto &source : ro_product_props_default_source_order) {
        set_ro_build_prop(source, "fingerprint", "google/redfin/redfin:11/RQ3A.210805.001.A1/7474174:user/release-keys");
    }

    /* RMX2030 */
    if (prjVersion == "19473") {
        property_override("ro.build.product", "RMX2030");
        for (const auto &source : ro_product_props_default_source_order) {
            set_ro_product_prop(source, "device", "RMX2030");
            set_ro_product_prop(source, "model", "RMX2030");
            set_ro_product_prop(source, "name", "RMX2030");
        }
        property_override("persist.vendor.fingerprint.fp_id", "E_520");
    }
    /* RMX1927 */
    else if (prjVersion == "19632") {
        property_override("ro.build.product", "RMX1927");
        for (const auto &source : ro_product_props_default_source_order) {
            set_ro_product_prop(source, "device", "RMX1927");
            set_ro_product_prop(source, "model", "RMX1927");
            set_ro_product_prop(source, "name", "RMX1927");
        }
        property_override("persist.sys.nfc.disPowerSave", "1");
        property_override("persist.sys.nfc.default", "on");
        property_override("persist.sys.nfc.aid_overflow", "true");
        property_override("ro.product.cuptsm", "OPPO|ESE|01|02");
        property_override("persist.sys.nfc.antenna_area", "bottom");
        // Set hardware SKU prop
        property_override("ro.boot.product.hardware.sku", "19632");
    } else {
        if (buf.find("S86125AA1") != std::string::npos) {
            property_override("ro.build.product", "RMX1925");
            for (const auto &source : ro_product_props_default_source_order) {
                set_ro_product_prop(source, "device", "RMX1925");
                set_ro_product_prop(source, "model", "RMX1925");
                set_ro_product_prop(source, "name", "RMX1925");
            }
        } else {
            property_override("ro.build.product", "RMX1911");
            for (const auto &source : ro_product_props_default_source_order) {
                set_ro_product_prop(source, "device", "RMX1911");
                set_ro_product_prop(source, "model", "RMX1911");
                set_ro_product_prop(source, "name", "RMX1911");
            }
        }
    }

    // Set dalvik heap configuration
    char const *heapstartsize;
    char const *heapgrowthlimit;
    char const *heapsize;
    char const *heapminfree;
    char const *heapmaxfree;
    char const *heaptargetutilization;

    struct sysinfo sys;
    sysinfo(&sys);

    if (sys.totalram > 3072ull * 1024 * 1024) {
        // from - phone-xhdpi-4096-dalvik-heap.mk
        heapstartsize = "8m";
        heapgrowthlimit = "192m";
        heapsize = "512m";
        heaptargetutilization = "0.6";
        heapminfree = "8m";
        heapmaxfree = "16m";
    } else {
        // from - phone-xhdpi-2048-dalvik-heap.mk
        heapstartsize = "8m";
        heapgrowthlimit = "192m";
        heapsize = "512m";
        heaptargetutilization = "0.75";
        heapminfree = "512k";
        heapmaxfree = "8m";
    }

    property_override("dalvik.vm.heapstartsize", heapstartsize);
    property_override("dalvik.vm.heapgrowthlimit", heapgrowthlimit);
    property_override("dalvik.vm.heapsize", heapsize);
    property_override("dalvik.vm.heaptargetutilization", heaptargetutilization);
    property_override("dalvik.vm.heapminfree", heapminfree);
    property_override("dalvik.vm.heapmaxfree", heapmaxfree);
}
