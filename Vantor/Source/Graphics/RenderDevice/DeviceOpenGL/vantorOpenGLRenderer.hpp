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
 *  File: vantorOpenGLRenderer.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include <glm/glm.hpp>

#include "../../Renderer/Light/vantorPointLight.hpp"
#include "../../Renderer/Light/vantorDirectionalLight.hpp"
#include "../../Geometry/Primitives/vantorQuad.hpp"
#include "vantorOpenGLShader.hpp"
#include "vantorOpenGLCommandBuffer.hpp"
#include "vantorOpenGLChache.hpp"
#include "../../../Core/Scene/vantorSceneNode.hpp"
#include "vantorOpenGLMaterialLibrary.hpp"
#include "PBR/vantorOpenGLPBR.hpp"

#include "../../Renderer/Camera/vantorCamera.hpp"

#include <glad/glad.h>
#include <string>

namespace vantor::Graphics::RenderDevice::OpenGL
{
    class Mesh;
    class Material;
    class Scene;
    class RenderTarget;
    class PostProcessor;

    class Renderer
    {
            friend PostProcessor;
            friend PBR;

        public:
            bool IrradianceGI = true;
            bool Shadows      = true;
            bool Lights       = true;
            bool RenderLights = true;
            bool LightVolumes = false;
            bool RenderProbes = false;
            bool Wireframe    = false;

        private:
            // render state
            CommandBuffer *m_CommandBuffer;
            GLCache        m_GLCache;
            glm::vec2      m_RenderSize;

            // lighting
            std::vector<vantor::Graphics::DirectionalLight *> m_DirectionalLights;
            std::vector<vantor::Graphics::PointLight *>       m_PointLights;
            RenderTarget                                     *m_GBuffer = nullptr;
            Mesh                                             *m_DeferredPointMesh;

            // materials
            MaterialLibrary *m_MaterialLibrary;

            // Camera
            vantor::Graphics::Camera *m_Camera;
            glm::mat4                 m_PrevViewProjection;

            // render-targets/post
            std::vector<RenderTarget *>                   m_RenderTargetsCustom;
            RenderTarget                                 *m_CurrentRenderTargetCustom = nullptr;
            RenderTarget                                 *m_CustomTarget;
            RenderTarget                                 *m_PostProcessTarget1;
            PostProcessor                                *m_PostProcessor;
            vantor::Graphics::Geometry::Primitives::Quad *m_NDCPlane;
            unsigned int                                  m_FramebufferCubemap;
            unsigned int                                  m_CubemapDepthRBO;

            // shadow buffers
            std::vector<RenderTarget *> m_ShadowRenderTargets;
            std::vector<glm::mat4>      m_ShadowViewProjections;

            // pbr
            PBR                   *m_PBR;
            unsigned int           m_PBREnvironmentIndex;
            std::vector<glm::vec4> m_ProbeSpatials;

            // ubo
            unsigned int m_GlobalUBO;

            // debug
            Mesh *m_DebugLightMesh;

        public:
            Renderer();
            ~Renderer();

            void Init();

            void      SetRenderSize(unsigned int width, unsigned int height);
            glm::vec2 GetRenderSize();

            void SetTarget(RenderTarget *renderTarget, GLenum target = GL_TEXTURE_2D);

            vantor::Graphics::Camera *GetCamera();
            void                      SetCamera(vantor::Graphics::Camera *camera);

            PostProcessor *GetPostProcessor();

            Material *CreateMaterial(std::string base = "default");
            Material *CreateCustomMaterial(Shader *shader);
            Material *CreatePostProcessingMaterial(Shader *shader);

            void PushRender(Mesh *mesh, Material *material, glm::mat4 transform = glm::mat4(), glm::mat4 prevFrameTransform = glm::mat4());
            void PushRender(vantor::SceneNode *node);
            void PushPostProcessor(Material *postProcessor);

            void AddLight(vantor::Graphics::DirectionalLight *light);
            void AddLight(vantor::Graphics::PointLight *light);

            void RenderPushedCommands();

            void Blit(Texture *src, RenderTarget *dst = nullptr, Material *material = nullptr, std::string textureUniformName = "TexSrc");

            void                                                SetSkyCapture(vantor::Graphics::RenderDevice::OpenGL::PBRCapture *pbrEnvironment);
            vantor::Graphics::RenderDevice::OpenGL::PBRCapture *GetSkypCature();
            void                                                AddIrradianceProbe(glm::vec3 position, float radius);
            void                                                BakeProbes(vantor::SceneNode *scene = nullptr);

        private:
            void renderCustomCommand(RenderCommand *command, vantor::Graphics::Camera *customCamera, bool updateGLSettings = true);
            void renderToCubemap(vantor::SceneNode *scene, TextureCube *target, glm::vec3 position = glm::vec3(0.0f), unsigned int mipLevel = 0);
            void
            renderToCubemap(std::vector<RenderCommand> &renderCommands, TextureCube *target, glm::vec3 position = glm::vec3(0.0f), unsigned int mipLevel = 0);
            void          renderMesh(Mesh *mesh, Shader *shader);
            void          updateGlobalUBOs();
            RenderTarget *getCurrentRenderTarget();

            void renderDeferredAmbient();
            void renderDeferredDirLight(vantor::Graphics::DirectionalLight *light);
            void renderDeferredPointLight(vantor::Graphics::PointLight *light);

            void renderShadowCastCommand(RenderCommand *command, const glm::mat4 &projection, const glm::mat4 &view);
    };
} // namespace vantor::Graphics::RenderDevice::OpenGL