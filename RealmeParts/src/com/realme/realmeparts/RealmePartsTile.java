package com.realme.realmeparts;

import android.content.ActivityNotFoundException;
import android.content.Intent;
import android.service.quicksettings.TileService;

import com.realme.realmeparts.RealmeParts;
import com.realme.realmeparts.RealmePartsActivity;

public class RealmePartsTile extends TileService {

    @Override
    public void onClick() {
        try {
            Intent intent = new Intent(this, RealmePartsActivity.class);
            intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            startActivityAndCollapse(intent);
        } catch (ActivityNotFoundException ignored) {
            // At this point, the app is most likely hidden and set to only open from Settings
            Intent intent = new Intent(this, RealmeParts.class);
            startActivityAndCollapse(intent);
        }
    }
}