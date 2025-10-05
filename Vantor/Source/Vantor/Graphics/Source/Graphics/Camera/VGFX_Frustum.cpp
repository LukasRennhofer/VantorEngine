/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#include <Graphics/Public/Camera/VGFX_Camera.hpp>
#include <Graphics/Public/Camera/VGFX_Frustum.hpp>

namespace VE::Graphics
{

    void VCameraFrustum::Update(ACamera *camera)
    {
        float tan        = 2.0 * std::tan(camera->FOV * 0.5); // TODO: Write own Tan func
        float nearHeight = tan * camera->Near;
        float nearWidth  = nearHeight * camera->Aspect;
        float farHeight  = tan * camera->Far;
        float farWidth   = farHeight * camera->Aspect;

        VE::Math::VVector3 nearCenter = camera->Position + camera->Forward * camera->Near;
        VE::Math::VVector3 farCenter  = camera->Position + camera->Forward * camera->Far;

        VE::Math::VVector3 v;
        // left plane
        v = (nearCenter - camera->Right * nearWidth * 0.5f) - camera->Position;
        VCamPlanes.Left.SetNormalD((v.Normalized()).Cross(camera->Up), nearCenter - camera->Right * nearWidth * 0.5f);
        // right plane
        v = (nearCenter + camera->Right * nearWidth * 0.5f) - camera->Position;
        VCamPlanes.Right.SetNormalD((camera->Up).Cross(v.Normalized()), nearCenter + camera->Right * nearWidth * 0.5f);
        // top plane
        v = (nearCenter + camera->Up * nearHeight * 0.5f) - camera->Position;
        VCamPlanes.Top.SetNormalD((v.Normalized()).Cross(camera->Right), nearCenter + camera->Up * nearHeight * 0.5f);
        // bottom plane
        v = (nearCenter - camera->Up * nearHeight * 0.5f) - camera->Position;
        VCamPlanes.Bottom.SetNormalD((camera->Right).Cross(v.Normalized()), nearCenter - camera->Up * nearHeight * 0.5f);
        // near plane
        VCamPlanes.Near.SetNormalD(camera->Forward, nearCenter);
        // far plane
        VCamPlanes.Far.SetNormalD(camera->Forward * -1, farCenter);
    }
    // ------------------------------------------------------------------------
    bool VCameraFrustum::Intersect(VE::Math::VVector3 point)
    {
        for (int i = 0; i < 6; ++i)
        {
            if (Planes[i].Distance(point) < 0)
            {
                return false;
            }
        }
        return true;
    }
    // ------------------------------------------------------------------------
    bool VCameraFrustum::Intersect(VE::Math::VVector3 point, float radius)
    {
        for (int i = 0; i < 6; ++i)
        {
            if (Planes[i].Distance(point) < -radius)
            {
                return false;
            }
        }
        return true;
    }
    // ------------------------------------------------------------------------
    bool VCameraFrustum::Intersect(VE::Math::VVector3 boxMin, VE::Math::VVector3 boxMax)
    {
        for (int i = 0; i < 6; ++i)
        {
            VE::Math::VVector3 positive = boxMin;
            if (Planes[i].Normal.x >= 0)
            {
                positive.x = boxMax.x;
            }
            if (Planes[i].Normal.y >= 0)
            {
                positive.y = boxMax.y;
            }
            if (Planes[i].Normal.z >= 0)
            {
                positive.z = boxMax.z;
            }
            if (Planes[i].Distance(positive) < 0)
            {
                return false;
            }
        }
        return true;
    }

} // namespace VE::Internal::Graphics