#include <jni.h>
#include <android/log.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include "imgui.h"
#include "imgui_impl_android.h"
#include "imgui_impl_opengl3.h"

#define LOG_TAG "VirtualContainer"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

static EGLDisplay display = EGL_NO_DISPLAY;
static EGLSurface surface = EGL_NO_SURFACE;
static EGLContext context = EGL_NO_CONTEXT

static ANativeWindow* window = nullptr;
static bool showImGui = false;

extern "C" JNIEXPORT void JNICALL
Java_com_virtualcontainer_NativeBridge_init(JNIEnv* env, jclass clazz) {
    LOGI("Initializing native container");
    
    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
}

extern "C" JNIEXPORT void JNICALL
Java_com_virtualcontainer_NativeBridge_surfaceCreated(JNIEnv* env, jclass clazz, jobject surfaceObj) {
    LOGI("Surface created");
    
    // Get native window
    window = ANativeWindow_fromSurface(env, surfaceObj);
    if (!window) {
        LOGE("Failed to get native window");
        return;
    }
    
    // Initialize EGL
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (display == EGL_NO_DISPLAY) {
        LOGE("Failed to get EGL display");
        return;
    }
    
    if (!eglInitialize(display, nullptr, nullptr)) {
        LOGE("Failed to initialize EGL");
        return;
    }
    
    // Configure EGL
    const EGLint attribs[] = {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_BLUE_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8,
        EGL_DEPTH_SIZE, 24,
        EGL_NONE
    };
    
    EGLConfig config;
    EGLint numConfigs;
    if (!eglChooseConfig(display, attribs, &config, 1, &numConfigs)) {
        LOGE("Failed to choose EGL config");
        return;
    }
    
    // Create EGL surface
    surface = eglCreateWindowSurface(display, config, window, nullptr);
    if (surface == EGL_NO_SURFACE) {
        LOGE("Failed to create EGL surface");
        return;
    }
    
    // Create EGL context
    const EGLint ctxAttribs[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };
    
    context = eglCreateContext(display, config, EGL_NO_CONTEXT, ctxAttribs);
    if (context == EGL_NO_CONTEXT) {
        LOGE("Failed to create EGL context");
        return;
    }
    
    if (!eglMakeCurrent(display, surface, surface, context)) {
        LOGE("Failed to make EGL context current");
        return;
    }
    
    // Initialize ImGui OpenGL ES implementation
    ImGui_ImplAndroid_Init(window);
    ImGui_ImplOpenGL3_Init("#version 100");
    
    LOGI("EGL and ImGui initialized successfully");
}

extern "C" JNIEXPORT void JNICALL
Java_com_virtualcontainer_NativeBridge_surfaceChanged(JNIEnv* env, jclass clazz, jint width, jint height) {
    LOGI("Surface changed: %dx%d", width, height);
    
    if (display != EGL_NO_DISPLAY && surface != EGL_NO_SURFACE) {
        eglResizeSurface(display, surface);
        glViewport(0, 0, width, height);
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_virtualcontainer_NativeBridge_surfaceDestroyed(JNIEnv* env, jclass clazz) {
    LOGI("Surface destroyed");
    
    // Cleanup ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplAndroid_Shutdown();
    
    // Cleanup EGL
    if (display != EGL_NO_DISPLAY) {
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (context != EGL_NO_CONTEXT) {
            eglDestroyContext(display, context);
            context = EGL_NO_CONTEXT;
        }
        if (surface != EGL_NO_SURFACE) {
            eglDestroySurface(display, surface);
            surface = EGL_NO_SURFACE;
        }
        eglTerminate(display);
        display = EGL_NO_DISPLAY;
    }
    
    if (window) {
        ANativeWindow_release(window);
        window = nullptr;
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_virtualcontainer_NativeBridge_toggleImGui(JNIEnv* env, jclass clazz) {
    showImGui = !showImGui;
    LOGI("ImGui toggled: %s", showImGui ? "ON" : "OFF");
}

extern "C" JNIEXPORT void JNICALL
Java_com_virtualcontainer_NativeBridge_renderFrame(JNIEnv* env, jclass clazz) {
    if (display == EGL_NO_DISPLAY || surface == EGL_NO_SURFACE) {
        return;
    }
    
    // Clear screen
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    if (showImGui) {
        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplAndroid_NewFrame();
        ImGui::NewFrame();
        
        // Create ImGui interface
        ImGui::Begin("Virtual Container Control Panel");
        ImGui::Text("Application Status: RUNNING");
        ImGui::Text("Virtual Storage: /sdcard/VSDCARD");
        
        if (ImGui::Button("Clear Virtual Cache")) {
            // Implement cache clearing logic
            LOGI("Clearing virtual cache");
        }
        
        if (ImGui::Button("Show Storage Mapping")) {
            // Show storage mapping info
            LOGI("Storage mapping displayed");
        }
        
        ImGui::SliderFloat("UI Scale", &ImGui::GetIO().FontGlobalScale, 0.5f, 2.0f);
        ImGui::End();
        
        // Render ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    
    // Swap buffers
    eglSwapBuffers(display, surface);
}