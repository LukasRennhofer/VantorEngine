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
 *  File: VRE_Camera.cpp
 *  Last Change: Automatically updated
 */

#include "VRE_Camera.hpp"

// Math
#include "../../Math/VMA_Common.hpp"

namespace Vantor::Renderer
{
    Camera::Camera() {}

    Camera::Camera(Vantor::Math::VVector3 position, Vantor::Math::VVector3 forward, Vantor::Math::VVector3 up) : Position(position), Forward(forward), Up(up)
    {
        UpdateView();
    }

    void Camera::Update(float dt) { Frustum.Update(this); }

    void Camera::SetPerspective(float fov, float aspect, float near, float far)
    {
        Perspective = true;
        Projection  = Vantor::Math::VMat4::Perspective(fov, aspect, near, far);
        FOV         = fov;
        Aspect      = aspect;
        Near        = near;
        Far         = far;
    }

    void Camera::SetOrthographic(float left, float right, float top, float bottom, float near, float far)
    {
        Perspective = false;
        Projection  = Vantor::Math::VMat4::Orthographic(left, right, top, bottom, near, far);
        Near        = near;
        Far         = far;
    }

    void Camera::UpdateView() { View = Vantor::Math::VMat4::LookAt(Position, Position + Forward, Up); }

    float Camera::FrustumHeightAtDistance(float distance)
    {
        if (Perspective)
        {
            return 2.0f * distance * tanf(Vantor::Math::DegToRad(FOV * 0.5));
        }
        else
        {
            return Frustum.VCamPlanes.Top.D;
        }
    }

    float Camera::DistanceAtFrustumHeight(float frustumHeight)
    {
        if (Perspective)
        {
            return frustumHeight * 0.5f / tanf(Vantor::Math::DegToRad(FOV * 0.5f));
        }
        else
        {
            return Frustum.VCamPlanes.Near.D;
        }
    }

}; // namespace Vantor::Renderer