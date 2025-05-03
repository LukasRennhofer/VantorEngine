/*
 *    				~ Vantor ~
 *               
 * Copyright (c) 2025 Lukas Rennhofer
 *
 * Licensed under the GNU General Public License, Version 3. See LICENSE file for more details.
 *
 * Author: Lukas Rennhofer
 * Date: 2025-03-08
 *
 * File: vantorOpenGLPBR.hpp
 * Last Change: 
*/

#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "../vantorOpenGLTexture.hpp"

namespace vantor::Graphics::RenderDevice::OpenGL {
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