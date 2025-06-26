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
 *  Date: 2025-06-26
 *
 *  File: VMA_Common.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include <cmath>

namespace Vantor::Math
{
    // Convert degrees to radians
    constexpr float DegToRad(float degrees) { return degrees * 3.14159265358979323846f / 180.0f; }

    // Convert radians to degrees
    constexpr float RadToDeg(float radians) { return radians * 180.0f / 3.14159265358979323846f; }
} // namespace Vantor::Math