package com.virtualcontainer;

import java.io.File;
import java.util.HashMap;
import java.util.Map;

public class VirtualStorageManager {
    private static final String VIRTUAL_PREFIX = "/sdcard/VSDCARD";
    private Map<String, String> pathMappings = new HashMap<>();
    
    public VirtualStorageManager() {
        initializeMappings();
    }
    
    private void initializeMappings() {
        // Map common storage paths to virtual equivalents
        pathMappings.put("/sdcard/Android/obb", VIRTUAL_PREFIX + "/Android/obb");
        pathMappings.put("/sdcard/Android/data", VIRTUAL_PREFIX + "/Android/data");
        pathMappings.put("/sdcard/Download", VIRTUAL_PREFIX + "/Download");
        pathMappings.put("/sdcard/Pictures", VIRTUAL_PREFIX + "/Pictures");
    }
    
    public String mapPath(String originalPath) {
        // Check if path matches any mapping pattern
        for (Map.Entry<String, String> mapping : pathMappings.entrySet()) {
            if (originalPath.startsWith(mapping.getKey())) {
                return originalPath.replace(mapping.getKey(), mapping.getValue());
            }
        }
        return originalPath;
    }
    
    public String mapObbPath(String originalObbPath) {
        // Special handling for OBB paths
        if (originalObbPath.contains("/Android/obb/")) {
            return originalObbPath.replace("/sdcard/Android/obb/", VIRTUAL_PREFIX + "/Android/obb/");
        }
        return originalObbPath;
    }
    
    public void createVirtualStructure() {
        // Create virtual directory structure
        new File(VIRTUAL_PREFIX + "/Android/obb").mkdirs();
        new File(VIRTUAL_PREFIX + "/Android/data").mkdirs();
        new File(VIRTUAL_PREFIX + "/Download").mkdirs();
        new File(VIRTUAL_PREFIX + "/Pictures").mkdirs();
    }
}