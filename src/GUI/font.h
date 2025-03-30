
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
#include "../Platform/Window.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Freetype
#include "../External/ft2build.h"
#include FT_FREETYPE_H

#include "../Graphics/Renderer/shader.h"


namespace chif::GUI::font {

    struct Character {
        unsigned int TextureID;
        glm::ivec2   Size;
        glm::ivec2   Bearing;
        unsigned int Advance;
    };

    class TextRenderer
    {
    public:
        std::map<char, Character> Characters; 
        chif::Graphics::Renderer::Shader::Shader* TextShader;

        TextRenderer(unsigned int width, unsigned int height);
        void Load(std::string font, unsigned int fontSize);
        void RenderText(std::string text, float x, float y, float scale, glm::vec3 color = glm::vec3(1.0f));
    private:
        unsigned int VAO, VBO;
    };
} // NAMESPACE chif::GUI::font