/*
 *    				~ CHIFEngine ~
 *               
 * Copyright (c) 2025 Lukas Rennhofer
 *
 * Licensed under the MIT License. See LICENSE file for more details.
 *
 * Author: Lukas Rennhofer
 * Date: 2025-03-08
 *
 * File: chifOpenGLPBR.hpp
 * Last Change: 
*/

#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>

#include "../chifOpenGLTexture.hpp"

namespace chif::Graphics::RenderDevice::OpenGL {
    // ====== Base PBRCapture ======
    struct PBRCapture
    {
        TextureCube* Irradiance  = nullptr;
        TextureCube* Prefiltered = nullptr;

        glm::vec3 Position;
        float      Radius;
    };

    // TODO
}