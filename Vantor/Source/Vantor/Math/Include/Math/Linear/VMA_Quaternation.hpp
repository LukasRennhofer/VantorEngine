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

#include "../VMA_Common.hpp"
#include "VMA_Matrix.hpp"
#include "VMA_Vector.hpp"
#include <cmath>

namespace VE::Math
{
    struct VQuaternion
    {
            float x = 0, y = 0, z = 0, w = 1;

            VQuaternion(float x_, float y_, float z_, float w_) noexcept : x(x_), y(y_), z(z_), w(w_) {}

            // Identity quaternion
            static VQuaternion Identity() noexcept { return VQuaternion{0, 0, 0, 1}; }

            // Constructor from axis-angle (degrees)
            static VQuaternion FromAxisAngle(const VVector3 &axis, float degrees) noexcept
            {
                float    rad  = degrees * VPI / 180.f;
                float    half = rad * 0.5f;
                float    sinH = std::sin(half);
                VVector3 n    = axis.Normalized();
                return VQuaternion{n.x * sinH, n.y * sinH, n.z * sinH, std::cos(half)};
            }

            // Normalize
            VQuaternion Normalized() const noexcept
            {
                float mag = std::sqrt(x * x + y * y + z * z + w * w);
                return VQuaternion{x / mag, y / mag, z / mag, w / mag};
            }

            // Conjugate
            VQuaternion Conjugated() const noexcept { return VQuaternion{-x, -y, -z, w}; }

            // Inverse
            VQuaternion Inverted() const noexcept
            {
                float normSq = x * x + y * y + z * z + w * w;
                return Conjugated() * (1.0f / normSq);
            }

            // Quaternion multiplication
            VQuaternion operator*(const VQuaternion &rhs) const noexcept
            {
                return VQuaternion{w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y, w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x,
                                   w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w, w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z};
            }

            // Scale quaternion
            VQuaternion operator*(float scalar) const noexcept { return VQuaternion{x * scalar, y * scalar, z * scalar, w * scalar}; }

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

                mat.m[0] = rot.m[0];
                mat.m[1] = rot.m[1];
                mat.m[2] = rot.m[2];

                mat.m[4] = rot.m[3];
                mat.m[5] = rot.m[4];
                mat.m[6] = rot.m[5];

                mat.m[8]  = rot.m[6];
                mat.m[9]  = rot.m[7];
                mat.m[10] = rot.m[8];

                return mat;
            }

            // Construct from Euler angles (in degrees): Vpitch (X), yaw (Y), roll (Z)
            static VQuaternion FromEulerAngles(const VVector3 &eulerDegrees) noexcept
            {
                float Vpitch = eulerDegrees.x * VPI / 180.0f;
                float yaw   = eulerDegrees.y * VPI / 180.0f;
                float roll  = eulerDegrees.z * VPI / 180.0f;

                float cy = std::cos(yaw * 0.5f);
                float sy = std::sin(yaw * 0.5f);
                float cp = std::cos(Vpitch * 0.5f);
                float sp = std::sin(Vpitch * 0.5f);
                float cr = std::cos(roll * 0.5f);
                float sr = std::sin(roll * 0.5f);

                VQuaternion q(0, 0, 0, 0);
                q.w = cr * cp * cy + sr * sp * sy;
                q.x = sr * cp * cy - cr * sp * sy;
                q.y = cr * sp * cy + sr * cp * sy;
                q.z = cr * cp * sy - sr * sp * cy;
                return q;
            }

            // Spherical linear interpolation
            VQuaternion Slerp(const VQuaternion& other, float t) const noexcept
            {
                // Compute the cosine of the angle between the two vectors
                float dot = x * other.x + y * other.y + z * other.z + w * other.w;
                
                // If the dot product is negative, slerp won't take the shorter path
                // So negate one quaternion to correct this
                VQuaternion q2 = other;
                if (dot < 0.0f) {
                    q2 = VQuaternion(-other.x, -other.y, -other.z, -other.w);
                    dot = -dot;
                }
                
                // If the quaternions are very close, use linear interpolation
                if (dot > 0.9995f) {
                    VQuaternion result(
                        x + t * (q2.x - x),
                        y + t * (q2.y - y),
                        z + t * (q2.z - z),
                        w + t * (q2.w - w)
                    );
                    return result.Normalized();
                }
                
                // Calculate the angle between the quaternions
                float theta_0 = std::acos(std::abs(dot));
                float theta = theta_0 * t;
                float sin_theta = std::sin(theta);
                float sin_theta_0 = std::sin(theta_0);
                
                float s0 = std::cos(theta) - dot * sin_theta / sin_theta_0;
                float s1 = sin_theta / sin_theta_0;
                
                return VQuaternion(
                    s0 * x + s1 * q2.x,
                    s0 * y + s1 * q2.y,
                    s0 * z + s1 * q2.z,
                    s0 * w + s1 * q2.w
                );
            }

    };
} // namespace VE::Internal::Math