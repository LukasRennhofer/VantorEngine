/****************************************************************************
 * VE Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), VE Studios™
 * Copyright : © 2025 Lukas Rennhofer, VE Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#include <RenderPipeline/Pipeline/VREP_Internal.hpp>
#include <RenderPipeline/Public/VREP_RenderPath.hpp>
#include <RenderPipeline/Pipeline/VREP_CommandBuffer.hpp>

#include <RHI/Interface/VRHI_Device.hpp>
#include <RHI/Interface/VRHI_RenderTarget.hpp>
#include <RHI/Interface/VRHI_Mesh.hpp>

#include <iostream>

namespace VE::Internal::RenderPipeline {

    VGeometryRenderPass::VGeometryRenderPass() = default;

    void VGeometryRenderPass::Execute()
    {
        // TODO
    }

    void VGeometryRenderPass::Cleanup()
    {
        // TODO
    }

    VForwardRenderPass::VForwardRenderPass() = default;

    void VForwardRenderPass::Execute()
    {
          // Go through all Meshes
        auto commands = m_RenderPath->GetCommandBuffer()->GetForwardRenderCommands();
        
        for (auto command : commands)
        {
            command.Material->GetShader()->Use();

            // Should be the same in every Shader
            command.Material->GetShader()->SetMat4("uModel", command.Transform);
            command.Material->GetShader()->SetMat4("uView", m_RenderPath->GetCamera()->View);
            command.Material->GetShader()->SetMat4("uProj",  m_RenderPath->GetCamera()->Projection);

            // Handle Textures of Material
            auto *samplers = command.Material->GetSamplerUniforms();
            for (auto it = samplers->begin(); it != samplers->end(); ++it)
            {
                // if (it->second.Type == VE::EUniformType::UniformTypeSAMPLERCUBE)
                // {
                    // it->second.TextureCube->Bind(it->second.Unit);
                // }
                // else
                if (it->second.Type == VE::EUniformType::UniformTypeSAMPLER2D) {
                    it->second.Texture->Bind(it->second.Unit);
                }
            }

            // Set all Uniforms of Material to its Shader
            auto *uniforms = command.Material->GetUniforms();
            for (auto it = uniforms->begin(); it != uniforms->end(); ++it)
            {

                switch (it->second.Type)
                {
                    case VE::EUniformType::UniformTypeBOOL:
                        command.Material->GetShader()->SetBool(it->first, it->second.Bool);
                        break;
                    case VE::EUniformType::UniformTypeINT:
                        command.Material->GetShader()->SetInt(it->first, it->second.Int);
                        break;
                    case VE::EUniformType::UniformTypeFLOAT:
                        command.Material->GetShader()->SetFloat(it->first, it->second.Float);
                        break;
                    case VE::EUniformType::UniformTypeVEC2:
                        command.Material->GetShader()->SetVec2(it->first, it->second.Vec2);
                        break;
                    case VE::EUniformType::UniformTypeVEC3:
                        command.Material->GetShader()->SetVec3(it->first, it->second.Vec3);
                        break;
                    case VE::EUniformType::UniformTypeVEC4:
                        command.Material->GetShader()->SetVec4(it->first, it->second.Vec4);
                        break;
                    case VE::EUniformType::UniformTypeMAT2:
                        command.Material->GetShader()->SetMat2(it->first, it->second.Mat2);
                        break;
                    case VE::EUniformType::UniformTypeMAT3:
                        command.Material->GetShader()->SetMat3(it->first, it->second.Mat3);
                        break;
                    case VE::EUniformType::UniformTypeMAT4:
                        command.Material->GetShader()->SetMat4(it->first, it->second.Mat4);
                        break;
                    default:
                        break;
                }
            }

            command.Model->Draw();
        }
    }

    void VForwardRenderPass::Cleanup()
    {
        // TODO
    }
}