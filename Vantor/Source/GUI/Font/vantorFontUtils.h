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
 *  Date: 2025-05-12
 *
 *  File: vantorFontUtils.h
 *  Last Change: Automatically updated
 */

#pragma once

#include <glad/glad.h>

#include "../../Core/Backlog/vantorBacklog.h"

class FontUtils
{
    public:
        FontUtils();
        ~FontUtils();

        static void loadShader(char *shaderSource, GLenum shaderType, GLuint &programId);
};
