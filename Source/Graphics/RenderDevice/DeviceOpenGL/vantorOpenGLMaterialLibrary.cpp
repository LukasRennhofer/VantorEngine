/*
 *  ╔═══════════════════════════════════════════════════════════════╗
 *  ║                          ~ Vantor ~                           ║
 *  ║                                                               ║
 *  ║  This file is part of the Vantor Engine.                      ║
 *  ║  Automatically formatted by vantorFormat.py                   ║
 *  ║                                                               ║
 *  ╚═══════════════════════════════════════════════════════════════╝
 *
 *  Copyright (c) 2025 Lukas Rennhofer
 *  Licensed under the GNU General Public License, Version 3.
 *  See LICENSE file for more details.
 *
 *  Author: Lukas Rennhofer
 *  Date: 2025-05-12
 *
 *  File: vantorOpenGLMaterialLibrary.cpp
 *  Last Change: Automatically updated
 */

#include "vantorOpenGLRenderTarget.hpp"

#include "vantorOpenGLMaterial.hpp"
#include "../../../Core/Resource/vantorResource.hpp"

#include "../../../Core/BackLog/vantorBacklog.h"
#include "../../../Helpers/vantorString.hpp"

namespace vantor::Graphics::RenderDevice::OpenGL
{
    // --------------------------------------------------------------------------------------------
    MaterialLibrary::MaterialLibrary(RenderTarget *gBuffer)
    {
        generateDefaultMaterials();
        generateInternalMaterials(gBuffer);
    }
    // --------------------------------------------------------------------------------------------
    MaterialLibrary::~MaterialLibrary()
    {
        for (auto it = m_DefaultMaterials.begin(); it != m_DefaultMaterials.end(); ++it)
        {
            delete it->second;
        }
        for (unsigned int i = 0; i < m_Materials.size(); ++i)
        {
            delete m_Materials[i];
        }
        delete debugLightMaterial;

        delete defaultBlitMaterial;
    }
    // --------------------------------------------------------------------------------------------
    Material *MaterialLibrary::CreateMaterial(std::string base)
    {
        auto found = m_DefaultMaterials.find(SID(base));
        if (found != m_DefaultMaterials.end())
        {
            Material  copy = found->second->Copy();
            Material *mat  = new Material(copy);
            m_Materials.push_back(mat);
            return mat;
        }
        else
        {
            vantor::Backlog::Log("OpenGLMaterialLibrary", "Material of template: " + base + " requested, but template did not exist.",
                                 vantor::Backlog::LogLevel::WARNING);
            return nullptr;
        }
    }
    // --------------------------------------------------------------------------------------------
    Material *MaterialLibrary::CreateCustomMaterial(Shader *shader)
    {
        Material *mat = new Material(shader);
        mat->Type     = MATERIAL_CUSTOM;
        m_Materials.push_back(mat);
        return mat;
    }
    // --------------------------------------------------------------------------------------------
    Material *MaterialLibrary::CreatePostProcessingMaterial(Shader *shader)
    {
        Material *mat = new Material(shader);
        mat->Type     = MATERIAL_POST_PROCESS;
        m_Materials.push_back(mat);
        return mat;
    }
    // --------------------------------------------------------------------------------------------
    void MaterialLibrary::generateDefaultMaterials()
    {
        // default render material (deferred path)
        Shader *defaultShader = vantor::Resources::LoadShader("default", "res/intern/shaders/deferred/g_buffer.vs", "res/intern/shaders/deferred/g_buffer.fs");
        Material *defaultMat  = new Material(defaultShader);
        defaultMat->Type      = MATERIAL_DEFAULT;
        defaultMat->SetTexture("TexAlbedo", vantor::Resources::LoadTexture("default albedo", "res/intern/textures/checkerboard.png", GL_TEXTURE_2D, GL_RGB), 3);
        defaultMat->SetTexture("TexNormal", vantor::Resources::LoadTexture("default normal", "res/intern/textures/norm.png"), 4);
        defaultMat->SetTexture("TexMetallic", vantor::Resources::LoadTexture("default metallic", "res/intern/textures/black.png"), 5);
        defaultMat->SetTexture("TexRoughness", vantor::Resources::LoadTexture("default roughness", "res/intern/textures/checkerboard.png"), 6);
        m_DefaultMaterials[SID("default")] = defaultMat;
        // glass material
        Shader *glassShader
            = vantor::Resources::LoadShader("glass", "res/intern/shaders/forward_render.vs", "res/intern/shaders/forward_render.fs", {"ALPHA_BLEND"});
        glassShader->Use();
        glassShader->SetInt("lightShadowMap1", 10);
        glassShader->SetInt("lightShadowMap2", 10);
        glassShader->SetInt("lightShadowMap3", 10);
        glassShader->SetInt("lightShadowMap4", 10);
        Material *glassMat = new Material(glassShader);
        glassMat->Type     = MATERIAL_CUSTOM;
        glassMat->SetTexture("TexAlbedo", vantor::Resources::LoadTexture("glass albedo", "res/intern/textures/glass.png", GL_TEXTURE_2D, GL_RGBA), 0);
        glassMat->SetTexture("TexNormal", vantor::Resources::LoadTexture("glass normal", "res/intern/textures/pbr/plastic/normal.png"), 1);
        glassMat->SetTexture("TexMetallic", vantor::Resources::LoadTexture("glass metallic", "res/intern/textures/pbr/plastic/metallic.png"), 2);
        glassMat->SetTexture("TexRoughness", vantor::Resources::LoadTexture("glass roughness", "res/intern/textures/pbr/plastic/roughness.png"), 3);
        glassMat->SetTexture("TexAO", vantor::Resources::LoadTexture("glass ao", "res/intern/textures/pbr/plastic/ao.png"), 4);
        glassMat->Blend                  = true;
        m_DefaultMaterials[SID("glass")] = glassMat;
        // alpha blend material
        Shader *alphaBlendShader
            = vantor::Resources::LoadShader("alpha blend", "res/intern/shaders/forward_render.vs", "res/intern/shaders/forward_render.fs", {"ALPHA_BLEND"});
        alphaBlendShader->Use();
        alphaBlendShader->SetInt("lightShadowMap1", 10);
        alphaBlendShader->SetInt("lightShadowMap2", 10);
        alphaBlendShader->SetInt("lightShadowMap3", 10);
        alphaBlendShader->SetInt("lightShadowMap4", 10);
        Material *alphaBlendMaterial           = new Material(alphaBlendShader);
        alphaBlendMaterial->Type               = MATERIAL_CUSTOM;
        alphaBlendMaterial->Blend              = true;
        m_DefaultMaterials[SID("alpha blend")] = alphaBlendMaterial;
        // alpha cutout material
        Shader *alphaDiscardShader
            = vantor::Resources::LoadShader("alpha discard", "res/intern/shaders/forward_render.vs", "res/intern/shaders/forward_render.fs", {"ALPHA_DISCARD"});
        alphaDiscardShader->Use();
        alphaDiscardShader->SetInt("lightShadowMap1", 10);
        alphaDiscardShader->SetInt("lightShadowMap2", 10);
        alphaDiscardShader->SetInt("lightShadowMap3", 10);
        alphaDiscardShader->SetInt("lightShadowMap4", 10);
        Material *alphaDiscardMaterial           = new Material(alphaDiscardShader);
        alphaDiscardMaterial->Type               = MATERIAL_CUSTOM;
        alphaDiscardMaterial->Cull               = false;
        m_DefaultMaterials[SID("alpha discard")] = alphaDiscardMaterial;
    }
    // --------------------------------------------------------------------------------------------
    void MaterialLibrary::generateInternalMaterials(RenderTarget *gBuffer)
    {
        // post-processing
        Shader *defaultBlitShader = vantor::Resources::LoadShader("blit", "res/intern/shaders/screen_quad.vs", "res/intern/shaders/default_blit.fs");
        defaultBlitMaterial       = new Material(defaultBlitShader);

        // deferred
        deferredAmbientShader
            = vantor::Resources::LoadShader("deferred ambient", "res/intern/shaders/deferred/screen_ambient.vs", "res/intern/shaders/deferred/ambient.fs");
        deferredIrradianceShader  = vantor::Resources::LoadShader("deferred irradiance", "res/intern/shaders/deferred/ambient_irradience.vs",
                                                                  "res/intern/shaders/deferred/ambient_irradience.fs");
        deferredDirectionalShader = vantor::Resources::LoadShader("deferred directional", "res/intern/shaders/deferred/screen_directional.vs",
                                                                  "res/intern/shaders/deferred/directional.fs");
        deferredPointShader = vantor::Resources::LoadShader("deferred point", "res/intern/shaders/deferred/point.vs", "res/intern/shaders/deferred/point.fs");

        deferredAmbientShader->Use();
        deferredAmbientShader->SetInt("gPositionMetallic", 0);
        deferredAmbientShader->SetInt("gNormalRoughness", 1);
        deferredAmbientShader->SetInt("gAlbedoAO", 2);
        deferredAmbientShader->SetInt("envIrradiance", 3);
        deferredAmbientShader->SetInt("envPrefilter", 4);
        deferredAmbientShader->SetInt("BRDFLUT", 5);
        deferredAmbientShader->SetInt("TexSSAO", 6);
        deferredIrradianceShader->Use();
        deferredIrradianceShader->SetInt("gPositionMetallic", 0);
        deferredIrradianceShader->SetInt("gNormalRoughness", 1);
        deferredIrradianceShader->SetInt("gAlbedoAO", 2);
        deferredIrradianceShader->SetInt("envIrradiance", 3);
        deferredIrradianceShader->SetInt("envPrefilter", 4);
        deferredIrradianceShader->SetInt("BRDFLUT", 5);
        deferredIrradianceShader->SetInt("TexSSAO", 6);
        deferredDirectionalShader->Use();
        deferredDirectionalShader->SetInt("gPositionMetallic", 0);
        deferredDirectionalShader->SetInt("gNormalRoughness", 1);
        deferredDirectionalShader->SetInt("gAlbedoAO", 2);
        deferredDirectionalShader->SetInt("lightShadowMap", 3);
        deferredPointShader->Use();
        deferredPointShader->SetInt("gPositionMetallic", 0);
        deferredPointShader->SetInt("gNormalRoughness", 1);
        deferredPointShader->SetInt("gAlbedoAO", 2);

        // shadows
        dirShadowShader = vantor::Resources::LoadShader("shadow directional", "res/intern/shaders/shadow_cast.vs", "res/intern/shaders/shadow_cast.fs");

        // debug
        Shader *debugLightShader = vantor::Resources::LoadShader("debug light", "res/intern/shaders/light.vs", "res/intern/shaders/light.fs");
        debugLightMaterial       = new Material(debugLightShader);
    }
} // namespace vantor::Graphics::RenderDevice::OpenGL