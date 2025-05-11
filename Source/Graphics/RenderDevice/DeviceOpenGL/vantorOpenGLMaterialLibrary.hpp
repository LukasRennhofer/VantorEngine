/*
 *    				~ Vantor ~
 *               
 * Copyright (c) 2025 Lukas Rennhofer
 *
 * Licensed under the GNU General Public License, Version 3. See LICENSE file for more details.
 *
 * Author: Lukas Rennhofer
 * Date: 2025-03-08
 *
 * File: vantorOpenGLMaterialLibrary.hpp
 * Last Change: 
*/

#pragma once

#include <map>
#include <vector>
#include <string>

namespace vantor::Graphics::RenderDevice::OpenGL
{
    class Material;
    class Renderer;
    class Shader;
    class RenderTarget;

    class MaterialLibrary
    {
        friend Renderer;
    private:
        std::map<unsigned int, Material*> m_DefaultMaterials;
        std::vector<Material*> m_Materials;

        // internal render-specific materials
        Material* defaultBlitMaterial;

        Shader* deferredAmbientShader;
        Shader* deferredIrradianceShader;
        Shader* deferredDirectionalShader;
        Shader* deferredPointShader;

        Shader *dirShadowShader;

        Material *debugLightMaterial;
    public:
        MaterialLibrary(RenderTarget *gBuffer);
        ~MaterialLibrary();

        // create either a deferred default material (based on default set of materials available (like glass)), or a custom material (with custom you have to supply your own shader)
        Material* CreateMaterial(std::string base);             // these don't have the custom flag set (default material has default state and uses checkerboard texture as albedo (and black metallic, half roughness, purple normal, white ao)
        Material* CreateCustomMaterial(Shader *shader);         // these have the custom flag set (will be rendered in forward pass)
        Material* CreatePostProcessingMaterial(Shader *shader); // these have the post-processing flag set (will be rendered after deferred/forward pass)
    private:
        // generate all default template materials
        void generateDefaultMaterials();
        // generate all internal materials used by the renderer; run in MaterialLibrary to improve readability.
        void generateInternalMaterials(RenderTarget *gBuffer);
    };
}