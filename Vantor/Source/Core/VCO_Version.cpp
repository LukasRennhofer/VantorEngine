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
 *  Date: 2025-06-09
 *
 *  File: VCO_Version.cpp
 *  Last Change: Automatically updated
 */

#include "VCO_Version.hpp"

namespace Vantor::Core::Version
{
    // Main Core
    const int major = 0;
    // minor features, major updates
    const int minor = 17;
    // minor bug fixes, alterations, refactors, updates
    const int revision = 1;
    // Version String
    const std::string version_string = std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(revision);

    int         GetMajor() { return major; }
    int         GetMinor() { return minor; }
    int         GetRevision() { return revision; }
    std::string GetVersionString() { return version_string; }
} // namespace Vantor::Core::version
