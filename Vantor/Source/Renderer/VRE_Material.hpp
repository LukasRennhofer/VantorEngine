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
 *  File: VRE_Material.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include <map>

#include "../Core/Types/VCO_Color.hpp"
#include "../RenderDevice/Interface/VRD_Shader.hpp"
#include "../RenderDevice/Interface/VRD_Shader.hpp"
#include "../RenderDevice/Interface/VRD_Texture.hpp"
#include "../RenderDevice/Interface/VRD_Texture.hpp"
#include "VRE_Uniform.hpp"
#include "VRE_Uniform.hpp"
// Uniforms
#include "VRE_Uniform.hpp"

/*
    VMaterial represents the complete render state configuration necessary for rendering a mesh.
    This includes shader programs, texture bindings, sampler states, and any relevant GPU state.

    Each mesh rendered in the scene must be associated with a material. VMaterial instances manage
    the link between shader resources and their required parameters.

    The renderer maintains a library of default materials and templates that can be used as a base
    for creating customized materials specific to an entity or render pass.
*/

namespace Vantor::Renderer
{
    // Material Types
    enum class VEMaterialType
    {
        MaterialDefault,
        MaterialPostProcess, // TODO
        MaterialCustom       // TODO
    };

    class VMaterial
    {
        private:
            Vantor::RenderDevice::VShader              *m_Shader; // Base Shader program
            std::map<std::string, VUniformValue>        m_Uniforms;
            std::map<std::string, VUniformValueSampler> m_SamplerUniforms;

        public:
            VEMaterialType              Type      = VEMaterialType::MaterialDefault;

            // Fallbacks
            // These Fallbacks will be applied in the Shader, if VTextureDiffuse
            // or VSpecularDiffuse are not given
            Vantor::Core::Types::VColor Color     = Vantor::Core::Types::VColor::Gray(); // Standart Color is gray
            float SpecularStrength = 0.5; // Standart is 0.5 (so in the middle)

            // TODO: Implement States, to represent the sate of a single Mesh
            // shadow state
            // bool ShadowCast    = true;
            // bool ShadowReceive = true;

        private:
        public:
            VMaterial();
            VMaterial(Vantor::RenderDevice::VShader *shader);

            Vantor::RenderDevice::VShader *GetShader();
            void                           SetShader(Vantor::RenderDevice::VShader *shader);

            VMaterial Copy();

            void SetBool(std::string name, bool value);
            void SetInt(std::string name, int value);
            void SetFloat(std::string name, float value);
            void SetTexture(std::string                     name,
                            Vantor::RenderDevice::VTexture *value,
                            unsigned int                    unit   = 0,
                            VEUniformType                   target = VEUniformType::UniformTypeSAMPLER2D, VESamplerType type = VESamplerType::SamplerDiffuse);
            void SetTextureCube(std::string name, Vantor::RenderDevice::VTextureCube *value, unsigned int unit = 0);
            void SetVector(std::string name, Vantor::Math::VVector2 value);
            void SetVector(std::string name, Vantor::Math::VVector3 value);
            void SetVector(std::string name, Vantor::Math::VVector4 value);
            void SetMatrix(std::string name, Vantor::Math::VMat2 value);
            void SetMatrix(std::string name, Vantor::Math::VMat3 value);
            void SetMatrix(std::string name, Vantor::Math::VMat4 value);

            std::map<std::string, VUniformValue>        *GetUniforms();
            std::map<std::string, VUniformValueSampler> *GetSamplerUniforms();
    };
} // namespace Vantor::Renderer