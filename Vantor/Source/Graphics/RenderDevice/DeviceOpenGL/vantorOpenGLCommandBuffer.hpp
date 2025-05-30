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
 *  Date: 2025-05-12
 *
 *  File: vantorOpenGLCommandBuffer.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>
#include <map>

namespace vantor::Graphics::RenderDevice::OpenGL
{

    class Mesh;
    class Material;
    class Renderer;
    class RenderTarget;

    /*

      NOTE: All render state required for submitting a render command.

    */
    struct RenderCommand
    {
            glm::mat4 Transform;
            glm::mat4 PrevTransform;
            Mesh     *Mesh;
            Material *Material;
            glm::vec3 BoxMin;
            glm::vec3 BoxMax;
    };

    class CommandBuffer
    {
        public:
        private:
            Renderer *m_Renderer;

            std::vector<RenderCommand>                           m_DeferredRenderCommands;
            std::vector<RenderCommand>                           m_AlphaRenderCommands;
            std::vector<RenderCommand>                           m_PostProcessingRenderCommands;
            std::map<RenderTarget *, std::vector<RenderCommand>> m_CustomRenderCommands;

        public:
            CommandBuffer(Renderer *renderer);
            ~CommandBuffer();

            void Push(Mesh         *mesh,
                      Material     *material,
                      glm::mat4     transform     = glm::mat4(),
                      glm::mat4     prevTransform = glm::mat4(),
                      glm::vec3     boxMin        = glm::vec3(-99999.0f),
                      glm::vec3     boxMax        = glm::vec3(99999.0f),
                      RenderTarget *target        = nullptr);

            void Clear();
            void Sort();

            std::vector<RenderCommand> GetDeferredRenderCommands(bool cull = false);
            std::vector<RenderCommand> GetAlphaRenderCommands(bool cull = false);
            std::vector<RenderCommand> GetCustomRenderCommands(RenderTarget *target, bool cull = false);
            std::vector<RenderCommand> GetPostProcessingRenderCommands();
            std::vector<RenderCommand> GetShadowCastRenderCommands();
    };
} // namespace vantor::Graphics::RenderDevice::OpenGL