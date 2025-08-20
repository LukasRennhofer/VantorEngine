/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

// TODO: More DOCS of the Structures and AAA ready structs

#pragma once

#include <Core/Types/VCO_Color.hpp>
#include <Math/Linear/VMA_Vector.hpp>

namespace Vantor::Graphics
{

    struct VPointLightData
    {
            Vantor::Math::VVector3 position;

            float constant;

            Vantor::Math::VVector3 ambient;

            float linear;

            Vantor::Math::VVector3 diffuse;

            float quadratic;

            Vantor::Math::VVector3 specular;

            float radius; // the light radius
    };

    struct VDirectionalLightData {
        Vantor::Math::VVector3 direction; // Light direction
        float padding1 = 0.0f;            // Padding for std140

        Vantor::Math::VVector3 ambient;   // Ambient color
        float padding2 = 0.0f;

        Vantor::Math::VVector3 diffuse;   // Diffuse color
        float padding3 = 0.0f;

        Vantor::Math::VVector3 specular;  // Specular color
        float padding4 = 0.0f;
    };

    struct VSpotLightData {
        Vantor::Math::VVector3 position;  // Light position
        float constant = 1.0f;            // Attenuation

        Vantor::Math::VVector3 direction; // Direction (normalized)
        float linear = 0.0f;              // Attenuation

        Vantor::Math::VVector3 ambient;   // Ambient color
        float quadratic = 0.0f;           // Attenuation

        Vantor::Math::VVector3 diffuse;   // Diffuse color
        float cutOff = 0.0f;              // Cosine of inner angle

        Vantor::Math::VVector3 specular;  // Specular color
        float outerCutOff = 0.0f;         // Cosine of outer angle

        float radius = 0.0f;              // Light range
        float padding = 0.0f;             // Padding for alignment
    };

    struct VAmbientLightData {
        Vantor::Math::VVector3 ambientColor;
        float padding = 0.0f; // Align to 16 bytes
    };

} // namespace Vantor::Graphics