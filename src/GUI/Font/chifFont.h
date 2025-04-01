
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
 * File: chifFont.h
 * Last Change: Added Dear Imgui Light theme by Pacôme Danhiez and added Demo Profiler
 */

#pragma once

#include <iostream>
#include <map>
#include <string>

#include <glad/glad.h>
#include "../Platform/chifWindow.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Freetype
#include "../../External/ft2build.h"
#include FT_FREETYPE_H

#include "../Graphics/Renderer/chifShader.h"

namespace chif::GUI {
    class Font {
    public:
        Font(const char* fontFile);
        ~Font();

        void setFontFile(const char* fontFile);

        FT_Face getFaceHandle();

    private:
        char* _fontFile;
        FT_Error _error;
        FT_Library _ft;
        FT_Face _face;

    };
} // NAMESPACE chif::GUI