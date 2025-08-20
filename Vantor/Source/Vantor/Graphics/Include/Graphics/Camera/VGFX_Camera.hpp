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

#include <ActorRuntime/VAR_Actor.hpp>

#include <Graphics/Camera/VGFX_Frustum.hpp>

namespace Vantor::Graphics
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
    class ACamera : public Vantor::ActorRuntime::AActor
    {
        public:
            Vantor::Math::VMat4 Projection;
            Vantor::Math::VMat4 View;

            Vantor::Math::VVector3 Position = Vantor::Math::VVector3(0.0f, 0.0f, 0.0f);
            Vantor::Math::VVector3 Forward  = Vantor::Math::VVector3(0.0f, 0.0f, -1.0f);
            Vantor::Math::VVector3 Up       = Vantor::Math::VVector3(0.0f, 1.0f, 0.0f);
            Vantor::Math::VVector3 Right    = Vantor::Math::VVector3(1.0f, 0.0f, 0.0f);

            float FOV;
            float Aspect;
            float Near;
            float Far;
            bool  Perspective;

            VCameraFrustum Frustum; // For LOD

        private:
        public:
            ACamera();
            ACamera(Vantor::Math::VVector3 position, Vantor::Math::VVector3 forward, Vantor::Math::VVector3 up);

            void Update(float dt);

            void SetPerspective(float fov, float aspect, float near, float far);
            void SetOrthographic(float left, float right, float top, float bottom, float near, float far);

            void UpdateView();

            float FrustumHeightAtDistance(float distance);
            float DistanceAtFrustumHeight(float frustumHeight);
    };
} // namespace Vantor::Graphics