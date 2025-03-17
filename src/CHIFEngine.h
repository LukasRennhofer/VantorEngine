
/*
 *    				~ CHIFEngine ~
 *               
 * Copyright (c) 2025 Lukas Rennhofer
 *
 * Licensed under the MIT License. See LICENSE file for more details.
 *
 * Author: Lukas Rennhofer
 * Date: 2025-03-11
 *
 * File: CHIFEngine.h
 * Last Change: Added Nintendo Switch (Homebrew) Files for Init
*/

// TODO: CMake file for compiling the CHIFEngine Library

#ifndef CHIFENGINE
#define CHIFENGINE
#define CHIF_ENGINE

#include <camera.h>
#include <mesh.h>
#include <model.h>

#include "core/version.h"

#include "Engine/BaseShader.h"
#include "Engine/buffers.h"
#include "Engine/sceneElements.h"
#include "Engine/ScreenSpaceShader.h"
#include "Engine/shader.h"

#include "graphics/CloudsModel.h"
#include "graphics/drawableObject.h"
#include "graphics/VolumetricClouds.h"
#include "graphics/Water.h"

#include "gui/ImguiGUI.h"

#include "platform/glError.h"
#include "platform/Window.h"

#include "utilities/utils.h"
#include "utilities/constants.h"
#include "utilities/terrain/Terrain.h"
#include "utilities/texture/texture.h"

#endif // CHIFENGINE