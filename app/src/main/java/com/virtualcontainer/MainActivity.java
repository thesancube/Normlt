package com.virtualcontainer;

import android.app.Activity;
import android.os.Bundle;
import android.view.SurfaceView;
import android.view.SurfaceHolder;
import android.widget.Button;
import android.widget.ListView;
import android.widget.ArrayAdapter;
import android.content.pm.PackageManager;
import android.content.pm.ApplicationInfo;
import java.util.ArrayList;
import java.util.List;

public class MainActivity extends Activity implements SurfaceHolder.Callback {
    private SurfaceView glSurface;
    private ListView appList;
    private Button toggleImGui;
    private ArrayAdapter<String> adapter;
    private ArrayList<String> apps = new ArrayList<>();
    
    static {
        System.loadLibrary("virtual_container");
    }
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        glSurface = findViewById(R.id.gl_surface);
        appList = findViewById(R.id.app_list);
        toggleImGui = findViewById(R.id.toggle_imgui);
        
        glSurface.getHolder().addCallback(this);
        
        // Load installed apps
        loadInstalledApps();
        
        // Setup ImGui toggle
        toggleImGui.setOnClickListener(v -> {
            NativeBridge.toggleImGui();
        });
        
        // Initialize native layer
        NativeBridge.init();
    }
    
    private void loadInstalledApps() {
        PackageManager pm = getPackageManager();
        List<ApplicationInfo> installedApps = pm.getInstalledApplications(PackageManager.GET_META_DATA);
        
        for (ApplicationInfo appInfo : installedApps) {
            if ((appInfo.flags & ApplicationInfo.FLAG_SYSTEM) == 0) {
                apps.add(appInfo.packageName);
            }
        }
        
        adapter = new ArrayAdapter<>(this, android.R.layout.simple_list_item_1, apps);
        appList.setAdapter(adapter);
        
        appList.setOnItemClickListener((parent, view, position, id) -> {
            String packageName = apps.get(position);
            AppLauncher.launchApp(this, packageName);
        });
    }
    
    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        NativeBridge.surfaceCreated(holder.getSurface());
    }
    
    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        NativeBridge.surfaceChanged(width, height);
    }
    
    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        NativeBridge.surfaceDestroyed();
    }
}