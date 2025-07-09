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
 *  Date: 2025-07-09
 *
 *  File: VCO_FSUtils.hpp
 *  Last Change: Automatically updated
 */

#pragma once

// Platform
#include "../../Platform/VPL_Environment.hpp"

#include <string>

namespace Vantor::Core::IO
{
    // Split Path in dir and fileName and directly modify them
    void SplitPath(const std::string &fullPath, std::string &dir, std::string &fileName);
    // Get filename from given Path
    std::string GetFileNameFromPath(const std::string &fullPath);
    // Get directory from given Path
    std::string GetDirectoryFromPath(const std::string &fullPath);
    // Get extension from given filename
    std::string GetExtensionFromFileName(const std::string &filename);
    // Replace extension from given filename
    std::string ReplaceExtension(const std::string &filename, const std::string &extension);
    // Force Extension to filename
    std::string ForceExtension(const std::string &filename, const std::string &extension);
    // Remove extension from given filename
    std::string RemoveExtension(const std::string &filename);

    // Abstracted functions
    inline bool FileExists(const std::string &path) { return Vantor::Platform::Environment::FileExists(path); }
} // namespace Vantor::Core::IO