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
 *  File: vantorOpenGLChache.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace vantor::Graphics::RenderDevice::OpenGL
{
    class GLCache
    {
        private:
            bool m_DepthTest;
            bool m_Blend;
            bool m_CullFace;

            GLenum m_DepthFunc;
            GLenum m_BlendSrc;
            GLenum m_BlendDst;
            GLenum m_FrontFace;
            GLenum m_PolygonMode;

            unsigned int m_ActiveShaderID;

        public:
            GLCache();
            ~GLCache();

            void SetDepthTest(bool enable);
            void SetDepthFunc(GLenum depthFunc);
            void SetBlend(bool enable);
            void SetBlendFunc(GLenum src, GLenum dst);
            void SetCull(bool enable);
            void SetCullFace(GLenum face);
            void SetPolygonMode(GLenum mode);

            void SwitchShader(unsigned int ID);
    };
} // namespace vantor::Graphics::RenderDevice::OpenGL