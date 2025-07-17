/*
 *  ╔═══════════════════════════════════════════════════════════════╗
 *  ║                          ~ Vantor ~                           ║
 *  ║                                                               ║
 *  ║  This file is part of the Vantor Engine.                      ║
 *  ║  Automatically formatted by vtrgFormat.py                     ║
 *  ║                                                               ║
 *  ╚═══════════════════════════════════════════════════════════════╝
 *
 *  Copyright (c) 2025 Lukas Rennhofer
 *  Licensed under the GNU General Public License, Version 3.
 *  See LICENSE file for more details.
 *
 *  Author: Lukas Rennhofer
 *  Date: 2025-07-16
 *
 *  File: VRDO_Shader.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include <Shared/glad/glad.h>
#include <string>

#include "../../Math/Linear/VMA_Matrix.hpp"
#include "../../Math/Linear/VMA_Vector.hpp"
#include "../Interface/Compiler/VRD_GLSLComp.hpp"
#include "../Interface/VRD_Shader.hpp"
#include "../Interface/VRD_Shader.hpp"

#include "../Interface/Compiler/VRD_GLSLComp.hpp"

namespace Vantor::RenderDevice
{

    class VOpenGLShader : public VShader
    {
        public:
            // Lang for OpenGL still GLSL
            // TODO: Overload functions to seperate vertex and fragment
            VOpenGLShader(const char *vertexCode, const char *fragmentCode, const char *fileNameVertex, const char *fileNameFragment);
            ~VOpenGLShader() override;

            void Use() const override;

            // Uniform Functions
            void setUniformBool(const std::string &name, bool value) const override;
            void setUniformInt(const std::string &name, int value) const override;
            void setUniformFloat(const std::string &name, float value) const override;
            void setUniformVec2(const std::string &name, const Vantor::Math::VVector2 &value) const override;
            void setUniformVec2(const std::string &name, float x, float y) const override;
            void setUniformVec3(const std::string &name, const Vantor::Math::VVector3 &value) const override;
            void setUniformVec3(const std::string &name, float x, float y, float z) const override;
            void setUniformVec4(const std::string &name, const Vantor::Math::VVector4 &value) const override;
            void setUniformVec4(const std::string &name, float x, float y, float z, float w) const override;
            void setUniformMat2(const std::string &name, const Vantor::Math::VMat2 &mat) const override;
            void setUniformMat3(const std::string &name, const Vantor::Math::VMat3 &mat) const override;
            void setUniformMat4(const std::string &name, const Vantor::Math::VMat4 &mat) const override;

            // For the CommandBuffer
            unsigned int GetShaderID() const override;

        private:
            GLuint ID;
            GLuint compileShader(GLenum type, const std::string &source);
            GLuint linkProgram(GLuint vertexShader, GLuint fragmentShader);
            GLint  getUniformLocation(const std::string &name) const;
    };

} // namespace Vantor::RenderDevice