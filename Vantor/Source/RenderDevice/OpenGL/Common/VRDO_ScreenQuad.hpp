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
 *  File: VRDO_ScreenQuad.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include <Shared/glad/glad.h>

#include "../../../Resource/VRES_Manager.hpp"
#include "../../Interface/Common/VRD_ScreenQuad.hpp"

#include <Shared/glad/glad.h>

namespace Vantor::RenderDevice
{
    // NOTE (LUKAS): The Texture Binding Unit is always 1!
    class VOpenGLScreenQuad : public VScreenQuad
    {
        public:
            VOpenGLScreenQuad();
            ~VOpenGLScreenQuad() override;

            void Draw() override;
            void DrawRaw() override;

        private:
            // OpenGL Objects
            GLuint m_VAO = 0;
            GLuint m_VBO = 0;

            float quadVertices[]; // Todo: make to resizeable vector
    };
} // namespace Vantor::RenderDevice