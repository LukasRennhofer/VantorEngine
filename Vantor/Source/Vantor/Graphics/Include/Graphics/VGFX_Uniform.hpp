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

#include <RHI/Interface/VRHI_Texture.hpp>

#include <string>

namespace Vantor::Graphics
{

    enum class EUniformType
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

    enum class ESamplerType {
        SamplerDiffuse,
        SamplerSpecular,
        SamplerNormal,
        SamplerMetallic,
        SamplerRoughness,
        SamplerHeight, // For parallax mapping
        SamplerAO,
        SamplerCustom
    };

    struct VUniform
    {
            EUniformType Type;
            std::string   Name;
            int           Size;
            unsigned int  Location;
    };

    struct VUniformValue
    {
            EUniformType Type;

            union
            {
                    bool  Bool;
                    int   Int;
                    float Float;

                    Vantor::Math::VVector2 Vec2;
                    Vantor::Math::VVector3 Vec3;
                    Vantor::Math::VVector4 Vec4;
                    Vantor::Math::VMat2    Mat2;
                    Vantor::Math::VMat3    Mat3;
                    Vantor::Math::VMat4    Mat4;
            };

            VUniformValue() {}
    };

    struct VUniformValueSampler
    {
            EUniformType Type;
            ESamplerType SType;
            unsigned int  Unit;
            union
            {
                    Vantor::RHI::IRHITexture     *Texture;
                    // TODO: TextureCube
            };

            VUniformValueSampler() {}
    };
} // namespace Vantor::Graphics