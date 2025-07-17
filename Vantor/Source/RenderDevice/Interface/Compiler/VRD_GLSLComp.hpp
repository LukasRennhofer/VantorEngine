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
 *  File: VRD_GLSLComp.hpp
 *  Last Change: Automatically updated
 */

#pragma once

// We use vglsl as our preprocessor
#include <Shared/VGLSL/vglsl.h>
#include <string>
#include <vector>

#include "../../../Core/BackLog/VCO_Backlog.hpp" // Backlog
#include "../../../Core/Types/VCO_ClassTypes.hpp" // VSingleton
#include <vector>
#include <string>

namespace Vantor::RenderDevice
{
    class VGLSLPreproccessor : public Vantor::Core::Types::VSingleton<VGLSLPreproccessor>
    {
            friend class Vantor::Core::Types::VSingleton<VGLSLPreproccessor>;

        private:
            std::vector<std::string> m_VirtualIncludes;

        public:
            // Private constructor/destructor
            VGLSLPreproccessor();
            ~VGLSLPreproccessor();

            const char *ProccessFromMemory(const char *memory, const char *fileName);

            std::vector<std::string> GetVirtualIncludes() { return m_VirtualIncludes; }
    };
} // namespace Vantor::RenderDevice