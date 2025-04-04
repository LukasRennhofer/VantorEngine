
/*
 *    				~ CHIFEngine ~
 *
 * Copyright (c) 2025 Lukas Rennhofer
 *
 * Licensed under the MIT License. See LICENSE file for more details.
 *
 * Author: Lukas Rennhofer
 * Date: 2025-04-01
 *
 * File: chifFontUtils.cpp
 * Last Change: Added Dear Imgui Light theme by Pacôme Danhiez and added Demo Profiler
*/

#include "chifFontUtils.h"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>

FontUtils::FontUtils() {}


FontUtils::~FontUtils() {}

void FontUtils::loadShader(char* shaderSource, GLenum shaderType, GLuint &programId) {
    GLuint shaderId = glCreateShader(shaderType);

    GLint result = GL_FALSE; // compilation result
    int infoLogLength; // length of info log

    std::ifstream shaderFile(shaderSource);
    std::string shaderStr;
    const char* shader;
    
    if(!shaderFile.is_open()) {
        std::string error = "Error: could not read file ";
        chif::Backlog::Log("Font", error, chif::Backlog::LogLevel::ERR);
    }

    // Read shader
    std::string buffer;
    while(std::getline(shaderFile, buffer)) {
        shaderStr += buffer + "\n";
    }

    shader = shaderStr.c_str();

    glShaderSource(shaderId,        // Shader handle
        1,               // Number of files
        &shader,  // Shader source code
        NULL);           // NULL terminated string
    glCompileShader(shaderId);

    // Check shader
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
    std::vector<char> errorMessage(std::max(infoLogLength, int(1)));
    glGetShaderInfoLog(shaderId, infoLogLength, NULL, &errorMessage[0]);
    glAttachShader(programId, shaderId);
    glLinkProgram(programId);

    // Check the program
    glGetProgramiv(programId, GL_LINK_STATUS, &result);
    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);
    std::vector<char> programErrorMessage(std::max(infoLogLength, int(1)));
    glGetProgramInfoLog(programId, infoLogLength, NULL, &programErrorMessage[0]);

    glDeleteShader(shaderId);

    shaderFile.close();
}