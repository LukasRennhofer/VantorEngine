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
 *  File: VRD_ScreenQuad.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include <memory>

#include "../VRD_Shader.hpp"

namespace Vantor::RenderDevice
{
    class VScreenQuad
    {
        public:
            VScreenQuad() = default;
            virtual ~VScreenQuad();

            virtual void Draw()    = 0;
            virtual void DrawRaw() = 0;

        protected:
            std::shared_ptr<VShader> m_ShaderScreenQuad;
    };

} // namespace Vantor::RenderDevice