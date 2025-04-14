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
 * Last Change: Updated the projects docst
 */

/*
INFO: CHIFEngine.h can be only compiled with the corresponding Library:

Based on your System:

     - Windows x64 (Tested on 7 - 11): libCHIFEngineWindows.a
     - Linux x64 (Tested on Ubuntu 24.10): libCHIFEngineLinux.a
     - Nintendo Switch (Tested on Homebrew): libCHIFEngineSwitch.a
*/

// CHIFENGINE PREPROCESSOR DEFINITION
#ifndef CHIFENGINE
#define CHIFENGINE

// CORE
#include "Core/chifApplication.hpp"
#include "Core/chifVersion.h"
#include "Core/JobSystem/chifJobSystem.h"
#include "Core/Backlog/chifBacklog.h"
#include "Core/Debug/chifInlineDebugger.h"

// ENTITY
#include "Entity/chifECS.h"

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
#include "Utils/OpenGL/glError.h"

// EXTERNAL-INCLUDED HEADERS
#include <camera.h>
#include <mesh.h>
#include <model.h>

#endif // CHIFENGINE