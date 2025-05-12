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
 *  File: vantorBaseShader.h
 *  Last Change: Automatically updated
 */

#pragma once
#include <string>
#include <glad/glad.h>

#include "../../Core/Backlog/vantorBacklog.h"

namespace vantor::Graphics::Renderer::Shader
{
    struct shaderType
    {
            shaderType() : type(-1), name("") {}
            shaderType(unsigned int type, std::string name) : type(type), name(name) {}

            unsigned int type;
            std::string  name;
    };

    bool        checkCompileErrors(unsigned int shader, const std::string &type, const std::string &shaderName);
    std::string getShaderName(const char *shaderPath);
    shaderType  getShaderType(const char *path);

    class BaseShader
    {
        public:
            BaseShader(const char *shaderPath);
            virtual ~BaseShader();

            std::string getName() { return getShaderName(path.c_str()); }

            shaderType   getType();
            unsigned int getShad();

        private:
            std::string  loadShaderFromFile(const char *shaderPath);
            std::string  path;
            unsigned int shad;
            shaderType   shadType;
    };
} // NAMESPACE vantor::Graphics::Renderer::Shader
