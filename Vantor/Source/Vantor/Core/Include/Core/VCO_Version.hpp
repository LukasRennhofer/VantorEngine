/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

#include <string>

namespace VE::Internal::Core::Version
{
    // return: int major
    int GetMajor();
    // return: int minor
    int GetMinor();
    // return: int revision
    int GetRevision();
    // Return the Version in the format "<major>.<minor>.<revision>"
    std::string GetVersionString();
} // namespace VE::Internal::Core::Version