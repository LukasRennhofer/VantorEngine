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

#include <ActorRuntime/Public/VAR_Actor.hpp>

#include <Graphics/Public/Camera/VGFX_Frustum.hpp>

namespace VE::Graphics
{
    // TODO: Refactor
    enum class ECameraMovement
    {
        CAMERA_FORWARD,
        CAMERA_BACK,
        CAMERA_LEFT,
        CAMERA_RIGHT,
        CAMERA_UP,
        CAMERA_DOWN,
    };

    // Camera is a standalone registered Entity
    class ACamera : public VE::AActor
    {
        public:
            VE::Math::VMat4 Projection;
            VE::Math::VMat4 View;

            VE::Math::VVector3 Position = VE::Math::VVector3(0.0f, 0.0f, 0.0f);
            VE::Math::VVector3 Forward  = VE::Math::VVector3(0.0f, 0.0f, -1.0f);
            VE::Math::VVector3 Up       = VE::Math::VVector3(0.0f, 1.0f, 0.0f);
            VE::Math::VVector3 Right    = VE::Math::VVector3(1.0f, 0.0f, 0.0f);

            float FOV;
            float Aspect;
            float Near;
            float Far;
            bool  Perspective;

            VCameraFrustum Frustum; // For LOD

        private:
        public:
            ACamera();
            ACamera(VE::Math::VVector3 position, VE::Math::VVector3 forward, VE::Math::VVector3 up);

            void Update(float dt);

            void SetPerspective(float fov, float aspect, float near, float far);
            void SetOrthographic(float left, float right, float top, float bottom, float near, float far);

            void UpdateView();

            float FrustumHeightAtDistance(float distance);
            float DistanceAtFrustumHeight(float frustumHeight);
    };
} // namespace VE::Internal::Graphics