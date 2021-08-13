/*
 * Copyright (C) 2020 LineageOS Project
 * Copyright (C) 2020 Harshit Jain
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cstdlib>
#include <vector>
#include <string>
#include <fstream>
#include <sys/sysinfo.h>

#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>
#include <sys/sysinfo.h>

#include <android-base/properties.h>
#include <android-base/logging.h>

#include "vendor_init.h"

struct realme_trinket_props
{
    std::string build_description;
    std::string build_fingerprint;
    std::string device_build;
    std::string product_device;
};

std::vector<std::string> ro_props_default_source_order = {
    "",
    "odm.",
    "product.",
    "system.",
    "vendor.",
    "system_ext.",
};

void property_override(char const prop[], char const value[], bool add = true)
{
    prop_info *pi;
    pi = (prop_info *)__system_property_find(prop);
    if (pi)
        __system_property_update(pi, value, strlen(value));
    else if (add)
        __system_property_add(prop, strlen(prop), value, strlen(value));
}

void setRMX(unsigned int variant)
{
    realme_trinket_props prop[4] = {};

    std::string build_desc = "trinket-user 10 QKQ1.200209.002 release-keys";
    std::string build_fingerprint = "google/redfin/redfin:11/RQ3A.210805.001.A1/7474174:user/release-keys";

    //RMX1911
    prop[0] = {
        build_desc,
        build_fingerprint,
        "RMX1911",
        "RMX1911",
    };

    // RMX1925
    prop[1] = {
        build_desc,
        build_fingerprint,
        "RMX1925",
        "RMX1925",
    };

    //RMX1927
    prop[2] = {
        build_desc,
        build_fingerprint,
        "RMX1927",
        "RMX1927",
    };

    //RMX2030
    prop[3] = {
        build_desc,
        build_fingerprint,
        "RMX2030",
        "RMX2030",
    };

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

    property_override("ro.build.description", prop[variant].build_description.c_str());
    property_override("ro.build.product", prop[variant].product_device.c_str());
    for (const auto &source : ro_props_default_source_order)
    {
        set_ro_build_prop(source, "fingerprint", prop[variant].build_fingerprint.c_str());
        set_ro_product_prop(source, "device", prop[variant].product_device.c_str());
        set_ro_product_prop(source, "model", prop[variant].device_build.c_str());
    }
    property_override("ro.build.fingerprint", prop[variant].build_fingerprint.c_str());
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

if (prjVersion == "19743")
    {
        setRMX(3); //RMX2030
    } else if (prjVersion == "19632")
    {
        setRMX(2); //RMX1927
        property_override("persist.sys.nfc.disPowerSave", "1");
        property_override("persist.sys.nfc.default", "on");
        property_override("persist.sys.nfc.aid_overflow", "true");
        property_override("ro.product.cuptsm", "OPPO|ESE|01|02");
        property_override("persist.sys.nfc.antenna_area", "bottom");
        // Set hardware SKU prop
        property_override("ro.boot.product.hardware.sku", "19632");
    }
    else
    {
        if (buf.find("S86125AA1") != std::string::npos) {
            setRMX(1); // RMX1925
        } else {
            setRMX(0); //RMX1911
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
