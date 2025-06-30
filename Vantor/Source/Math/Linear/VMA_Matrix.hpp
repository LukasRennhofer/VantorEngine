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
 *  Date: 2025-06-30
 *
 *  File: VMA_Matrix.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include "VMA_Vector.hpp"

#include <array>
#include <cassert>

namespace Vantor::Math
{

    // ----------------- VMat4 -----------------
    struct VMat4
    {

            std::array<float, 16> m{};

            // Identity matrix
            static constexpr VMat4 Identity() noexcept
            {
                VMat4 mat{};
                mat.m[0]  = 1.f;
                mat.m[5]  = 1.f;
                mat.m[10] = 1.f;
                mat.m[15] = 1.f;
                return mat;
            }

            // Multiply two matrices
            VMat4 operator*(const VMat4 &rhs) const noexcept
            {
                VMat4 result{};
                for (int row = 0; row < 4; ++row)
                {
                    for (int col = 0; col < 4; ++col)
                    {
                        float sum = 0.f;
                        for (int i = 0; i < 4; ++i)
                        {
                            sum += m[row * 4 + i] * rhs.m[i * 4 + col];
                        }
                        result.m[row * 4 + col] = sum;
                    }
                }
                return result;
            }

            // Create translation matrix
            static VMat4 Translate(const VVector3 &translation) noexcept
            {
                VMat4 mat = Identity();
                mat.m[12] = translation.x;
                mat.m[13] = translation.y;
                mat.m[14] = translation.z;
                return mat;
            }

            // Create rotation matrix around Y (yaw)
            static VMat4 RotationYaw(float degrees) noexcept
            {
                float rad = degrees * 3.14159265358979323846f / 180.f;
                float c   = std::cos(rad);
                float s   = std::sin(rad);

                VMat4 mat = Identity();
                mat.m[0]  = c;
                mat.m[2]  = s;
                mat.m[8]  = -s;
                mat.m[10] = c;
                return mat;
            }

            // Create rotation matrix around X (pitch)
            static VMat4 RotationPitch(float degrees) noexcept
            {
                float rad = degrees * 3.14159265358979323846f / 180.f;
                float c   = std::cos(rad);
                float s   = std::sin(rad);

                VMat4 mat = Identity();
                mat.m[5]  = c;
                mat.m[6]  = -s;
                mat.m[9]  = s;
                mat.m[10] = c;
                return mat;
            }

            // Combine yaw + pitch rotations (yaw first)
            static VMat4 RotationYawPitch(float yawDegrees, float pitchDegrees) noexcept { return RotationYaw(yawDegrees) * RotationPitch(pitchDegrees); }

            // Create lookAt matrix (right-handed)
            static VMat4 LookAt(const VVector3 &eye, const VVector3 &center, const VVector3 &up) noexcept
            {
                VVector3 f = (center - eye).Normalized();
                VVector3 s = f.Cross(up).Normalized();
                VVector3 u = s.Cross(f);

                VMat4 mat = Identity();
                mat.m[0]  = s.x;
                mat.m[1]  = u.x;
                mat.m[2]  = -f.x;
                mat.m[4]  = s.y;
                mat.m[5]  = u.y;
                mat.m[6]  = -f.y;
                mat.m[8]  = s.z;
                mat.m[9]  = u.z;
                mat.m[10] = -f.z;

                mat.m[12] = -s.Dot(eye);
                mat.m[13] = -u.Dot(eye);
                mat.m[14] = f.Dot(eye);
                return mat;
            }

            static VMat4 Perspective(float fovDegrees, float aspectRatio, float nearPlane, float farPlane) noexcept
            {
                float fovRad = fovDegrees * 3.14159265358979323846f / 180.f;
                float f      = 1.f / std::tan(fovRad / 2.f);

                VMat4 mat{};
                mat.m[0]  = f / aspectRatio;                                 // scale X
                mat.m[5]  = f;                                               // scale Y
                mat.m[10] = (farPlane + nearPlane) / (nearPlane - farPlane); // Z scaling
                mat.m[11] = -1.f;                                            // perspective divide
                mat.m[14] = (2.f * farPlane * nearPlane) / (nearPlane - farPlane);
                mat.m[15] = 0.f;

                return mat;
            }

            static VMat4 Orthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane) noexcept
            {
                VMat4 mat{};

                float rl = right - left;
                float tb = top - bottom;
                float fn = farPlane - nearPlane;

                assert(rl != 0.f && tb != 0.f && fn != 0.f);

                mat.m[0]  = 2.f / rl;
                mat.m[5]  = 2.f / tb;
                mat.m[10] = -2.f / fn;

                mat.m[12] = -(right + left) / rl;
                mat.m[13] = -(top + bottom) / tb;
                mat.m[14] = -(farPlane + nearPlane) / fn;

                mat.m[15] = 1.f;

                return mat;
            }

            constexpr const float *Data() const noexcept { return m.data(); }
            float                 *Data() noexcept { return m.data(); }
    };

    // ----------------- VMat3 -----------------
    struct VMat3
    {
            std::array<float, 9> m{}; // 3x3 matrix stored in row-major order

            // Identity matrix
            static constexpr VMat3 Identity() noexcept
            {
                VMat3 mat{};
                mat.m[0] = 1.f;
                mat.m[4] = 1.f;
                mat.m[8] = 1.f;
                return mat;
            }

            // Multiply two matrices
            VMat3 operator*(const VMat3 &rhs) const noexcept
            {
                VMat3 result{};
                for (int row = 0; row < 3; ++row)
                {
                    for (int col = 0; col < 3; ++col)
                    {
                        float sum = 0.f;
                        for (int i = 0; i < 3; ++i)
                        {
                            sum += m[row * 3 + i] * rhs.m[i * 3 + col];
                        }
                        result.m[row * 3 + col] = sum;
                    }
                }
                return result;
            }

            // Multiply matrix by vector
            VVector3 operator*(const VVector3 &v) const noexcept
            {
                return VVector3{m[0] * v.x + m[1] * v.y + m[2] * v.z, m[3] * v.x + m[4] * v.y + m[5] * v.z, m[6] * v.x + m[7] * v.y + m[8] * v.z};
            }

            // Create rotation matrix around X axis (degrees)
            static VMat3 RotationX(float degrees) noexcept
            {
                float rad = degrees * 3.14159265358979323846f / 180.f;
                float c   = std::cos(rad);
                float s   = std::sin(rad);
                VMat3 mat = Identity();
                mat.m[4]  = c;
                mat.m[5]  = -s;
                mat.m[7]  = s;
                mat.m[8]  = c;
                return mat;
            }

            // Create rotation matrix around Y axis (degrees)
            static VMat3 RotationY(float degrees) noexcept
            {
                float rad = degrees * 3.14159265358979323846f / 180.f;
                float c   = std::cos(rad);
                float s   = std::sin(rad);
                VMat3 mat = Identity();
                mat.m[0]  = c;
                mat.m[2]  = s;
                mat.m[6]  = -s;
                mat.m[8]  = c;
                return mat;
            }

            // Create rotation matrix around Z axis (degrees)
            static VMat3 RotationZ(float degrees) noexcept
            {
                float rad = degrees * 3.14159265358979323846f / 180.f;
                float c   = std::cos(rad);
                float s   = std::sin(rad);
                VMat3 mat = Identity();
                mat.m[0]  = c;
                mat.m[1]  = -s;
                mat.m[3]  = s;
                mat.m[4]  = c;
                return mat;
            }

            // Create a LookAt rotation matrix (only rotation, no translation)
            static VMat3 LookAt(const VVector3 &eye, const VVector3 &center, const VVector3 &up) noexcept
            {
                VVector3 f = (center - eye).Normalized();
                VVector3 s = f.Cross(up).Normalized();
                VVector3 u = s.Cross(f);

                VMat3 mat{};
                // Each row is a basis vector of the new orientation
                mat.m[0] = s.x;
                mat.m[1] = s.y;
                mat.m[2] = s.z;
                mat.m[3] = u.x;
                mat.m[4] = u.y;
                mat.m[5] = u.z;
                mat.m[6] = -f.x;
                mat.m[7] = -f.y;
                mat.m[8] = -f.z;

                return mat;
            }

            // Transpose the matrix
            VMat3 Transpose() const noexcept
            {
                VMat3 result{};
                for (int row = 0; row < 3; ++row)
                {
                    for (int col = 0; col < 3; ++col)
                    {
                        result.m[col * 3 + row] = m[row * 3 + col];
                    }
                }
                return result;
            }

            constexpr const float *Data() const noexcept { return m.data(); }
            float                 *Data() noexcept { return m.data(); }
    };

    // ----------------- VMat2 -----------------
    struct VMat2
    {

            // 2x2 matrix stored in column-major order:
            // [ m[0] m[2] ]
            // [ m[1] m[3] ]
            std::array<float, 4> m{};

            // Identity matrix
            static constexpr VMat2 Identity() noexcept
            {
                VMat2 mat{};
                mat.m[0] = 1.f;
                mat.m[3] = 1.f;
                return mat;
            }

            // Multiply two 2x2 matrices
            VMat2 operator*(const VMat2 &rhs) const noexcept
            {
                VMat2 result{};
                result.m[0] = m[0] * rhs.m[0] + m[2] * rhs.m[1];
                result.m[1] = m[1] * rhs.m[0] + m[3] * rhs.m[1];
                result.m[2] = m[0] * rhs.m[2] + m[2] * rhs.m[3];
                result.m[3] = m[1] * rhs.m[2] + m[3] * rhs.m[3];
                return result;
            }

            // Multiply by a vector (2D)
            VVector2 operator*(const VVector2 &vec) const noexcept { return {m[0] * vec.x + m[2] * vec.y, m[1] * vec.x + m[3] * vec.y}; }

            // Transpose the matrix
            VMat2 Transposed() const noexcept
            {
                VMat2 result{};
                result.m[0] = m[0];
                result.m[1] = m[2];
                result.m[2] = m[1];
                result.m[3] = m[3];
                return result;
            }

            // Compute determinant
            constexpr float Determinant() const noexcept { return m[0] * m[3] - m[2] * m[1]; }

            // Create a rotation matrix (in degrees)
            static VMat2 Rotation(float degrees) noexcept
            {
                float rad = degrees * 3.14159265358979323846f / 180.f;
                float c   = std::cos(rad);
                float s   = std::sin(rad);

                VMat2 mat{};
                mat.m[0] = c;
                mat.m[1] = s;
                mat.m[2] = -s;
                mat.m[3] = c;
                return mat;
            }

            // Create a scaling matrix
            static VMat2 Scale(float scaleX, float scaleY) noexcept
            {
                VMat2 mat{};
                mat.m[0] = scaleX;
                mat.m[3] = scaleY;
                return mat;
            }

            constexpr const float *Data() const noexcept { return m.data(); }
            float                 *Data() noexcept { return m.data(); }
    };
} // namespace Vantor::Math