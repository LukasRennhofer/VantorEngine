/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

#include <cmath>

namespace VE::Math
{
    constexpr float VPI  = 3.14159265358979323846f;
    constexpr float VTAU = 6.28318530717958647692f; // TAU = 2 * PI

    // Convert degrees to radians
    constexpr float DegToRad(float degrees) { return degrees * VPI / 180.0f; }

    // Convert radians to degrees
    constexpr float RadToDeg(float radians) { return radians * 180.0f / VPI; }

    // Rounds up a given value to the nearest multiple of a specified alignment
    template <typename T> constexpr T align(T value, T alignment) { return ((value + alignment - T(1)) / alignment) * alignment; }
} // namespace VE::Internal::Math