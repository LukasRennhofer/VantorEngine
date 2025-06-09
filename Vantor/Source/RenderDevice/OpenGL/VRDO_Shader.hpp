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
 *  Date: 2025-06-08
 *
 *  File: VRDO_Shader.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include <Shared/glad/glad.h>
#include <Shared/glm/glm.hpp>
#include <string>
#include "../Interface/VRD_Shader.hpp"

namespace Vantor::RenderDevice
{

    class VOpenGLShader : public VShader
    {
        public:
            // Lang for OpenGL still GLSL
            // TODO: Overload functions to seperate vertex and fragment
            VOpenGLShader(const char *vertexCode, const char *fragmentCode);
            ~VOpenGLShader() override;

            void use() const override;

            // Uniform Functions
            void setUniformBool(const std::string &name, bool value) const override;
            void setUniformInt(const std::string &name, int value) const override;
            void setUniformFloat(const std::string &name, float value) const override;
            void setVec2(const std::string &name, const glm::vec2 &value) const override;
            void setVec2(const std::string &name, float x, float y) const override;
            void setVec3(const std::string &name, const glm::vec3 &value) const override;
            void setVec3(const std::string &name, float x, float y, float z) const override;
            void setVec4(const std::string &name, const glm::vec4 &value) const override;
            void setVec4(const std::string &name, float x, float y, float z, float w) const override;
            void setMat2(const std::string &name, const glm::mat2 &mat) const override;
            void setMat3(const std::string &name, const glm::mat3 &mat) const override;
            void setMat4(const std::string &name, const glm::mat4 &mat) const override;

        private:
            GLuint      ID;
            GLuint      compileShader(GLenum type, const std::string &source);
            GLuint      linkProgram(GLuint vertexShader, GLuint fragmentShader);
            GLint       getUniformLocation(const std::string &name) const;
    };

} // namespace Vantor::RenderDevice
