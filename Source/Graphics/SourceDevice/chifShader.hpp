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
 * File: chifShader.hpp
 * Last Change: 
*/

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

// Enum to define the shader types (e.g., vertex, fragment, compute)
enum class shaderType { Vertex = 0, Fragment, Compute, Undefined };

namespace chif::Graphics::SourceDevice {

    // ========== Base Shader ========
    class BaseShader
    {
    public:
        BaseShader(const char *shaderPath)
        virtual ~BaseShader();

        std::string getName() const;
        shaderType getType() const;
        unsigned int getShad() const;

    protected:
        std::string loadShaderFromFile(const char *shaderPath);
        std::string getShaderName(const char *shaderPath);

    private:
        std::string path;           // Path to the shader file
        unsigned int shad;          // Shader ID (e.g., OpenGL shader ID)
        shaderType shadType;        // Shader type (vertex, fragment, etc.)
    };

} // namespace chif::Graphics::sourceDevice