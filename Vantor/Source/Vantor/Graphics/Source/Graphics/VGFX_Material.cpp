/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#include <Graphics/VGFX_Material.hpp>

namespace Vantor::Graphics
{
    VMaterial::VMaterial() {}

    VMaterial::VMaterial(Vantor::RHI::IRHIShader *shader) { m_Shader = shader; }

    Vantor::RHI::IRHIShader *VMaterial::GetShader() { return m_Shader; }

    void VMaterial::SetShader(Vantor::RHI::IRHIShader *shader) { m_Shader = shader; }

    VMaterial VMaterial::Copy()
    {
        VMaterial copy(m_Shader);

        copy.Type = Type;

        // Fallbacks
        copy.Color = Color;
        copy.SpecularStrength = SpecularStrength;

        copy.m_Uniforms        = m_Uniforms;
        copy.m_SamplerUniforms = m_SamplerUniforms;

        return copy;
    }

    void VMaterial::SetBool(std::string name, bool value)
    {
        m_Uniforms[name].Type = EUniformType::UniformTypeBOOL;
        m_Uniforms[name].Bool = value;
    }

    void VMaterial::SetInt(std::string name, int value)
    {
        m_Uniforms[name].Type = EUniformType::UniformTypeINT;
        m_Uniforms[name].Int  = value;
    }

    void VMaterial::SetFloat(std::string name, float value)
    {
        m_Uniforms[name].Type  = EUniformType::UniformTypeFLOAT;
        m_Uniforms[name].Float = value;
    }

    void VMaterial::SetTexture(std::string name, Vantor::RHI::IRHITexture *value, unsigned int unit, EUniformType target, ESamplerType type)
    {
        m_SamplerUniforms[name].Unit    = unit;
        m_SamplerUniforms[name].Texture = value;

        m_SamplerUniforms[name].Type = target;
        m_SamplerUniforms[name].SType = type;

        if (m_Shader)
        {
            m_Shader->Use();
            m_Shader->SetInt(name, unit);
        }
    }

    // void VMaterial::SetTextureCube(std::string name, Vantor::RenderDevice::VTextureCube *value, unsigned int unit)
    // {
    //     m_SamplerUniforms[name].Unit        = unit;
    //     m_SamplerUniforms[name].Type        = UniformTypeSAMPLERCUBE;
    //     m_SamplerUniforms[name].TextureCube = value;

    //     if (m_Shader)
    //     {
    //         m_Shader->Use();
    //         m_Shader->setUniformInt(name, unit);
    //     }
    // }

    void VMaterial::SetVector(std::string name, Vantor::Math::VVector2 value)
    {
        m_Uniforms[name].Type = EUniformType::UniformTypeVEC2;
        m_Uniforms[name].Vec2 = value;
    }

    void VMaterial::SetVector(std::string name, Vantor::Math::VVector3 value)
    {
        m_Uniforms[name].Type = EUniformType::UniformTypeVEC3;
        m_Uniforms[name].Vec3 = value;
    }

    void VMaterial::SetVector(std::string name, Vantor::Math::VVector4 value)
    {
        m_Uniforms[name].Type = EUniformType::UniformTypeVEC4;
        m_Uniforms[name].Vec4 = value;
    }

    void VMaterial::SetMatrix(std::string name, Vantor::Math::VMat2 value)
    {
        m_Uniforms[name].Type = EUniformType::UniformTypeMAT2;
        m_Uniforms[name].Mat2 = value;
    }

    void VMaterial::SetMatrix(std::string name, Vantor::Math::VMat3 value)
    {
        m_Uniforms[name].Type = EUniformType::UniformTypeMAT3;
        m_Uniforms[name].Mat3 = value;
    }

    void VMaterial::SetMatrix(std::string name, Vantor::Math::VMat4 value)
    {
        m_Uniforms[name].Type = EUniformType::UniformTypeMAT4;
        m_Uniforms[name].Mat4 = value;
    }

    std::map<std::string, VUniformValue> *VMaterial::GetUniforms() { return &m_Uniforms; }

    std::map<std::string, VUniformValueSampler> *VMaterial::GetSamplerUniforms() { return &m_SamplerUniforms; }
} // namespace Vantor::Graphics