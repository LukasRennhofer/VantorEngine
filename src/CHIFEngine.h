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

     - Windows x64 (7 - 11): libCHIFEnginelibWindows.a
     - Linux x64 (Ubuntu, Debian): libCHIFEnginelibLinux.a
     - Nintendo Switch (Homebrew): libCHIFEnginelibSwitch.a
*/

// CHIFENGINE PREPROCESSOR DEFINITION
#ifndef CHIFENGINE
#define CHIFENGINE

// CORE
#include "Core/version.h"
#include "Core/JobSystem/JobSystem.h"

// GRAPHICS
#include "Graphics/Renderer/BaseShader.h"
#include "Graphics/Renderer/buffers.h"
#include "Graphics/Renderer/ScreenSpaceShader.h"
#include "Graphics/Renderer/shader.h"
#include "Graphics/sceneElements.h"
#include "Graphics/CloudsModel.h"
#include "Graphics/drawableObject.h"
#include "Graphics/VolumetricClouds.h"
#include "Graphics/Water.h"
#include "Graphics/Terrain.h"
#include "Graphics/texture.h"

// PLATFORM
#include "Platform/Window.h"
#include "Platform/glError.h"

// GUI
#include "GUI/ImguiGUI.h"

// UTILITIES
#include "Utils/utils.h"
#include "Utils/constants.h"

// EXTERNAL-INCLUDED HEADERS
#include <camera.h>
#include <mesh.h>
#include <model.h>

#endif // CHIFENGINE