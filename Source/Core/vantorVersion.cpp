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
 *  Date: 2025-05-11
 *
 *  File: vantorVersion.cpp
 *  Last Change: Automatically updated
 */

#include "vantorVersion.h"

/**
 * * @brief Versioning system for Vantor
 * * @details The versioning system follows the Semantic Versioning (SemVer)
 * guidelines.
 * * Will be moved in the future to vantorApplication.cpp / .h
 */
namespace vantor::Core::version
{
    // Main Core
    const int major = 0;
    // minor features, major updates
    const int minor = 16;
    // minor bug fixes, alterations, refactors, updates
    const int revision = 14;
    // Version String
    const std::string version_string = std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(revision);

    int         GetMajor() { return major; }
    int         GetMinor() { return minor; }
    int         GetRevision() { return revision; }
    std::string GetVersionString() { return version_string; }
} // namespace vantor::Core::version
