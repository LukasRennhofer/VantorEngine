/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

#include <array>
#include <cassert>
#include <cmath>

namespace VE::Math
{

    // ----------------- VVector2 -----------------
    struct VVector2
    {
            float x = 0.0f, y = 0.0f;

            // Constructors
            constexpr VVector2() noexcept = default;
            constexpr VVector2(float _x, float _y) noexcept : x(_x), y(_y) {}

            // Operators
            constexpr VVector2 operator+(const VVector2 &rhs) const noexcept { return {x + rhs.x, y + rhs.y}; }
            constexpr VVector2 operator-(const VVector2 &rhs) const noexcept { return {x - rhs.x, y - rhs.y}; }
            constexpr VVector2 operator*(float scalar) const noexcept { return {x * scalar, y * scalar}; }
            constexpr VVector2 operator/(float scalar) const noexcept { return {x / scalar, y / scalar}; }

            VVector2 &operator+=(const VVector2 &rhs) noexcept
            {
                x += rhs.x;
                y += rhs.y;
                return *this;
            }
            VVector2 &operator-=(const VVector2 &rhs) noexcept
            {
                x -= rhs.x;
                y -= rhs.y;
                return *this;
            }
            VVector2 &operator*=(float scalar) noexcept
            {
                x *= scalar;
                y *= scalar;
                return *this;
            }
            VVector2 &operator/=(float scalar) noexcept
            {
                x /= scalar;
                y /= scalar;
                return *this;
            }

            // Utilities
            constexpr float Dot(const VVector2 &rhs) const noexcept { return x * rhs.x + y * rhs.y; }
            float           length() const noexcept { return std::sqrt(x * x + y * y); }
            float           lengthSquared() const noexcept { return x * x + y * y; }

            VVector2 Normalized() const noexcept
            {
                float len = length();
                return len > 0 ? (*this) / len : VVector2{};
            }

            void Normalize() noexcept
            {
                float len = length();
                if (len > 0)
                {
                    x /= len;
                    y /= len;
                }
            }

            constexpr const float *Data() const noexcept { return &x; }
            float                 *Data() noexcept { return &x; }
    };

    // Scalar * vector operator
    inline VVector2 operator*(float scalar, const VVector2 &vec) noexcept { return vec * scalar; }

    // ----------------- VVector3 -----------------
    struct VVector3
    {
            float x = 0.0f, y = 0.0f, z = 0.0f;

            constexpr VVector3() noexcept = default;
            constexpr VVector3(float _x, float _y, float _z) noexcept : x(_x), y(_y), z(_z) {}

            constexpr VVector3 operator+(const VVector3 &rhs) const noexcept { return {x + rhs.x, y + rhs.y, z + rhs.z}; }
            constexpr VVector3 operator-(const VVector3 &rhs) const noexcept { return {x - rhs.x, y - rhs.y, z - rhs.z}; }
            constexpr VVector3 operator*(float scalar) const noexcept { return {x * scalar, y * scalar, z * scalar}; }
            constexpr VVector3 operator/(float scalar) const noexcept { return {x / scalar, y / scalar, z / scalar}; }

            VVector3 &operator+=(const VVector3 &rhs) noexcept
            {
                x += rhs.x;
                y += rhs.y;
                z += rhs.z;
                return *this;
            }
            VVector3 &operator-=(const VVector3 &rhs) noexcept
            {
                x -= rhs.x;
                y -= rhs.y;
                z -= rhs.z;
                return *this;
            }
            VVector3 &operator*=(float scalar) noexcept
            {
                x *= scalar;
                y *= scalar;
                z *= scalar;
                return *this;
            }
            VVector3 &operator/=(float scalar) noexcept
            {
                x /= scalar;
                y /= scalar;
                z /= scalar;
                return *this;
            }

            constexpr float Dot(const VVector3 &rhs) const noexcept { return x * rhs.x + y * rhs.y + z * rhs.z; }

            VVector3 Cross(const VVector3 &rhs) const noexcept { return {y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x}; }

            float length() const noexcept { return std::sqrt(x * x + y * y + z * z); }
            float lengthSquared() const noexcept { return x * x + y * y + z * z; }

            VVector3 Normalized() const noexcept
            {
                float len = length();
                return len > 0 ? (*this) / len : VVector3{};
            }

            void Normalize() noexcept
            {
                float len = length();
                if (len > 0)
                {
                    x /= len;
                    y /= len;
                    z /= len;
                }
            }

            constexpr const float *Data() const noexcept { return &x; }
            float                 *Data() noexcept { return &x; }
    };

    inline VVector3 operator*(float scalar, const VVector3 &vec) noexcept { return vec * scalar; }

    // ----------------- VVector4 -----------------
    struct VVector4
    {
            float x = 0.0f, y = 0.0f, z = 0.0f, w = 0.0f;

            constexpr VVector4() noexcept = default;
            constexpr VVector4(float _x, float _y, float _z, float _w) noexcept : x(_x), y(_y), z(_z), w(_w) {}

            constexpr VVector4 operator+(const VVector4 &rhs) const noexcept { return {x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w}; }
            constexpr VVector4 operator-(const VVector4 &rhs) const noexcept { return {x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w}; }
            constexpr VVector4 operator*(float scalar) const noexcept { return {x * scalar, y * scalar, z * scalar, w * scalar}; }
            constexpr VVector4 operator/(float scalar) const noexcept { return {x / scalar, y / scalar, z / scalar, w / scalar}; }

            VVector4 &operator+=(const VVector4 &rhs) noexcept
            {
                x += rhs.x;
                y += rhs.y;
                z += rhs.z;
                w += rhs.w;
                return *this;
            }
            VVector4 &operator-=(const VVector4 &rhs) noexcept
            {
                x -= rhs.x;
                y -= rhs.y;
                z -= rhs.z;
                w -= rhs.w;
                return *this;
            }
            VVector4 &operator*=(float scalar) noexcept
            {
                x *= scalar;
                y *= scalar;
                z *= scalar;
                w *= scalar;
                return *this;
            }
            VVector4 &operator/=(float scalar) noexcept
            {
                x /= scalar;
                y /= scalar;
                z /= scalar;
                w /= scalar;
                return *this;
            }

            constexpr float Dot(const VVector4 &rhs) const noexcept { return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w; }

            float length() const noexcept { return std::sqrt(x * x + y * y + z * z + w * w); }
            float lengthSquared() const noexcept { return x * x + y * y + z * z + w * w; }

            VVector4 Normalized() const noexcept
            {
                float len = length();
                return len > 0 ? (*this) / len : VVector4{};
            }

            void Normalize() noexcept
            {
                float len = length();
                if (len > 0)
                {
                    x /= len;
                    y /= len;
                    z /= len;
                    w /= len;
                }
            }

            constexpr const float *Data() const noexcept { return &x; }
            float                 *Data() noexcept { return &x; }
    };

    inline VVector4 operator*(float scalar, const VVector4 &vec) noexcept { return vec * scalar; }

    // Clamp scalar
    inline float Clamp(float value, float minVal, float maxVal) noexcept { return std::fmax(minVal, std::fmin(maxVal, value)); }

    // Clamp01 scalar
    inline float Clamp01(float value) noexcept { return Clamp(value, 0.0f, 1.0f); }

    // Lerp scalar
    inline float Lerp(float a, float b, float t) noexcept { return a + (b - a) * Clamp01(t); }

    // --------- VVector2 Utils ---------
    inline VVector2 Clamp(const VVector2 &v, const VVector2 &min, const VVector2 &max) noexcept { return {Clamp(v.x, min.x, max.x), Clamp(v.y, min.y, max.y)}; }

    inline VVector2 Clamp01(const VVector2 &v) noexcept { return Clamp(v, VVector2(0.0f, 0.0f), VVector2(1.0f, 1.0f)); }

    inline VVector2 Lerp(const VVector2 &a, const VVector2 &b, float t) noexcept { return a + (b - a) * Clamp01(t); }

    // --------- VVector3 Utils ---------
    inline VVector3 Clamp(const VVector3 &v, const VVector3 &min, const VVector3 &max) noexcept
    {
        return {Clamp(v.x, min.x, max.x), Clamp(v.y, min.y, max.y), Clamp(v.z, min.z, max.z)};
    }

    inline VVector3 Clamp01(const VVector3 &v) noexcept { return Clamp(v, VVector3(0.0f, 0.0f, 0.0f), VVector3(1.0f, 1.0f, 1.0f)); }

    inline VVector3 Lerp(const VVector3 &a, const VVector3 &b, float t) noexcept { return a + (b - a) * Clamp01(t); }

    // --------- VVector4 Utils ---------
    inline VVector4 Clamp(const VVector4 &v, const VVector4 &min, const VVector4 &max) noexcept
    {
        return {Clamp(v.x, min.x, max.x), Clamp(v.y, min.y, max.y), Clamp(v.z, min.z, max.z), Clamp(v.w, min.w, max.w)};
    }

    inline VVector4 Clamp01(const VVector4 &v) noexcept { return Clamp(v, VVector4(0.0f, 0.0f, 0.0f, 0.0f), VVector4(1.0f, 1.0f, 1.0f, 1.0f)); }

    inline VVector4 Lerp(const VVector4 &a, const VVector4 &b, float t) noexcept { return a + (b - a) * Clamp01(t); }

} // namespace VE::Internal::Math