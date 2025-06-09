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
 *  File: VRD_Shader.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <Shared/glm/glm.hpp>

namespace Vantor::RenderDevice
{
    class VShader
    {
        public:
            virtual ~VShader() = default;

            virtual void use() const = 0;

            // Uniform Functions
            virtual void setUniformBool(const std::string &name, bool value) const                  = 0;
            virtual void setUniformInt(const std::string &name, int value) const                    = 0;
            virtual void setUniformFloat(const std::string &name, float value) const                = 0;
            virtual void setVec2(const std::string &name, const glm::vec2 &value) const             = 0;
            virtual void setVec2(const std::string &name, float x, float y) const                   = 0;
            virtual void setVec3(const std::string &name, const glm::vec3 &value) const             = 0;
            virtual void setVec3(const std::string &name, float x, float y, float z) const          = 0;
            virtual void setVec4(const std::string &name, const glm::vec4 &value) const             = 0;
            virtual void setVec4(const std::string &name, float x, float y, float z, float w) const = 0;
            virtual void setMat2(const std::string &name, const glm::mat2 &mat) const               = 0;
            virtual void setMat3(const std::string &name, const glm::mat3 &mat) const               = 0;
            virtual void setMat4(const std::string &name, const glm::mat4 &mat) const               = 0;
    };
} // namespace Vantor::RenderDevice