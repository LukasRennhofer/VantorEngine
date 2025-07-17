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
 *  File: VRD_Shader.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "../../Math/Linear/VMA_Matrix.hpp"
#include "../../Math/Linear/VMA_Matrix.hpp"
#include "../../Math/Linear/VMA_Vector.hpp"

namespace Vantor::RenderDevice
{
    class VShader
    {
        public:
            virtual ~VShader() = default;

            virtual void Use() const = 0;

            // Uniform Functions
            virtual void setUniformBool(const std::string &name, bool value) const                          = 0;
            virtual void setUniformInt(const std::string &name, int value) const                            = 0;
            virtual void setUniformFloat(const std::string &name, float value) const                        = 0;
            virtual void setUniformVec2(const std::string &name, const Vantor::Math::VVector2 &value) const = 0;
            virtual void setUniformVec2(const std::string &name, float x, float y) const                    = 0;
            virtual void setUniformVec3(const std::string &name, const Vantor::Math::VVector3 &value) const = 0;
            virtual void setUniformVec3(const std::string &name, float x, float y, float z) const           = 0;
            virtual void setUniformVec4(const std::string &name, const Vantor::Math::VVector4 &value) const = 0;
            virtual void setUniformVec4(const std::string &name, float x, float y, float z, float w) const  = 0;
            virtual void setUniformMat2(const std::string &name, const Vantor::Math::VMat2 &mat) const      = 0;
            virtual void setUniformMat3(const std::string &name, const Vantor::Math::VMat3 &mat) const      = 0;
            virtual void setUniformMat4(const std::string &name, const Vantor::Math::VMat4 &mat) const      = 0;

            // For the CommandBuffer
            virtual unsigned int GetShaderID() const = 0;
    };
} // namespace Vantor::RenderDevice