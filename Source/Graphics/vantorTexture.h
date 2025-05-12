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
 *  File: vantorTexture.h
 *  Last Change: Automatically updated
 */

#pragma once
#include <stb_image.h>
#include <string>
#include <iostream>
#include <vector>

#include "../Core/Backlog/vantorBacklog.h"

#include <glad/glad.h>

using namespace std;

namespace vantor::Graphics
{
    unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);
    unsigned int loadCubemap(vector<std::string> faces);
    unsigned int generateTexture2D(int w, int h);
    unsigned int generateTexture3D(int w, int h, int d);
    void         bindTexture2D(unsigned int tex, int unit = 0);
} // NAMESPACE vantor::Graphics