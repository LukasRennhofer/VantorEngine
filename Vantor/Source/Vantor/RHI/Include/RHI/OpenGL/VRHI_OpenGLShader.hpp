/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

#include <RHI/Interface/VRHI_Shader.hpp>

#include <Shared/glad/glad.h>
#include <string>

namespace Vantor::RHI
{

class OpenGLShader : public IRHIShader
{
public:
    OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource);
    virtual ~OpenGLShader();

    // IRHIShader implementation
    void Use() override;

    // Uniform Setters
    void SetBool(const std::string& name, bool value) override;
    void SetInt(const std::string& name, int value) override;
    void SetFloat(const std::string& name, float value) override;

    void SetVec2(const std::string& name, float x, float y) override;
    void SetVec2(const std::string& name, const Vantor::Math::VVector2 &value) override;
    void SetVec3(const std::string& name, float x, float y, float z) override;
    void SetVec3(const std::string& name, const Vantor::Math::VVector3 &value) override;
    void SetVec4(const std::string& name, float x, float y, float z, float w) override;
    void SetVec4(const std::string& name, const Vantor::Math::VVector4 &value) override;

    void SetMat2(const std::string& name, const Vantor::Math::VMat2 &mat) override;
    void SetMat3(const std::string& name, const Vantor::Math::VMat3 &mat) override;
    void SetMat4(const std::string& name, const Vantor::Math::VMat4 &mat) override;

private:
    uint32_t m_program;

    uint32_t CompileShader(const std::string& source, GLenum type);
    bool CheckCompileErrors(uint32_t shader, const std::string& type);
    bool CheckLinkErrors(uint32_t program);
    int GetUniformLocation(const std::string& name);
};

} // namespace Vantor::RHI
