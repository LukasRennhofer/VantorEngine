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
 *  File: VRE_Frustum.cpp
 *  Last Change: Automatically updated
 */

#include "VRE_Camera.hpp"
#include "VRE_Frustum.hpp"

namespace Vantor::Renderer
{
    // ------------------------------------------------------------------------
    void CameraFrustum::Update(Camera *camera)
    {
        float tan        = 2.0 * std::tan(camera->FOV * 0.5);
        float nearHeight = tan * camera->Near;
        float nearWidth  = nearHeight * camera->Aspect;
        float farHeight  = tan * camera->Far;
        float farWidth   = farHeight * camera->Aspect;

        Vantor::Math::VVector3 nearCenter = camera->Position + camera->Forward * camera->Near;
        Vantor::Math::VVector3 farCenter  = camera->Position + camera->Forward * camera->Far;

        Vantor::Math::VVector3 v;
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
    bool CameraFrustum::Intersect(Vantor::Math::VVector3 point)
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
    bool CameraFrustum::Intersect(Vantor::Math::VVector3 point, float radius)
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
    bool CameraFrustum::Intersect(Vantor::Math::VVector3 boxMin, Vantor::Math::VVector3 boxMax)
    {
        for (int i = 0; i < 6; ++i)
        {
            Vantor::Math::VVector3 positive = boxMin;
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

} // namespace Vantor::Renderer