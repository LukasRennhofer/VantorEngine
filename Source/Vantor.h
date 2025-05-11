/*
 *    				~ Vantor ~
 *
 * Copyright (c) 2025 Lukas Rennhofer
 *
 * Licensed under the GNU General Public License, Version 3. See LICENSE file for more details.
 *
 * Author: Lukas Rennhofer
 * Date: 2025-03-27
 *
 * File: Vantor.h
 * Last Change: Updated the projects docs
*/

/*
INFO: Vantor.h can be only compiled with the corresponding Library:

Based on your System:

     - Windows x64 (Tested on 7 - 11): libVantorWindows.a
     - Linux x64 (Tested on Ubuntu 24.10): libVantorLinux.a
     - Nintendo Switch (Tested on Homebrew): libVantorSwitch.a
*/

/*
Vantors Preprocessor Variables:
     : Main Variables:
          VANTOR       = Used for the main header implementation
          VANTOR_DEBUG = Used for identifying a debug build (Defined in CMake) (RESERVED, BUT NOT IN USE)

     : Platforms :
          __WINDOWS__ = Used for identifying a Window build (Defined in CMake)
          __LINUX__   = Used for identifying a Linux build (Defined in CMake)
          __SWITCH__  = Used for identifying a Nintendo Switch build (Defined in CMake)
          __PS5__     = Used for identifying a PS5 build (Defined in CMake) (RESERVED, BUT NOT IN USE)
          __XBOX__    = Used for identifying a Xbox Series X/S build (Defined in CMake) (RESERVED, BUT NOT IN USE)

     : Platform API :
          VANTOR_API_COMPILE = Used to identify if the API is able to be switched at runtime
                             or if it is being chosen at compile time (Defined in CMake) 
                             (If Defined: Compile with only one given API, else: API is switchable based on platform)
          VANTOR_API_OPENGL  = Used for building with RenderDevice OpenGL (Defined in CMake)
          VANTOR_API_VULKAN  = Used for building with RenderDevice Vulkan (Defined in CMake) (RESERVED, BUT NOT IN USE)
          VANTOR_API_DIRECTX = Used for building with RenderDevice DirectX (Defined in CMake) (RESERVED, BUT NOT IN USE)

     : Manager API :
          VANTOR_WM_GLFW = Used to identify if GLFW is being used as the WM
          VANTOR_WM_SDL2 = Used to identify if SDL2 is being used as the WM
          VANTOR_WM_EGL  = Used to identify if EGL is being used as the WM

*/

// VANTOR PREPROCESSOR DEFINITION
#ifndef VANTOR
#define VANTOR

// CORE
#include "Core/vantorApplication.hpp"
#include "Core/vantorVersion.h"
#include "Core/JobSystem/vantorJobSystem.h"
#include "Core/Backlog/vantorBacklog.h"
#include "Core/Debug/vantorInlineDebugger.h"
#include "Core/Resource/vantorResource.cpp"
#include "Core/Resource/vantorResourceLoader.cpp"

// ENTITY
// #include "Entity/vantorECS.h"

// GRAPHICS
// GRAPHICS / GEOMETRY / PRIMITIVES
#include "Graphics/Geometry/Primitives/vantorCircle.hpp"
#include "Graphics/Geometry/Primitives/vantorCube.hpp"
#include "Graphics/Geometry/Primitives/vantorSphere.hpp"
#include "Graphics/Geometry/Primitives/vantorLine.hpp"
#include "Graphics/Geometry/Primitives/vantorPlane.hpp"
#include "Graphics/Geometry/Primitives/vantorQuad.hpp"
#include "Graphics/Geometry/Primitives/vantorSphere.hpp"
#include "Graphics/Geometry/Primitives/vantorTorus.hpp"
#include "Graphics/Renderer/Background/vantorBackground.hpp"
#include "Graphics/Renderer/Camera/vantorCamera.hpp"
#include "Graphics/Renderer/Light/vantorDirectionalLight.hpp"
#include "Graphics/Renderer/Light/vantorPointLight.hpp"
// RENDERDEVICE
#include "Graphics/RenderDevice/vantorRenderDevice.hpp"
// PLATFORM
#include "Platform/vantorWindow.h"
#include "Platform/vantorInput.hpp"
// PLATFORMS
#include "Platform/Platforms/vantorPlatformEnvironment.hpp"

// GUI
#include "GUI/vantorImgui.h"

// HELPERS
#include "Helpers/vantorColor.hpp"
#include "Helpers/vantorMath.hpp"
#include "Helpers/vantorTimer.hpp"
#include "Helpers/vantorFS.hpp"

// UTILITIES
#include "Utils/utils.h"
#include "Utils/constants.h"

#endif // VANTOR