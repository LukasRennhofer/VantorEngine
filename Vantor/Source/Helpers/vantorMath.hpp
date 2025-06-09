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
 *  File: vantorMath.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include <cmath>
#include <cstdint>
#include <algorithm>

namespace Vantor::Helpers::Math
{
    constexpr float PI      = 3.14159265359f;
    constexpr float DEG2RAD = PI / 180.0f;
    constexpr float RAD2DEG = 180.0f / PI;

    // === Vector 2D ===
    struct Vec2
    {
            float x = 0, y = 0;

            constexpr Vec2() = default;
            constexpr Vec2(float x, float y) : x(x), y(y) {}

            Vec2 operator+(const Vec2 &v) const { return {x + v.x, y + v.y}; }
            Vec2 operator-(const Vec2 &v) const { return {x - v.x, y - v.y}; }
            Vec2 operator*(float s) const { return {x * s, y * s}; }
            Vec2 operator/(float s) const { return {x / s, y / s}; }

            float length() const { return std::sqrt(x * x + y * y); }
            Vec2  normalized() const
            {
                float len = length();
                return len != 0 ? *this / len : Vec2();
            }
    };

    // === Vector 3D ===
    struct Vec3
    {
            float x = 0, y = 0, z = 0;

            constexpr Vec3() = default;
            constexpr Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

            Vec3 operator+(const Vec3 &v) const { return {x + v.x, y + v.y, z + v.z}; }
            Vec3 operator-(const Vec3 &v) const { return {x - v.x, y - v.y, z - v.z}; }
            Vec3 operator*(float s) const { return {x * s, y * s, z * s}; }
            Vec3 operator/(float s) const { return {x / s, y / s, z / s}; }

            float length() const { return std::sqrt(x * x + y * y + z * z); }
            Vec3  normalized() const
            {
                float len = length();
                return len != 0 ? *this / len : Vec3();
            }

            static Vec3 cross(const Vec3 &a, const Vec3 &b) { return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x}; }

            static float dot(const Vec3 &a, const Vec3 &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
    };

    // === Vector 4D ===
    struct Vec4
    {
            float x = 0, y = 0, z = 0, w = 0;

            constexpr Vec4() = default;
            constexpr Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    };

    // === 4x4 Matrix ===
    struct Mat4
    {
            float m[4][4]{};

            static Mat4 identity()
            {
                Mat4 mat;
                for (int i = 0; i < 4; ++i)
                    mat.m[i][i] = 1.0f;
                return mat;
            }

            static Mat4 translation(const Vec3 &t)
            {
                Mat4 mat    = identity();
                mat.m[0][3] = t.x;
                mat.m[1][3] = t.y;
                mat.m[2][3] = t.z;
                return mat;
            }

            static Mat4 scale(const Vec3 &s)
            {
                Mat4 mat    = {};
                mat.m[0][0] = s.x;
                mat.m[1][1] = s.y;
                mat.m[2][2] = s.z;
                mat.m[3][3] = 1.0f;
                return mat;
            }

            // Rotation around Z axis (2D rotation)
            static Mat4 rotationZ(float angleRad)
            {
                Mat4  mat   = identity();
                float c     = std::cos(angleRad);
                float s     = std::sin(angleRad);
                mat.m[0][0] = c;
                mat.m[0][1] = -s;
                mat.m[1][0] = s;
                mat.m[1][1] = c;
                return mat;
            }
    };

    // === Math Utils ===
    inline float Clamp(float val, float minVal, float maxVal) { return std::max(minVal, std::min(val, maxVal)); }

    inline float Lerp(float a, float b, float t) { return a + (b - a) * t; }

    inline Vec3 Lerp(const Vec3 &a, const Vec3 &b, float t) { return a + (b - a) * t; }

    inline Vec4 Lerp(const Vec4 &a, const Vec4 &b, float t)
    {
        return Vec4(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t, a.w + (b.w - a.w) * t);
    }

    inline float Radians(float degrees) { return degrees * DEG2RAD; }

    inline float Degrees(float radians) { return radians * RAD2DEG; }
} // namespace Vantor::Helpers::Math
