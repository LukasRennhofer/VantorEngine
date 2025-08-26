/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

#include <Math/Linear/VMA_Vector.hpp>
#include <Math/Linear/VMA_Matrix.hpp>

namespace VE::Internal::Graphics
{
    class ACamera;

    struct VFrustumPlane
    {
            VE::Internal::Math::VVector3 Normal;
            float                  D;

            void SetNormalD(VE::Internal::Math::VVector3 normal, VE::Internal::Math::VVector3 point)
            {
                Normal = normal.Normalized();
                D      = -Normal.Dot(point);
            }

            float Distance(VE::Internal::Math::VVector3 point) { return Normal.Dot(point) + D; }
    };

    class VCameraFrustum
    {
        public:
            VFrustumPlane Planes[6];
            struct VCamPlanes
            {
                    VFrustumPlane Left;
                    VFrustumPlane Right;
                    VFrustumPlane Top;
                    VFrustumPlane Bottom;
                    VFrustumPlane Near;
                    VFrustumPlane Far;
            } VCamPlanes;

        public:
            VCameraFrustum() {}

            void Update(ACamera *camera);

            bool Intersect(VE::Internal::Math::VVector3 point);
            bool Intersect(VE::Internal::Math::VVector3 point, float radius);
            bool Intersect(VE::Internal::Math::VVector3 boxMin, VE::Internal::Math::VVector3 boxMax);
    };
} // namespace VE::Internal::Graphics