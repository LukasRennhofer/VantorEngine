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
 *  File: VRE_Material.cpp
 *  Last Change: Automatically updated
 */

#include "VRE_Material.hpp"

namespace Vantor::Renderer
{
    VMaterial::VMaterial() {}

    VMaterial::VMaterial(Vantor::RenderDevice::VShader *shader) { m_Shader = shader; }

    Vantor::RenderDevice::VShader *VMaterial::GetShader() { return m_Shader; }

    void VMaterial::SetShader(Vantor::RenderDevice::VShader *shader) { m_Shader = shader; }

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
        m_Uniforms[name].Type = UniformTypeBOOL;
        m_Uniforms[name].Bool = value;
    }

    void VMaterial::SetInt(std::string name, int value)
    {
        m_Uniforms[name].Type = UniformTypeINT;
        m_Uniforms[name].Int  = value;
    }

    void VMaterial::SetFloat(std::string name, float value)
    {
        m_Uniforms[name].Type  = UniformTypeFLOAT;
        m_Uniforms[name].Float = value;
    }

    void VMaterial::SetTexture(std::string name, Vantor::RenderDevice::VTexture *value, unsigned int unit, VEUniformType target, VESamplerType type)
    {
        m_SamplerUniforms[name].Unit    = unit;
        m_SamplerUniforms[name].Texture = value;

        m_SamplerUniforms[name].Type = target;
        m_SamplerUniforms[name].SType = type;

        if (m_Shader)
        {
            m_Shader->Use();
            m_Shader->setUniformInt(name, unit);
        }
    }

    void VMaterial::SetTextureCube(std::string name, Vantor::RenderDevice::VTextureCube *value, unsigned int unit)
    {
        m_SamplerUniforms[name].Unit        = unit;
        m_SamplerUniforms[name].Type        = UniformTypeSAMPLERCUBE;
        m_SamplerUniforms[name].TextureCube = value;

        if (m_Shader)
        {
            m_Shader->Use();
            m_Shader->setUniformInt(name, unit);
        }
    }

    void VMaterial::SetVector(std::string name, Vantor::Math::VVector2 value)
    {
        m_Uniforms[name].Type = UniformTypeVEC2;
        m_Uniforms[name].Vec2 = value;
    }

    void VMaterial::SetVector(std::string name, Vantor::Math::VVector3 value)
    {
        m_Uniforms[name].Type = UniformTypeVEC3;
        m_Uniforms[name].Vec3 = value;
    }

    void VMaterial::SetVector(std::string name, Vantor::Math::VVector4 value)
    {
        m_Uniforms[name].Type = UniformTypeVEC4;
        m_Uniforms[name].Vec4 = value;
    }

    void VMaterial::SetMatrix(std::string name, Vantor::Math::VMat2 value)
    {
        m_Uniforms[name].Type = UniformTypeMAT2;
        m_Uniforms[name].Mat2 = value;
    }

    void VMaterial::SetMatrix(std::string name, Vantor::Math::VMat3 value)
    {
        m_Uniforms[name].Type = UniformTypeMAT3;
        m_Uniforms[name].Mat3 = value;
    }

    void VMaterial::SetMatrix(std::string name, Vantor::Math::VMat4 value)
    {
        m_Uniforms[name].Type = UniformTypeMAT4;
        m_Uniforms[name].Mat4 = value;
    }

    std::map<std::string, VUniformValue> *VMaterial::GetUniforms() { return &m_Uniforms; }

    std::map<std::string, VUniformValueSampler> *VMaterial::GetSamplerUniforms() { return &m_SamplerUniforms; }
} // namespace Vantor::Renderer