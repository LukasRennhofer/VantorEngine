/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

#include <string>
#include <memory>
#include <cstdint>

#include <Math/Linear/VMA_Vector.hpp>
#include <Math/Linear/VMA_Matrix.hpp>

namespace VE::Internal::RHI
{

class IRHIShader
{
public:
    virtual ~IRHIShader() = default;

    virtual void Use() = 0;

    // Uniform Setters
    virtual void SetBool(const std::string& name, bool value) = 0;
    virtual void SetInt(const std::string& name, int value) = 0;
    virtual void SetFloat(const std::string& name, float value) = 0;

    virtual void SetVec2(const std::string& name, float x, float y) = 0;
    virtual void SetVec2(const std::string& name, const VE::Math::VVector2 &value) = 0;
    virtual void SetVec3(const std::string& name, float x, float y, float z) = 0;
    virtual void SetVec3(const std::string& name, const VE::Math::VVector3 &value) = 0;
    virtual void SetVec4(const std::string& name, float x, float y, float z, float w) = 0;
    virtual void SetVec4(const std::string& name, const VE::Math::VVector4 &value) = 0;

    virtual void SetMat2(const std::string& name, const VE::Math::VMat2 &mat) = 0;
    virtual void SetMat3(const std::string& name, const VE::Math::VMat3 &mat) = 0;
    virtual void SetMat4(const std::string& name, const VE::Math::VMat4 &mat) = 0;

    // // OpenGL: GLuint ID
    // // Vulkan: VkShaderModule*
    // virtual void* GetHandle() const = 0;
};

} // namespace VE::Internal::RHI
