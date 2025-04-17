
/*
 *    				~ CHIFEngine ~
 *               
 * Copyright (c) 2025 Lukas Rennhofer
 *
 * Licensed under the MIT License. See LICENSE file for more details.
 *
 * Author: Lukas Rennhofer
 * Date: 2025-03-08
 *
 * File: chifTexture.h
 * Last Change: 
 */
 

#pragma once
#include <stb_image.h>
#include <string>
#include <iostream>
#include <vector>

#include "../Core/Backlog/chifBacklog.h"

#include <glad/glad.h>

using namespace std;

namespace chif::Graphics {
    unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);
    unsigned int loadCubemap(vector<std::string> faces);
    unsigned int generateTexture2D(int w, int h);
    unsigned int generateTexture3D(int w, int h, int d);
    void bindTexture2D(unsigned int tex, int unit = 0);
} // NAMESPACE chif::Graphics