/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#include <Core/VCO_Version.hpp>

namespace VE::Internal::Core::Version
{
    // Main Core
    const int major = 0;
    // minor features, major updates
    const int minor = 20;
    // minor bug fixes, alterations, refactors, updates
    const int revision = 9;
    // Version String
    const std::string version_string = std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(revision);

    int         GetMajor() { return major; }
    int         GetMinor() { return minor; }
    int         GetRevision() { return revision; }
    std::string GetVersionString() { return version_string; }
} // namespace VE::Internal::Core::Version