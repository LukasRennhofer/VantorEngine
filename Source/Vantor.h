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
          VANTOR = Used for the main header implementation

     : Platforms :
          __WINDOWS__ = Used for identifying a Window build (Defined in CMake)
          __LINUX__ = Used for identifying a Linux build (Defined in CMake)
          __SWITCH__ = Used for identifying a Nintendo Switch build (Defined in CMake)

     : Platform API :
          VANTOR_API_COMPILE = Used to identify if the API is able to be switched at runtime
                             or if it is being chosen at compile time (Defined in CMake) 
                             (If Defined: Compile with only one given API, else: API is switchable based on platform)
          VANTOR_API_OPENGL = Used for building with RenderDevice OpenGL (Defined in CMake)
          VANTOR_API_VULKAN = Used for building with RenderDevice Vulkan (Defined in CMake)
          VANTOR_API_DIRECTX = Used for building with RenderDevice DirectX (Defined in CMake)

*/

// VANTOR PREPROCESSOR DEFINITION
#ifndef VANTOR
#define VANTOR

// CORE
#include "Core/vantorApplication.hpp"
#include "Core/vantorVersion.h"
#include "Core/ResourceManager/vantorResourceManager.hpp"
#include "Core/JobSystem/vantorJobSystem.h"
#include "Core/Backlog/vantorBacklog.h"
#include "Core/Debug/vantorInlineDebugger.h"

// ENTITY
// #include "Entity/vantorECS.h"

// GRAPHICS
#include "Graphics/Renderer/vantorBaseShader.h"
#include "Graphics/Renderer/vantorBuffers.h"
#include "Graphics/Renderer/vantorScreenSpaceShader.h"
#include "Graphics/Renderer/vantorShader.h"
#include "Graphics/vantorSceneElements.h"
#include "Graphics/vantorCloudsModel.h"
#include "Graphics/vantorDrawableObject.h"
#include "Graphics/vantorVolumetricClouds.h"
#include "Graphics/vantorWater.h"
#include "Graphics/vantorTerrain.h"
#include "Graphics/vantorTexture.h"
// RENDERDEVICE: TODO: MOVE AWAY
#include "Graphics/RenderDevice/vantorRenderDevice.hpp"
// PLATFORM
#include "Platform/vantorWindow.h"
#include "Platform/vantorInput.hpp"
// PLATFORMS
#include "Platform/Platforms/vantorPlatformEnvironment.hpp"

// GUI
#include "GUI/Font/vantorFont.h"
#include "GUI/Font/vantorFontAtlas.h"
#include "GUI/Font/vantorLabel.h"
#include "GUI/vantorImgui.h"

// HELPERS
#include "Helpers/vantorColor.hpp"
#include "Helpers/vantorMath.hpp"
#include "Helpers/vantorTimer.hpp"
#include "Helpers/vantorFS.hpp"

// UTILITIES
#include "Utils/utils.h"
#include "Utils/constants.h"
#include "Utils/OpenGL/glError.h" // TODO: Move to RenderDevice

// EXTERNAL-INCLUDED HEADERS
#include <camera.h>
#include <mesh.h>
#include <model.h>

#endif // VANTOR