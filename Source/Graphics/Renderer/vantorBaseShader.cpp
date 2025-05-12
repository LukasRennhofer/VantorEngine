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
 *  File: vantorBaseShader.cpp
 *  Last Change: Automatically updated
 */

#include "vantorBaseShader.h"
#include <iostream>
#include <fstream>
#include <sstream>

namespace vantor::Graphics::Renderer::Shader
{
    bool checkCompileErrors(unsigned int shader, const std::string &type, const std::string &shaderName)
    {
        int  success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);

                std::string errorMessage = "Shader: \"" + shaderName + "\" Compilation Error of type: " + type;
                vantor::Backlog::Log("Shader", errorMessage, vantor::Backlog::LogLevel::ERR);
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::string errorMessage = "Programming linking error of type: " + type;
                vantor::Backlog::Log("Shader", errorMessage, vantor::Backlog::LogLevel::ERR);
            }
        }
        return success;
    }

    std::string getShaderName(const char *path)
    {
        std::string  pathstr        = std::string(path);
        const size_t last_slash_idx = pathstr.find_last_of("/");
        if (std::string::npos != last_slash_idx)
        {
            pathstr.erase(0, last_slash_idx + 1);
        }
        return pathstr;
    }

    shaderType getShaderType(const char *path)
    {
        std::string  type           = getShaderName(path);
        const size_t last_slash_idx = type.find_last_of(".");
        if (std::string::npos != last_slash_idx)
        {
            type.erase(0, last_slash_idx + 1);
        }
        if (type == "vert") return shaderType(GL_VERTEX_SHADER, "VERTEX");
        if (type == "frag") return shaderType(GL_FRAGMENT_SHADER, "FRAGMENT");
        if (type == "tes") return shaderType(GL_TESS_EVALUATION_SHADER, "TESS_EVALUATION");
        if (type == "tcs") return shaderType(GL_TESS_CONTROL_SHADER, "TESS_CONTROL");
        if (type == "geom") return shaderType(GL_GEOMETRY_SHADER, "GEOMETRY");
        if (type == "comp") return shaderType(GL_COMPUTE_SHADER, "COMPUTE");
    }

    BaseShader::BaseShader(const char *shaderPath)
    {
        path                     = std::string(shaderPath);
        std::string shaderCode   = loadShaderFromFile(shaderPath);
        const char *shaderString = shaderCode.c_str();

        shadType = getShaderType(shaderPath);
        shad     = glCreateShader(shadType.type);
        glShaderSource(shad, 1, &shaderString, NULL);
        glCompileShader(shad);
        checkCompileErrors(shad, shadType.name.c_str(), getShaderName(shaderPath));
    }

    std::string BaseShader::loadShaderFromFile(const char *shaderPath)
    {
        std::string   shaderCode;
        std::ifstream shaderFile;
        shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            shaderFile.open(shaderPath);
            std::stringstream shaderStream;
            shaderStream << shaderFile.rdbuf();
            shaderFile.close();
            shaderCode = shaderStream.str();
        }
        catch (std::ifstream::failure e)
        {
            std::string errorMessage = "Shader: \"" + getShaderName(shaderPath) + "\" not succesfully read";
            vantor::Backlog::Log("Shader", errorMessage, vantor::Backlog::LogLevel::ERR);
        }
        return shaderCode;
    }

    BaseShader::~BaseShader()
    {
        // glDeleteShader(shad);
    }

    unsigned int BaseShader::getShad() { return shad; }
} // NAMESPACE vantor::Graphics::Renderer::Shader
