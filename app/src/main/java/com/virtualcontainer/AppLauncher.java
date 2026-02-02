package com.virtualcontainer;

import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Bundle;

public class AppLauncher {
    public static void launchApp(Context context, String packageName) {
        try {
            // Create virtual context for the app
            VirtualContext virtualContext = new VirtualContext(context);
            
            // Get launch intent
            PackageManager pm = virtualContext.getPackageManager();
            Intent launchIntent = pm.getLaunchIntentForPackage(packageName);
            
            if (launchIntent != null) {
                // Add virtual context flags
                launchIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                launchIntent.putExtra("virtual_context", true);
                
                // Launch with instrumentation for monitoring
                Bundle options = new Bundle();
                options.putString("virtual_storage", VirtualStorageManager.VIRTUAL_PREFIX);
                
                virtualContext.startActivity(launchIntent, options);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}