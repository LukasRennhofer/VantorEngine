/*
 *  ╔═══════════════════════════════════════════════════════════════╗
 *  ║                          ~ Vantor ~                           ║
 *  ║                                                               ║
 *  ║  This file is part of the Vantor Engine.                      ║
 *  ║  Automatically formatted by vantorFormat.py                   ║
 *  ║                                                               ║
 *  ╚═══════════════════════════════════════════════════════════════╝
 *
 *  Copyright (c) 2025 Lukas Rennhofer
 *  Licensed under the GNU General Public License, Version 3.
 *  See LICENSE file for more details.
 *
 *  Author: Lukas Rennhofer
 *  Date: 2025-06-08
 *
 *  File: Vantor.h
 *  Last Change: Automatically updated
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
          VANTOR_API       = Used for the main header implementation
          VANTOR_DEBUG = Used for identifying a debug build (Defined in CMake) (RESERVED, BUT NOT IN USE)
          VANTOR_PROFILE = Used for profiling the whole engine (Defined by user, or buildTool)

     : Platforms :
          __WINDOWS__ = Used for identifying a Window build (Defined in CMake)
          __LINUX__   = Used for identifying a Linux build (Defined in CMake)
          __NSX__  = Used for identifying a Nintendo Switch build (Defined in
CMake)
          __PS5__     = Used for identifying a PS5 build (Defined in CMake)
(RESERVED, BUT NOT IN USE)
          __XBOX__    = Used for identifying a Xbox Series X/S build (Defined in
CMake) (RESERVED, BUT NOT IN USE)

     : Platform API :
          VANTOR_API_COMPILE = Used to identify if the API is able to be
switched at runtime or if it is being chosen at compile time (Defined in CMake)
                             (If Defined: Compile with only one given API, else:
API is switchable based on platform) VANTOR_API_OPENGL  = Used for building with
RenderDevice OpenGL (Defined in CMake) VANTOR_API_VULKAN  = Used for building
with RenderDevice Vulkan (Defined in CMake) (RESERVED, BUT NOT IN USE)
          VANTOR_API_DIRECTX = Used for building with RenderDevice DirectX
(Defined in CMake) (RESERVED, BUT NOT IN USE)

     : Manager API :
          VANTOR_WM_GLFW = Used to identify if GLFW is being used as the WM
          VANTOR_WM_SDL2 = Used to identify if SDL2 is being used as the WM
          VANTOR_WM_EGL  = Used to identify if EGL is being used as the WM
     
     : Integrations :
          VANTOR_INTEGRATION_IMGUI = Used to identify if DEAR IMGUI is an integrated module of the current build (Defined in CMake)
*/

// VANTOR PREPROCESSOR DEFINITION
#ifndef VANTOR_API
#define VANTOR_API

// CORE
#include "../../Source/Core/VCO_Version.hpp"
#include "../../Source/Core/Backlog/VCO_Backlog.hpp"
#include "../../Source/Core/Global/VCO_ServiceRegistry.hpp"
#include "../../Source/Core/Debug/VCO_InlineDebugger.hpp"
#include "../../Source/Core/IO/VCO_FSUtils.hpp"
#include "../../Source/Core/Types/VCO_Color.hpp"
// CORE/CONTAINER
#include "../../Source/Core/Container/VCO_Vector.hpp"

// APPLICATION
#include "../../Source/Application/VAP_Application.hpp"

// RENDERDEVICE
#include "../../Source/RenderDevice/Interface/VRD_RenderDevice.hpp"
#include "../../Source/RenderDevice/VRD_Factory.hpp"

// RENDERER
#include "../../Source/Renderer/VRE_Material.hpp"
#include "../../Source/Renderer/VRE_Uniform.hpp"
#include "../../Source/Renderer/Light/VRE_LightData.hpp"
// RENDERER/CAMERA
#include "../../Source/Renderer/Camera/VRE_Camera.hpp"
#include "../../Source/Renderer/Camera/VRE_Frustum.hpp"

// CONTEXT
#include "../../Source/Context/Interface/VCT_Window.hpp"

// Platform
#include "../../Source/Platform/VPL_Environment.hpp"

// Resource Manager
#include "../../Source/Resource/VRES_Manager.hpp"

// INPUT
#include "../../Source/InputDevice/VID_IDevice.hpp"
#include "../../Source/InputDevice/VID_Factory.hpp"
#include "../../Source/InputDevice/VID_Manager.hpp"

// OBJECT SYSTEM
#include "../../Source/ObjectSystem/VOS_Object.hpp"
#include "../../Source/ObjectSystem/Component/VOS_Component.hpp"
#include "../../Source/ObjectSystem/Register/VOS_Registry.hpp"
#include "../../Source/ObjectSystem/Component/VOS_Base.hpp"  // Basic Components

// HELPERS
// #include "Source/Helpers/vantorColor.hpp"
// #include "Source/vHelpers/vantorMath.hpp"
// #include "Source/Helpers/vantorTimer.hpp"
// #include "Source/Helpers/vantorFS.hpp"

#endif // VANTOR