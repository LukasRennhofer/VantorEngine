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
 * File: vantorOpenGL.hpp
 * Last Change:
*/

#pragma once

#include <glm/glm.hpp>

#include "../../Renderer/Light/vantorPointLight.hpp"
#include "../../Renderer/Light/vantorDirectionalLight.hpp"
#include "../../Geometry/Primitives/vantorQuad.hpp"
#include "vantorOpenGLCommandBuffer.hpp"
#include "PBR/vantorOpenGLPBR.hpp"
#include "vantorOpenGLChache.hpp"

#include <glad/glad.h>

namespace vantor::Graphics::RenderDevice::OpenGL {
    class Mesh;
    class Material;
    class Scene;
    class SceneNode;
    class Camera;
    class RenderTarget;
    class MaterialLibrary;
    class PBR;
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
        CommandBuffer* m_CommandBuffer;
        GLCache        m_GLCache;
        glm::vec2     m_RenderSize;

        // lighting
        std::vector<vantor::Graphics::DirectionalLight*> m_DirectionalLights;
        std::vector<vantor::Graphics::PointLight*>       m_PointLights;
        RenderTarget* m_GBuffer = nullptr;
        Mesh*         m_DeferredPointMesh;

        // materials
        MaterialLibrary* m_MaterialLibrary;

        // camera
        Camera*    m_Camera;
        glm::mat4 m_PrevViewProjection;

        // render-targets/post
        std::vector<RenderTarget*> m_RenderTargetsCustom;
        RenderTarget*              m_CurrentRenderTargetCustom = nullptr;
        RenderTarget*              m_CustomTarget;
        RenderTarget*              m_PostProcessTarget1;
        PostProcessor*             m_PostProcessor;
        vantor::Graphics::Geometry::Primitives::Quad*                      m_NDCPlane;
        unsigned int m_FramebufferCubemap; 
        unsigned int m_CubemapDepthRBO;

        // shadow buffers
        std::vector<RenderTarget*> m_ShadowRenderTargets;
        std::vector<glm::mat4>    m_ShadowViewProjections;
        
        // pbr
        PBR* m_PBR;
        unsigned int m_PBREnvironmentIndex;
        std::vector<glm::vec4> m_ProbeSpatials;

        // ubo
        unsigned int m_GlobalUBO;

        // debug
        Mesh* m_DebugLightMesh;

    public:
        Renderer();
        ~Renderer();

        void Init(GLADloadproc loadProcFunc);

        void SetRenderSize(unsigned int width, unsigned int height);
        glm::vec2 GetRenderSize();

        void SetTarget(RenderTarget* renderTarget, GLenum target = GL_TEXTURE_2D);

        Camera* GetCamera();
        void    SetCamera(Camera* camera);

        PostProcessor* GetPostProcessor();

        // create either a deferred default material (based on default set of materials available (like glass)), or a custom material (with custom you have to supply your own shader)
        Material* CreateMaterial(std::string base = "default"); // these don't have the custom flag set (default material has default state and uses checkerboard texture as albedo (and black metallic, half roughness, purple normal, white ao)
        Material* CreateCustomMaterial(Shader* shader);         // these have the custom flag set (will be rendered in forward pass)
        Material* CreatePostProcessingMaterial(Shader* shader); // these have the post-processing flag set (will be rendered after deferred/forward pass)

        void PushRender(Mesh* mesh, Material* material, glm::mat4 transform = glm::mat4(), glm::mat4 prevFrameTransform = glm::mat4());
        void PushRender(SceneNode* node);
        void PushPostProcessor(Material* postProcessor);

        void AddLight(vantor::Graphics::DirectionalLight *light);
        void AddLight(vantor::Graphics::PointLight       *light);        

        void RenderPushedCommands();

        void Blit(Texture* src, RenderTarget* dst = nullptr, Material* material = nullptr, std::string textureUniformName = "TexSrc");

        // pbr
        void        SetSkyCapture(PBRCapture* pbrEnvironment);
        PBRCapture* GetSkypCature();
        void        AddIrradianceProbe(glm::vec3 position, float radius);
        void        BakeProbes(SceneNode* scene = nullptr);
    private:
        //rendering a custom forward-pass command
        void renderCustomCommand(RenderCommand* command, Camera* customCamera, bool updateGLSettings = true);
        void renderToCubemap(SceneNode* scene, TextureCube* target, glm::vec3 position = glm::vec3(0.0f), unsigned int mipLevel = 0);
        void renderToCubemap(std::vector<RenderCommand>& renderCommands, TextureCube* target, glm::vec3 position = glm::vec3(0.0f), unsigned int mipLevel = 0);
        void renderMesh(Mesh* mesh, Shader* shader);
        void updateGlobalUBOs();
        RenderTarget* getCurrentRenderTarget();

        void renderDeferredAmbient();
        void renderDeferredDirLight(DirectionalLight* light);
        void renderDeferredPointLight(PointLight* light);

        void renderShadowCastCommand(RenderCommand* command, const glm::mat4& projection, const glm::mat4& view);
    };
} // namespace vantor::Graphics::RenderDevice::OpenGL