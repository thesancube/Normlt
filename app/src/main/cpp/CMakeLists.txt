cmake_minimum_required(VERSION 3.22.1)

project("virtual_container")

# Add C++17 standard support
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add ImGui sources
file(GLOB IMGUI_SOURCES
    "imgui/*.cpp"
    "imgui/*.h"
)

# Add implementation sources
set(IMPL_SOURCES
    virtual_container.cpp
    imgui_impl_android.cpp
    imgui_impl_opengl3.cpp
)

# Combine all sources
set(ALL_SOURCES ${IMGUI_SOURCES} ${IMPL_SOURCES})

# Create shared library
add_library(virtual_container SHARED ${ALL_SOURCES})

# Find and link Android libraries
find_library(log-lib log)
find_library(android-lib android)
find_library(egl-lib EGL)
find_library(gles2-lib GLESv2)

# Link libraries
target_link_libraries(virtual_container
    ${log-lib}
    ${android-lib}
    ${egl-lib}
    ${gles2-lib}
)

# Add compiler definitions
target_compile_definitions(virtual_container PRIVATE
    IMGUI_IMPL_ANDROID_ES2
    ANDROID
    _ANDROID
)

# Include directories
target_include_directories(virtual_container PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
)

# Set compile flags
target_compile_options(virtual_container PRIVATE
    -Wall
    -Wextra
    -O2
)