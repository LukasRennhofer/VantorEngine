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
 *  Date: 2025-07-16
 *
 *  File: VRE_LightData.hpp
 *  Last Change: Automatically updated
 */

#pragma once

// Color
#include "../../Core/Types/VCO_Color.hpp"
#include "../../Math/Linear/VMA_Vector.hpp" // VVector
// Math
#include "../../Math/Linear/VMA_Vector.hpp" // VVector

namespace Vantor::Renderer
{

    /**
     * Point light data structure compatible with GLSL shader bindings
     *
     * This structure represents a point light source in 3D space that emits light
     * uniformly in all directions. The light's intensity decreases with distance
     * according to the attenuation formula:
     *
     * attenuation = 1.0 / (constant + linear * distance + quadratic * distance²)
     *
     * The structure is carefully laid out to match GLSL std140 layout requirements
     * for uniform buffer objects, ensuring proper alignment and padding.
     *
     * The ambient, diffuse, and specular arrays are sized to [3] to match
     * GLSL vec3 alignment requirements in uniform buffers.
     */
    struct VPointLightData
    {
            Vantor::Math::VVector3 position;

            /**
             * Constant attenuation factor
             *
             * The constant term in the attenuation equation. Typically set to 1.0
             * to ensure the light has some base intensity regardless of distance.
             * Higher values reduce the overall light intensity.
             */
            float constant;

            /**
             * Ambient light color components
             *
             * The ambient lighting contribution represents the light that bounces
             * around the environment and illuminates objects uniformly. This creates
             * a base level of illumination that prevents completely dark shadows.
             */
            Vantor::Math::VVector3 ambient;

            /**
             * Linear attenuation factor
             *
             * The linear term in the distance attenuation equation. Controls how
             * quickly the light intensity decreases with distance. Typical values
             * range from 0.0 (no linear falloff) to 0.1 or higher for faster falloff.
             */
            float linear;

            /**
             * Diffuse light color components
             *
             * The diffuse lighting represents the main directional lighting effect.
             * It determines how bright surfaces appear when facing the light source
             * and follows Lambert's cosine law for realistic surface shading.
             */
            Vantor::Math::VVector3 diffuse;

            /**
             * Quadratic attenuation factor
             *
             * The quadratic term in the distance attenuation equation. Provides
             * realistic physically-based light falloff that follows the inverse
             * square law. Typical values range from 0.0 to 0.1 depending on
             * the desired light range and intensity.
             */
            float quadratic;

            /**
             * Specular light color components
             *
             * The specular lighting creates highlights on shiny surfaces and
             * represents the direct reflection of the light source. The intensity
             * and size of highlights depend on the surface material properties
             * and viewing angle.
             */
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

} // namespace Vantor::Renderer