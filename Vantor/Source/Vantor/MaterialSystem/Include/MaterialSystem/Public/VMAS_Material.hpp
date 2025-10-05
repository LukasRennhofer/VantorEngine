/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

#include <map>

#include <Core/Types/VCO_Color.hpp>

#include <RHI/Interface/VRHI_Shader.hpp>
#include <RHI/Interface/VRHI_Texture.hpp>

#include <MaterialSystem/Public/VMAS_Uniform.hpp>

/*
    VMaterial represents the complete render state configuration necessary for rendering a mesh.
    This includes shader programs, texture bindings, sampler states, and any relevant GPU state.

    Each mesh rendered in the scene must be associated with a material. VMaterial instances manage
    the link between shader resources and their required parameters.

    The renderer maintains a library of default materials and templates that can be used as a base
    for creating customized materials specific to an entity or render pass.
*/

namespace VE
{
    // Material Types
    enum class EMaterialType
    {
        MaterialDefault,
        MaterialPostProcess, // TODO
        MaterialPBR,
        MaterialCustom       // TODO
    };

    class VMaterial
    {
        private:
            VE::Internal::RHI::IRHIShader              *m_Shader; // Base Shader program
            std::map<std::string, VUniformValue>        m_Uniforms;
            std::map<std::string, VUniformValueSampler> m_SamplerUniforms;

        public:
            EMaterialType              Type      = EMaterialType::MaterialDefault;

            // Fallbacks
            // These Fallbacks will be applied in the Shader, if VTextureDiffuse
            // or VSpecularDiffuse are not given
            VE::Internal::Core::Types::VColor Color     = VE::Internal::Core::Types::VColor::Gray(); // Standart Color is gray
            float SpecularStrength = 0.5; // Standart is 0.5 (so in the middle)

            // TODO: Implement States, to represent the sate of a single Mesh
            // shadow state
            // bool ShadowCast    = true;
            // bool ShadowReceive = true;

        private:
        public:
            VMaterial();
            VMaterial(VE::Internal::RHI::IRHIShader *shader);

            VE::Internal::RHI::IRHIShader *GetShader();
            void                     SetShader(VE::Internal::RHI::IRHIShader *shader);

            VMaterial Copy();

            bool IsDefaultMaterial() {return Type == EMaterialType::MaterialDefault; }
            bool IsPBRMaterial() {return Type == EMaterialType::MaterialPBR; }

            void SetBool(std::string name, bool value);
            void SetInt(std::string name, int value);
            void SetFloat(std::string name, float value);
            void SetTexture(std::string                     name,
                            VE::Internal::RHI::IRHITexture *value,
                            unsigned int                    unit   = 0,
                            EUniformType                   target = EUniformType::UniformTypeSAMPLER2D, ESamplerType type = ESamplerType::SamplerDiffuse);

            void SetVector(std::string name, VE::Math::VVector2 value);
            void SetVector(std::string name, VE::Math::VVector3 value);
            void SetVector(std::string name, VE::Math::VVector4 value);
            void SetMatrix(std::string name, VE::Math::VMat2 value);
            void SetMatrix(std::string name, VE::Math::VMat3 value);
            void SetMatrix(std::string name, VE::Math::VMat4 value);

            // TODO: make the names SetVector3, etc.
            std::map<std::string, VUniformValue>        *GetUniforms();
            std::map<std::string, VUniformValueSampler> *GetSamplerUniforms();
    };
} // namespace VE