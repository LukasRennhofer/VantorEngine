/*
 *    				~ CHIFEngine ~
 *
 * Copyright (c) 2025 Lukas Rennhofer
 *
 * Licensed under the MIT License. See LICENSE file for more details.
 *
 * Author: Lukas Rennhofer
 * Date: 2025-03-27
 *
 * File: CHIFEngine.h
 * Last Change: Updated the projects docs
*/

/*
INFO: CHIFEngine.h can be only compiled with the corresponding Library:

Based on your System:

     - Windows x64 (Tested on 7 - 11): libCHIFEngineWindows.a
     - Linux x64 (Tested on Ubuntu 24.10): libCHIFEngineLinux.a
     - Nintendo Switch (Tested on Homebrew): libCHIFEngineSwitch.a
*/

/*
CHIFEngines Preprocessor Variables:
     : Main Variables:
          CHIFENGINE = Used for the main header implementation

     : Platforms :
          __WINDOWS__ = Used for identifying a Window build (Defined in CMake)
          __LINUX__ = Used for identifying a Linux build (Defined in CMake)
          __SWITCH__ = Used for identifying a Nintendo Switch build (Defined in CMake)

     : Platform API :
          CHIF_API_COMPILE = Used to identify if the API is able to be switched at runtime
                             or if it is being chosen at compile time (Defined in CMake) 
                             (If Defined: Compile with only one given API, else: API is switchable based on platform)
          CHIF_API_OPENGL = Used for building with RenderDevice OpenGL (Defined in CMake)
          CHIF_API_VULKAN = Used for building with RenderDevice Vulkan (Defined in CMake)
          CHIF_API_DIRECTX = Used for building with RenderDevice DirectX (Defined in CMake)

*/

// CHIFENGINE PREPROCESSOR DEFINITION
#ifndef CHIFENGINE
#define CHIFENGINE

// CORE
#include "Core/chifApplication.hpp"
#include "Core/chifVersion.h"
#include "Core/ResourceManager/chifResourceManager.hpp"
#include "Core/JobSystem/chifJobSystem.h"
#include "Core/Backlog/chifBacklog.h"
#include "Core/Debug/chifInlineDebugger.h"

// ENTITY
// #include "Entity/chifECS.h"

// GRAPHICS
#include "Graphics/Renderer/chifBaseShader.h"
#include "Graphics/Renderer/chifBuffers.h"
#include "Graphics/Renderer/chifScreenSpaceShader.h"
#include "Graphics/Renderer/chifShader.h"
#include "Graphics/chifSceneElements.h"
#include "Graphics/chifCloudsModel.h"
#include "Graphics/chifDrawableObject.h"
#include "Graphics/chifVolumetricClouds.h"
#include "Graphics/chifWater.h"
#include "Graphics/chifTerrain.h"
#include "Graphics/chifTexture.h"
// RENDERDEVICE: TODO: MOVE AWAY
#include "Graphics/RenderDevice/chifRenderDevice.hpp"
// PLATFORM
#include "Platform/chifWindow.h"
#include "Platform/chifInput.hpp"
// PLATFORMS
#include "Platform/Platforms/chifPlatformEnvironment.hpp"

// GUI
#include "GUI/Font/chifFont.h"
#include "GUI/Font/chifFontAtlas.h"
#include "GUI/Font/chifLabel.h"
#include "GUI/chifImgui.h"

// HELPERS
#include "Helpers/chifColor.hpp"
#include "Helpers/chifMath.hpp"
#include "Helpers/chifTimer.hpp"
#include "Helpers/chifFS.hpp"

// UTILITIES
#include "Utils/utils.h"
#include "Utils/constants.h"
#include "Utils/OpenGL/glError.h" // TODO: Move to RenderDevice

// EXTERNAL-INCLUDED HEADERS
#include <camera.h>
#include <mesh.h>
#include <model.h>

#endif // CHIFENGINE