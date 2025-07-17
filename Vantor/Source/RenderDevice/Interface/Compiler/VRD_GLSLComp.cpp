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
 *  File: VRD_GLSLComp.cpp
 *  Last Change: Automatically updated
 */

#include "VRD_GLSLComp.hpp"

namespace Vantor::RenderDevice
{
    VGLSLPreproccessor::VGLSLPreproccessor()
    {
        // Initialize Vantor Include Path
        vglsl_add_virtual_include_path("Vantor", "Shaders/Private");
        m_VirtualIncludes.push_back(std::string("Vantor"));
    }

    VGLSLPreproccessor::~VGLSLPreproccessor()
    {
        // Clear Virtual Include Paths
        vglsl_clear_virtual_include_paths();

        m_VirtualIncludes.clear();
    }

    const char *VGLSLPreproccessor::ProccessFromMemory(const char *memory, const char *fileName)
    {
        // Parse a shader file with includes
        VglslResult result = vglsl_parse_memory(memory, fileName);

        const char *output;

        if (!result.success)
        {
            Vantor::Backlog::Log("VRDevice::VGLSLPreproccessor", "Error in GLSL Preproccessor Compilation.", Vantor::Backlog::LogLevel::ERR);
            // std::cout << "  " << result.error_message << std::endl;
            // std::cout << "  In file: \"" << result.error_file << "\" Line: " << result.error_line << std::endl;
        }

        output = result.output ? strdup(result.output) : nullptr;

        // std::cout << "==========================" << std::endl;
        // std::cout << output << std::endl;
        // std::cout << "==========================" << std::endl;

        // Free the result data
        vglsl_free_result(&result);

        return output;
    }
} // namespace Vantor::RenderDevice