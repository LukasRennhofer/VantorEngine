#pragma once

#include <string>

// RenderDevice Interface
#include "../RenderDevice/Interface/VRD_Texture.hpp"

// Math
#include "../Math/Linear/VMA_Vector.hpp"
#include "../Math/Linear/VMA_Matrix.hpp"

namespace Vantor::Renderer {

    enum VEUniformType
    {
        UniformTypeBOOL,
        UniformTypeINT,
        UniformTypeFLOAT,
        UniformTypeSAMPLER1D,
        UniformTypeSAMPLER2D,
        UniformTypeSAMPLER3D,
        UniformTypeSAMPLERCUBE,
        UniformTypeVEC2,
        UniformTypeVEC3,
        UniformTypeVEC4,
        UniformTypeMAT2,
        UniformTypeMAT3,
        UniformTypeMAT4,
    };

    struct VUniform
    {
        VEUniformType  Type;
        std::string  Name;
        int          Size;
        unsigned int Location;
    };

    struct VUniformValue
    {
        VEUniformType Type;

        union
        {
            bool       Bool;
            int        Int;
            float      Float;

            Vantor::Math::VVector2 Vec2;
            Vantor::Math::VVector3 Vec3;
            Vantor::Math::VVector4 Vec4;
            Vantor::Math::VMat2 Mat2;
            Vantor::Math::VMat3 Mat3;
            Vantor::Math::VMat4 Mat4;
        };

        VUniformValue() {}
    };

    struct VUniformValueSampler
    {
        VEUniformType  Type;
        unsigned int Unit;
        union 
        {
            Vantor::RenderDevice::VTexture     *Texture;
            Vantor::RenderDevice::VTextureCube *TextureCube;
        };

        VUniformValueSampler() {}
    };

    struct VVertexAttribute
    {
        VEUniformType  Type;
        std::string  Name;
        int          Size;
        unsigned int Location;
    };
}