
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
 * File: font.cpp
 * Last Change: Added Dear Imgui Light theme by Pacôme Danhiez and added Demo Profiler
 */

#include "font.h"

namespace chif::gui::font {
    // FT LIB
    FT_Library ft;
    FT_Face face;
    // CHARS
    std::map<GLchar, Character> Characters;
    std::string currentFont = "resources/calibri.ttf";
    chif::Shader::Shader fontShader("fontShader");
    unsigned int texture;
    unsigned int VAO, VBO;

    bool initFont() {
        fontShader.attachShader("shaders/text.vert");
        fontShader.attachShader("shaders/text.frag");
        fontShader.linkPrograms();
        glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(chif::Window::SCR_WIDTH), 0.0f, static_cast<float>(chif::Window::SCR_HEIGHT));
        fontShader.use();
        glUniformMatrix4fv(glGetUniformLocation(fontShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        if (FT_Init_FreeType(&ft)) {
                std::cout << "[ERROR::FREETYPE] Could not init FreeType Library" << std::endl;
                return false;
        }

        return true;
    }

    bool setFont(std::string filePath) {
        currentFont = filePath;

        if (currentFont.empty()) {
            std::cout << "[ERROR::FREETYPE] Failed to load the Current Font" << std::endl;
            return false;
        }
        
        if (FT_New_Face(ft, currentFont.c_str(), 0, &face)) {
            std::cout << "[ERROR::FREETYP] Failed to load font" << std::endl;
            return false;
        } else {
            // set size to load glyphs as
            FT_Set_Pixel_Sizes(face, 0, 48);

            // disable byte-alignment restriction
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            // load first 128 characters of ASCII set
            for (unsigned char c = 0; c < 128; c++)
            { 
                if (FT_Load_Char(face, c, FT_LOAD_RENDER))
                {
                    std::cout << "[ERROR::FREETYTPE] Failed to load Glyph" << std::endl;
                    continue;
                }

                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture);
                glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_RED,
                    face->glyph->bitmap.width,
                    face->glyph->bitmap.rows,
                    0,
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    face->glyph->bitmap.buffer
                );
                // set texture options
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                // now store character for later use
                Character character = {
                    texture,
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    static_cast<unsigned int>(face->glyph->advance.x)
                };
                Characters.insert(std::pair<char, Character>(c, character));
            }
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        return true;
    }

    void RenderText2D(std::string text, float x, float y, float scale, glm::vec3 color)
    {
        // activate corresponding render state	
        fontShader.use();
        glUniform3f(glGetUniformLocation(fontShader.ID, "textColor"), color.x, color.y, color.z);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);

        // iterate through all characters
        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++) 
        {
            Character ch = Characters[*c];

            float xpos = x + ch.Bearing.x * scale;
            float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

            float w = ch.Size.x * scale;
            float h = ch.Size.y * scale;
            // update VBO for each character
            float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },            
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }           
            };
            // render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            // update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void destroyFont() {
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    }
} // NAMESPACE CHIF::GUI::FONT