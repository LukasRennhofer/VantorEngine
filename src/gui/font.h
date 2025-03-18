
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
 * File: font.h
 * Last Change: Added Dear Imgui Light theme by Pacôme Danhiez and added Demo Profiler
 */

#pragma once

#include <iostream>
#include <map>
#include <string>

#include <glad/glad.h>
#include "../platform/Window.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Freetype
#include "../external/ft2build.h"
#include FT_FREETYPE_H

#include "../Engine/shader.h"
#include "../Engine/ScreenSpaceShader.h"


namespace chif::gui::font {

    bool initFont();
    bool setFont(std::string filePath);
    void RenderText2D(std::string text, float x, float y, float scale, glm::vec3 color);
    void destroyFont();

    /// Holds all state information relevant to a character as loaded using FreeType
    struct Character {
        unsigned int TextureID; // ID handle of the glyph texture
        glm::ivec2   Size;      // Size of glyph
        glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
        unsigned int Advance;   // Horizontal offset to advance to next glyph
    };
} // NAMESPACE CHIF::GUI::FONT