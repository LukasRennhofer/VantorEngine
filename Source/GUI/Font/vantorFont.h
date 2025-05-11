/*
 *  ╔═══════════════════════════════════════════════════════════════╗
 *  ║                          ~ Vantor ~                           ║
 *  ║                                                               ║
 *  ║  This file is part of the Vantor Engine.                      ║
 *  ║  Automatically formatted by vantorFormat.py                   ║
 *  ║                                                               ║
 *  ╚═══════════════════════════════════════════════════════════════╝
 *
 *  Copyright (c) 2025 Lukas Rennhofer
 *  Licensed under the GNU General Public License, Version 3.
 *  See LICENSE file for more details.
 *
 *  Author: Lukas Rennhofer
 *  Date: 2025-05-11
 *
 *  File: vantorFont.h
 *  Last Change: Automatically updated
 */

#pragma once

#include <iostream>
#include <map>
#include <string>

#include <glad/glad.h>
#include "../Platform/vantorWindow.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Freetype
#include "../../External/ft2build.h"
#include FT_FREETYPE_H

#include "../Graphics/Renderer/vantorShader.h"

namespace vantor::GUI
{
    class Font
    {
        public:
            Font(const char *fontFile);
            ~Font();

            void setFontFile(const char *fontFile);

            FT_Face getFaceHandle();

        private:
            char      *_fontFile;
            FT_Error   _error;
            FT_Library _ft;
            FT_Face    _face;
    };
} // NAMESPACE vantor::GUI