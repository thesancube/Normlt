package com.virtualcontainer;

import android.content.Context;
import android.content.ContextWrapper;
import android.os.Environment;
import java.io.File;

public class VirtualContext extends ContextWrapper {
    private VirtualStorageManager storageManager;
    
    public VirtualContext(Context base) {
        super(base);
        storageManager = new VirtualStorageManager();
    }
    
    @Override
    public File getExternalFilesDir(String type) {
        // Redirect to virtual storage
        String virtualPath = storageManager.mapPath(
            super.getExternalFilesDir(type).getAbsolutePath()
        );
        return new File(virtualPath);
    }
    
    @Override
    public File getObbDir() {
        // Map /sdcard/Android/obb/<package> to /sdcard/VSDCARD/Android/obb/<package>
        String originalPath = super.getObbDir().getAbsolutePath();
        String virtualPath = storageManager.mapObbPath(originalPath);
        return new File(virtualPath);
    }
    
    @Override
    public File[] getObbDirs() {
        File originalObb = super.getObbDir();
        String virtualPath = storageManager.mapObbPath(originalObb.getAbsolutePath());
        return new File[]{new File(virtualPath)};
    }
}