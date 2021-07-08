/*
* Copyright (C) 2016 The OmniROM Project
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
*/
package com.realme.realmeparts;

import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.content.res.Resources;
import android.content.Intent;
import android.os.Bundle;
import android.provider.Settings;
import android.text.TextUtils;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ListView;
import android.util.Log;
import androidx.preference.ListPreference;
import androidx.preference.Preference;
import androidx.preference.PreferenceCategory;
import androidx.preference.PreferenceFragment;
import androidx.preference.PreferenceGroup;
import androidx.preference.PreferenceScreen;
import androidx.preference.SwitchPreference;
import androidx.preference.TwoStatePreference;

import com.realme.realmeparts.kcal.KCalSettingsActivity;
import com.realme.realmeparts.Constants;

import com.realme.realmeparts.preferences.SecureSettingListPreference;
import com.realme.realmeparts.preferences.SecureSettingSwitchPreference;

public class RealmeParts extends PreferenceFragment
        implements Preference.OnPreferenceChangeListener {

    private static final String PREF_DEVICE_KCAL = "kcal";

    public static final String KEY_VIBSTRENGTH = "vib_strength";
    public static final String KEY_OTG_SWITCH = "otg";

    private Preference mKcal;
    private VibratorStrengthPreference mVibratorStrength;

    private static TwoStatePreference mOTGModeSwitch;

    public static final String PREF_USB_FASTCHARGE = "fastcharge";
    public static final String USB_FASTCHARGE_PATH = "/sys/kernel/fast_charge/force_fast_charge";
    private SwitchPreference mUsbFastCharger;

    public static final String PREF_ENABLE_DIRAC = "dirac_enabled";
    public static final String PREF_HEADSET = "dirac_headset_pref";
    public static final String PREF_PRESET = "dirac_preset_pref";

    private SecureSettingSwitchPreference mEnableDirac;
    private SecureSettingListPreference mHeadsetType;
    private SecureSettingListPreference mPreset;

    @Override
    public void onCreatePreferences(Bundle savedInstanceState, String rootKey) {
        addPreferencesFromResource(R.xml.main);
        getActivity().getActionBar().setDisplayHomeAsUpEnabled(true);

        mVibratorStrength = (VibratorStrengthPreference) findPreference(KEY_VIBSTRENGTH);
        if (mVibratorStrength != null)
            mVibratorStrength.setEnabled(VibratorStrengthPreference.isSupported());

        mKcal = findPreference(PREF_DEVICE_KCAL);

        mKcal.setOnPreferenceClickListener(preference -> {
            Intent intent = new Intent(getActivity().getApplicationContext(), KCalSettingsActivity.class);
            startActivity(intent);
            return true;
        });

        mOTGModeSwitch = (TwoStatePreference) findPreference(KEY_OTG_SWITCH);
        mOTGModeSwitch.setEnabled(OTGModeSwitch.isSupported());
        mOTGModeSwitch.setChecked(OTGModeSwitch.isCurrentlyEnabled(this.getContext()));
        mOTGModeSwitch.setOnPreferenceChangeListener(new OTGModeSwitch());

        if (FileUtils.fileWritable(USB_FASTCHARGE_PATH)) {
            mUsbFastCharger = findPreference(PREF_USB_FASTCHARGE);
            mUsbFastCharger.setEnabled(UsbFastCharge.isSupported());
            mUsbFastCharger.setChecked(UsbFastCharge.isCurrentlyEnabled(this.getContext()));
            mUsbFastCharger.setOnPreferenceChangeListener(new UsbFastCharge(getContext()));
        } else {
            getPreferenceScreen().removePreference(findPreference(PREF_USB_FASTCHARGE));
        }

        boolean enhancerEnabled;
        try {
            enhancerEnabled = DiracService.sDiracUtils.isDiracEnabled();
        } catch (java.lang.NullPointerException e) {
            getContext().startService(new Intent(getContext(), DiracService.class));
            try {
                enhancerEnabled = DiracService.sDiracUtils.isDiracEnabled();
            } catch (NullPointerException ne) {
                // Avoid crash
                ne.printStackTrace();
                enhancerEnabled = false;
            }
        }

        mEnableDirac = (SecureSettingSwitchPreference) findPreference(PREF_ENABLE_DIRAC);
        mEnableDirac.setOnPreferenceChangeListener(new DiracPrefs(getContext()));
        mEnableDirac.setChecked(enhancerEnabled);

        mHeadsetType = (SecureSettingListPreference) findPreference(PREF_HEADSET);
        mHeadsetType.setOnPreferenceChangeListener(new DiracPrefs(getContext()));

        mPreset = (SecureSettingListPreference) findPreference(PREF_PRESET);
        mPreset.setOnPreferenceChangeListener(new DiracPrefs(getContext()));
    }

    @Override
    public boolean onPreferenceChange(Preference preference, Object newValue) {
        Constants.setPreferenceInt(getContext(), preference.getKey(), Integer.parseInt((String) newValue));
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
        // Respond to the action bar's Up/Home button
        case android.R.id.home:
            getActivity().finish();
            return true;
        }
        return super.onOptionsItemSelected(item);
    }
}
