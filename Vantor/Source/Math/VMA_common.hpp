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
 *  File: VMA_common.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include "LinearAlgebra/VMA_Vector.hpp"

#include <algorithm> // For std::min/max

namespace Vantor::Math
{

    namespace Constants
    {
        // --- Scalar constants ---
        constexpr float PI         = 3.14159265358979323846f;
        constexpr float TAU        = 6.28318530717958647692f; // 2 * PI
        constexpr float HALF_PI    = 1.57079632679489661923f;
        constexpr float QUARTER_PI = 0.78539816339744830961f;

        constexpr float DEG2RAD = PI / 180.0f;
        constexpr float RAD2DEG = 180.0f / PI;

        constexpr float EPSILON   = 1e-6f; // For float comparisons
        constexpr float FLOAT_MAX = 3.4028235e+38f;
        constexpr float FLOAT_MIN = 1.1754944e-38f;

        // --- Integer limits (optional if you're not using std::numeric_limits) ---
        constexpr int INT_MAX = 2147483647;
        constexpr int INT_MIN = (-INT_MAX - 1);

        // --- Utility values ---
        constexpr float ONE_OVER_SQRT2 = 0.70710678118f;
        constexpr float GOLDEN_RATIO   = 1.61803398875f;
    } // namespace Constants

    // ----- Scalar functions -----
    inline float Clamp(float val, float minVal, float maxVal) { return std::max(minVal, std::min(val, maxVal)); }

    inline float Lerp(float a, float b, float t) { return a + (b - a) * t; }

    inline float Radians(float degrees) { return degrees * Constants::DEG2RAD; }

    inline float Degrees(float radians) { return radians * Constants::RAD2DEG; }

    // ----- Vector functions -----

    inline Vec3 Lerp(const Vec3 &a, const Vec3 &b, float t) { return a + (b - a) * t; }

    inline Vec4 Lerp(const Vec4 &a, const Vec4 &b, float t)
    {
        return Vec4(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t, a.w + (b.w - a.w) * t);
    }
} // namespace Vantor::Math