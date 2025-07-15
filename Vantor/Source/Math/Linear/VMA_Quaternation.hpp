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
 *  Date: 2025-07-15
 *
 *  File: VMA_Quaternion.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include "VMA_Vector.hpp"
#include "VMA_Matrix.hpp"

#include "../VMA_Common.hpp"

#include <cmath>

namespace Vantor::Math
{
    struct VQuaternion
    {
        float x = 0, y = 0, z = 0, w = 1;

        VQuaternion(float x_, float y_, float z_, float w_) noexcept
        : x(x_), y(y_), z(z_), w(w_) {}

        // Identity quaternion
        static VQuaternion Identity() noexcept
        {
            return VQuaternion{0, 0, 0, 1};
        }

        // Constructor from axis-angle (degrees)
        static VQuaternion FromAxisAngle(const VVector3 &axis, float degrees) noexcept
        {
            float rad   = degrees * PI / 180.f;
            float half  = rad * 0.5f;
            float sinH  = std::sin(half);
            VVector3 n  = axis.Normalized();
            return VQuaternion{n.x * sinH, n.y * sinH, n.z * sinH, std::cos(half)};
        }

        // Normalize
        VQuaternion Normalized() const noexcept
        {
            float mag = std::sqrt(x * x + y * y + z * z + w * w);
            return VQuaternion{x / mag, y / mag, z / mag, w / mag};
        }

        // Conjugate
        VQuaternion Conjugated() const noexcept
        {
            return VQuaternion{-x, -y, -z, w};
        }

        // Inverse
        VQuaternion Inverted() const noexcept
        {
            float normSq = x * x + y * y + z * z + w * w;
            return Conjugated() * (1.0f / normSq);
        }

        // Quaternion multiplication
        VQuaternion operator*(const VQuaternion &rhs) const noexcept
        {
            return VQuaternion{
                w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
                w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x,
                w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w,
                w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z};
        }

        // Scale quaternion
        VQuaternion operator*(float scalar) const noexcept
        {
            return VQuaternion{x * scalar, y * scalar, z * scalar, w * scalar};
        }

        // Apply quaternion rotation to a vector
        VVector3 Rotate(const VVector3 &v) const noexcept
        {
            VQuaternion qv{v.x, v.y, v.z, 0};
            VQuaternion result = (*this) * qv * Inverted();
            return {result.x, result.y, result.z};
        }

        // Convert to 3x3 matrix
        VMat3 ToMat3() const noexcept
        {
            float xx = x * x, yy = y * y, zz = z * z;
            float xy = x * y, xz = x * z, yz = y * z;
            float wx = w * x, wy = w * y, wz = w * z;

            VMat3 mat;
            mat.m[0] = 1 - 2 * (yy + zz);
            mat.m[1] = 2 * (xy - wz);
            mat.m[2] = 2 * (xz + wy);

            mat.m[3] = 2 * (xy + wz);
            mat.m[4] = 1 - 2 * (xx + zz);
            mat.m[5] = 2 * (yz - wx);

            mat.m[6] = 2 * (xz - wy);
            mat.m[7] = 2 * (yz + wx);
            mat.m[8] = 1 - 2 * (xx + yy);
            return mat;
        }

        // Convert to 4x4 matrix
        VMat4 ToMat4() const noexcept
        {
            VMat3 rot = ToMat3();
            VMat4 mat = VMat4::Identity();

            mat.m[0]  = rot.m[0];
            mat.m[1]  = rot.m[1];
            mat.m[2]  = rot.m[2];

            mat.m[4]  = rot.m[3];
            mat.m[5]  = rot.m[4];
            mat.m[6]  = rot.m[5];

            mat.m[8]  = rot.m[6];
            mat.m[9]  = rot.m[7];
            mat.m[10] = rot.m[8];

            return mat;
        }
    };
} // namespace Vantor::Math
