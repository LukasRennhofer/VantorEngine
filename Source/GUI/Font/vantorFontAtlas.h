
/*
 *    				~ Vantor ~
 *
 * Copyright (c) 2025 Lukas Rennhofer
 *
 * Licensed under the GNU General Public License, Version 3. See LICENSE file for more details.
 *
 * Author: Lukas Rennhofer
 * Date: 2025-04-01
 *
 * File: vantorFontAtlas.h
 * Last Change: Added Dear Imgui Light theme by Pacôme Danhiez and added Demo Profiler
*/

#pragma once

#include <glad/glad.h>
#include <ft2build.h>

#include "../../Core/Backlog/vantorBacklog.h"

#include FT_FREETYPE_H

// Inspiration from https://github.com/jtberglund/GLFont

namespace vantor::GUI {
    class FontAtlas {
        public:
            struct Character {
                float advanceX;
                float advanceY;
        
                float bitmapWidth;
                float bitmapHeight;
        
                float bitmapLeft;
                float bitmapTop;
        
                float xOffset;
            };
        
            FontAtlas(FT_Face face, int pixelSize);
            ~FontAtlas();
        
            inline GLuint getTexId() { return _tex; }
            inline int getAtlasWidth() { return _width; }
            inline int getAtlasHeight() { return _height; }
            inline Character* getCharInfo() { return _chars; }
        
        private:
            FT_Face _face;
            FT_GlyphSlot _slot;
            GLuint _tex;
        
            Character _chars[128];
        
            int _width;  // width of texture
            int _height; // height of texture
    };
} // Namespace vantor::GUI