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

#ifndef CHIFENGINE
#define CHIFENGINE

#include "Core/version.h"
#include "Core/JobSystem/JobSystem.h"

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

#include "Platform/Window.h"
#include "Platform/glError.h"

#include "GUI/ImguiGUI.h"

#include "Utils/utils.h"
#include "Utils/constants.h"

#include <camera.h>
#include <mesh.h>
#include <model.h>

#endif // CHIFENGINE