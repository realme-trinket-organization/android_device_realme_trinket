/*
 * Copyright (C) 2015 The CyanogenMod Project
 * Copyright (C) 2017 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.realme.realmeparts;

import java.util.HashMap;
import java.util.Map;

import android.content.Context;
import android.content.SharedPreferences;
import android.os.UserHandle;
import android.provider.Settings;
import androidx.preference.PreferenceManager;
import androidx.preference.SwitchPreference;

import android.media.AudioManager;

public class Constants {

    public static final Map<String, String> sStringKeyPreferenceMap = new HashMap<>();
    public static final Map<String, Integer> sKeyDefaultMap = new HashMap<>();

    public static int getPreferenceInt(Context context, String key) {
        return Settings.System.getIntForUser(context.getContentResolver(),
                sStringKeyPreferenceMap.get(key), sKeyDefaultMap.get(key), UserHandle.USER_CURRENT);
    }

    public static void setPreferenceInt(Context context, String key, int value) {
        Settings.System.putIntForUser(context.getContentResolver(),
                sStringKeyPreferenceMap.get(key), value, UserHandle.USER_CURRENT);
    }
}
