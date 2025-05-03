
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
 * File: vantorFontUtils.h
 * Last Change: Added Dear Imgui Light theme by Pacôme Danhiez and added Demo Profiler
*/


#pragma once

#include <glad/glad.h>

#include "../../Core/Backlog/vantorBacklog.h"

class FontUtils {
public:
    FontUtils();
    ~FontUtils();

    static void loadShader(char* shaderSource, GLenum shaderType, GLuint &programId);
};
