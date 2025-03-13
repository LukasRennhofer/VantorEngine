# Dependencies for Building CHIFEngine (TODO)
## 1. Required Tools

- CMake: Version 3.10 or higher
    Used for generating build files.
    Download: [Cmake Website](https://cmake.org/download/)

- Make (on Windows, you will need a Windows-compatible version like MinGW or MSYS2):
        On Linux/macOS, make is typically pre-installed.
        On Windows, use a package manager like MSYS2 to install make.

    To install on Windows:

    ```pacman -S make```

 - GCC or Clang (on Linux/macOS) or MinGW (on Windows):
        GCC (or Clang) for compiling C++ code.
        On Windows, install MinGW via MSYS2 or use a preconfigured GCC toolchain.
        On Linux/macOS, use the system's package manager:
            On Ubuntu: sudo apt install build-essential
            On macOS: brew install gcc

## 2. Libraries

The project uses various libraries that should be set up as dependencies:

### Windows:

- GLFW 3.x (for window creation and input management)
        Include directory: src/Include/GLFW
        Library directory: src/Libraries
        Available at: https://www.glfw.org/download.html
        Make sure to build GLFW or get precompiled binaries and place them in the respective directories.

- OpenGL (for rendering)
        This is typically already available on most systems.
        On Windows, ensure that opengl32.lib is available and linked.

- Additional Windows Libraries:
        gdi32
        user32
        imm32
        shell32

## Linux:

- GLFW 3.x (for window creation and input management)
        Install via package manager: sudo apt install libglfw3-dev
- OpenGL (for rendering)
        Install via package manager: sudo apt install libopengl-dev
- X11 (for window system interaction)
        Install via package manager: sudo apt install libx11-dev
- Additional Linux Libraries:
        pthread: sudo apt install libpthread-dev
        dl: sudo apt install libdl-dev
        jsoncpp: sudo apt install libjsoncpp-dev
        fmt: sudo apt install libfmt-dev
        assimp: sudo apt install libassimp-dev
        EGL: sudo apt install libegl1-mesa-dev
        GLESv2: sudo apt install libgles2-mesa-dev
        gbm: sudo apt install libgbm-dev
        drm: sudo apt install libdrm-dev

## 3. ImGui

ImGui is included within your project in the src/imgui directory.
No external setup required for ImGui as long as it's already included in your project folder.

## 4. stb_image (for texture loading)

The project uses stb_image for loading images (included in the src directory).
No installation required, as stb_image is included in the project itself (src/stb_image_impl.cpp).

## 5. Additional Notes

Make sure to set the following environment variables on Windows if using MinGW or MSYS2:
- CMake: CMake should be available in the PATH.
- MinGW: C:/msys64/mingw64/bin should be included in the PATH for MinGW to work properly.