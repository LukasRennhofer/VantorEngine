#pragma once

// Color
#include "../../Core/Types/VCO_Color.hpp"

// Math
#include "../../Math/Linear/VMA_Vector.hpp" // VVector

namespace Vantor::Renderer {

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
    struct VPointLightData {
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
} // namespace Vantor::Renderer