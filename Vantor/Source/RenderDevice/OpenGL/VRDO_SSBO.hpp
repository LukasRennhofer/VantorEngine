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
 *  File: VRDO_SSBO.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include <Shared/glad/glad.h>
#include <vector>

#include "../../Renderer/Light/VRE_LightData.hpp"
#include "../../Renderer/Light/VRE_LightData.hpp"

namespace Vantor::RenderDevice
{
    template <typename T> class VOpenGLSSBO
    {
        public:
            VOpenGLSSBO() { glGenBuffers(1, &m_SSBO); }

            virtual ~VOpenGLSSBO()
            {
                if (m_SSBO) glDeleteBuffers(1, &m_SSBO);
            }

            virtual void Update(const std::vector<T> &data)
            {
                glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SSBO);
                size_t newSize = data.size() * sizeof(T);

                if (newSize > m_CurrentSize)
                {
                    glBufferData(GL_SHADER_STORAGE_BUFFER, newSize, data.data(), GL_DYNAMIC_DRAW);
                    m_CurrentSize = newSize;
                }
                else
                {
                    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, newSize, data.data());
                }

                glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
            }

            void Bind(GLuint bindingPoint = 1) const { glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPoint, m_SSBO); }

        protected:
            GLuint m_SSBO        = 0;
            size_t m_CurrentSize = 0; // in bytes
    };
} // namespace Vantor::RenderDevice