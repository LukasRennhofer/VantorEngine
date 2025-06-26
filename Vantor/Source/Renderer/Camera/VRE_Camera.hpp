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
 *  File: VRE_Camera.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include "../../Math/Linear/VMA_Vector.hpp"
#include "../../Math/Linear/VMA_Matrix.hpp"

#include "VRE_Frustum.hpp"

namespace Vantor::Renderer
{
    // TODO: Refactor
    enum VECameraMovement
    {
        CAMERA_FORWARD,
        CAMERA_BACK,
        CAMERA_LEFT,
        CAMERA_RIGHT,
        CAMERA_UP,
        CAMERA_DOWN,
    };

    class Camera
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

            CameraFrustum Frustum;

        private:
        public:
            Camera();
            Camera(Vantor::Math::VVector3 position, Vantor::Math::VVector3 forward, Vantor::Math::VVector3 up);

            void Update(float dt);

            void SetPerspective(float fov, float aspect, float near, float far);
            void SetOrthographic(float left, float right, float top, float bottom, float near, float far);

            void UpdateView();

            float FrustumHeightAtDistance(float distance);
            float DistanceAtFrustumHeight(float frustumHeight);
    };
} // namespace Vantor::Renderer