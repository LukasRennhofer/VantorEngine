#pragma once

#include "../../Math/Linear/VMA_Vector.hpp"
#include "../../Math/Linear/VMA_Matrix.hpp"

namespace Vantor::Renderer
{
    class Camera;

    struct FrustumPlane
    {
        Vantor::Math::VVector3 Normal;
        float      D;

        void SetNormalD(Vantor::Math::VVector3 normal, Vantor::Math::VVector3 point)
        {
            Normal = normal.Normalized();
            D      = -Normal.Dot(point);
        }

        float Distance(Vantor::Math::VVector3 point)
        {
            return Normal.Dot(point) + D;
        }
    };

    class CameraFrustum
    {
    public:

        FrustumPlane Planes[6];
        struct VCamPlanes
        {
            FrustumPlane Left;
            FrustumPlane Right;
            FrustumPlane Top;
            FrustumPlane Bottom;
            FrustumPlane Near;
            FrustumPlane Far;
        } VCamPlanes;

    public:
        CameraFrustum() { }

        void Update(Camera* camera);

        bool Intersect(Vantor::Math::VVector3 point);
        bool Intersect(Vantor::Math::VVector3 point, float radius);
        bool Intersect(Vantor::Math::VVector3 boxMin, Vantor::Math::VVector3 boxMax);
    };
}