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
 *  File: VMA_Common.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include <cmath>

namespace Vantor::Math
{
    constexpr float PI  = 3.14159265358979323846f;
    constexpr float TAU = 6.28318530717958647692f; // TAU = 2 * PI

    // Convert degrees to radians
    constexpr float DegToRad(float degrees) { return degrees * PI / 180.0f; }

    // Convert radians to degrees
    constexpr float RadToDeg(float radians) { return radians * 180.0f / PI; }

    // Rounds up a given value to the nearest multiple of a specified alignment
    template <typename T> constexpr T align(T value, T alignment) { return ((value + alignment - T(1)) / alignment) * alignment; }
} // namespace Vantor::Math