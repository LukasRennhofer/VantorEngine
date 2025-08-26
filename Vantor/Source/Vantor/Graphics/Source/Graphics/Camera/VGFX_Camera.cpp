/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#include <Math/VMA_Common.hpp>

#include <Graphics/Camera/VGFX_Camera.hpp>

namespace VE::Internal::Graphics
{
    ACamera::ACamera() {}

    ACamera::ACamera(VE::Internal::Math::VVector3 position, VE::Internal::Math::VVector3 forward, VE::Internal::Math::VVector3 up) : Position(position), Forward(forward), Up(up)
    {
        UpdateView();
    }

    void ACamera::Update(float dt) { Frustum.Update(this); }

    void ACamera::SetPerspective(float fov, float aspect, float near, float far)
    {
        Perspective = true;
        Projection  = VE::Internal::Math::VMat4::Perspective(fov, aspect, near, far);
        FOV         = fov;
        Aspect      = aspect;
        Near        = near;
        Far         = far;
    }

    void ACamera::SetOrthographic(float left, float right, float top, float bottom, float near, float far)
    {
        Perspective = false;
        Projection  = VE::Internal::Math::VMat4::Orthographic(left, right, top, bottom, near, far);
        Near        = near;
        Far         = far;
    }

    void ACamera::UpdateView() { View = VE::Internal::Math::VMat4::LookAt(Position, Position + Forward, Up); }

    float ACamera::FrustumHeightAtDistance(float distance)
    {
        if (Perspective)
        {
            return 2.0f * distance * tanf(VE::Internal::Math::DegToRad(FOV * 0.5));
        }
        else
        {
            return Frustum.VCamPlanes.Top.D;
        }
    }

    float ACamera::DistanceAtFrustumHeight(float frustumHeight)
    {
        if (Perspective)
        {
            return frustumHeight * 0.5f / tanf(VE::Internal::Math::DegToRad(FOV * 0.5f));
        }
        else
        {
            return Frustum.VCamPlanes.Near.D;
        }
    }

}; // namespace VE::Internal::Graphics