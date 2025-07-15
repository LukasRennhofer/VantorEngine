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
 *  Date: 2025-07-11
 *
 *  File: VRDO_RenderPath.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include <Shared/glad/glad.h>
#include <memory>
#include <unordered_map>
#include <vector>

#include "../../Resource/VRES_Manager.hpp"
#include "../Interface/VRD_RenderPath.hpp"
#include "VRDO_Shader.hpp"
#include "VRDO_Texture.hpp"

#include "VRDO_Shader.hpp"
#include "VRDO_Texture.hpp"
#include "VRDO_RenderTarget.hpp"

// Uniform buffer
#include "VRDO_UBO.hpp"

// Command Buffer
#include "VRDO_CommandBuffer.hpp"

// State Cache
#include "VRDO_StateCache.hpp"

// SSBO
#include "VRDO_SSBO.hpp"

// Screen Quad
#include "Common/VRDO_ScreenQuad.hpp"

// Resource Manager
#include "../../Resource/VRES_Manager.hpp"

namespace Vantor::RenderDevice
{
    // OpenGL-specific render pass base class
    class VRenderPassGL : public VRenderPass
    {
        public:
            VRenderPassGL(VERenderPassType type, const std::string &name);
            virtual ~VRenderPassGL() = default;

            VERenderPassType   GetType() const override { return m_Type; }
            const std::string &GetName() const override { return m_Name; }

        protected:
            VERenderPassType m_Type;
            std::string      m_Name;
    };

    // OpenGL 3D Forward Rendering Pass
    class VForwardRenderPassGL : public VRenderPassGL
    {
        public:
            VForwardRenderPassGL();
            ~VForwardRenderPassGL() = default;

            void Initialize(VRenderPath3D* renderPath) override;
            // Camera just for now
            void Execute() override;
            void Cleanup() override;

        private:
            VRenderPath3D* m_RenderPath;
    };

    class VGeometryRenderPassGL : public VRenderPassGL
    {
        public:
            VGeometryRenderPassGL();
            ~VGeometryRenderPassGL() = default;

            void Initialize(VRenderPath3D* renderPath) override;
            // Camera just for now
            void Execute() override;
            void Cleanup() override;

        private:
            VRenderPath3D* m_RenderPath;
    };

    class VLightingRenderPassGL : public VRenderPassGL
    {
        public:
            VLightingRenderPassGL();
            ~VLightingRenderPassGL() = default;

            void Initialize(VRenderPath3D* renderPath) override;
            // Camera just for now
            void Execute() override;
            void Cleanup() override;

        private:
            std::unique_ptr<VRenderTarget> m_OutBuffer;

            // Shaders : TODO : Move away to MaterialLib or something else (like a Register or smth)
            std::shared_ptr<VShader> m_ShaderLighting;

            VRenderPath3D* m_RenderPath;

            // Just for now: Screen Quad and OutBuffer
            VOpenGLScreenQuad m_ScreenQuad;
    };

    // OpenGL Shadow Mapping Pass : TODO
    // class VShadowMapPassGL : public VRenderPassGL
    // {
    //     public:
    //         VShadowMapPassGL(uint32_t shadowMapSize = 2048);
    //         virtual ~VShadowMapPassGL() = default;

    //         void Initialize() override;
    //         void Execute(VCommandBuffer* commandBuffer) override;
    //         void Cleanup() override;

    //         GLuint GetShadowMapTexture() const { return m_ShadowMapTexture; }
    //         void SetLightSpaceMatrix(const Vantor::Math::VMat4& matrix) { m_LightSpaceMatrix = matrix; }

    //     private:
    //         uint32_t m_ShadowMapSize;
    //         GLuint m_ShadowMapTexture = 0;
    //         GLuint m_ShadowMapFramebuffer = 0;
    //         Vantor::Math::VMat4 m_LightSpaceMatrix;
    //         std::unique_ptr<VShader> m_ShadowShader;
    // };

    // OpenGL 3D Render Path Implementation
    class VRenderPath3DGL : public VRenderPath3D
    {
        public:
            VRenderPath3DGL();
            virtual ~VRenderPath3DGL() = default;

            // Core rendering methods
            void Initialize(VRDevice *device) override;
            void Render() override;
            void Shutdown() override;


            // Push to CommandBufferr
            void PushRender(VMesh *mesh, Vantor::Renderer::VMaterial *material, Vantor::Math::VMat4 transform) override;
            void PushRender(Vantor::Object::VObject* object) override;

            void PushPointLight(const Vantor::Renderer::VPointLightData& pointLightData) override;

            // Render target management
            void           SetRenderTarget(VRenderTarget *target) override;
            VRenderTarget *GetRenderTarget() const override;

            // Viewport configuration
            void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
            void GetViewport(uint32_t &x, uint32_t &y, uint32_t &width, uint32_t &height) const override;

            // Render pass management
            void         AddRenderPass(std::unique_ptr<VRenderPass> pass) override;
            VRenderPass *GetRenderPass(VERenderPassType type) const override;

            // Statistics and profiling
            const VERenderStats &GetRenderStats() const override;
            void                 ResetRenderStats() override;

            // 3D specific methods
            void                      SetCamera(Vantor::Renderer::Camera *camera) override;
            Vantor::Renderer::Camera *GetCamera() const override;

            // This function is for binding UBOs and SSBOs after binding Shaders (each bool represents an internal UBO or SSBO)
            void ActivateStorage(VEStorageType storage) const override;

            // Command Buffer
            VCommandBuffer* GetCommandBuffer() override;

            // TODO
            // void EnableShadows(bool enable) override;
            // bool AreShadowsEnabled() const override;
            // void SetShadowMapSize(uint32_t size) override;

            // void SetExposure(float exposure) override;
            // void SetGamma(float gamma) override;
            // void EnableHDR(bool enable) override;
            // void EnableFXAA(bool enable) override;

            void SetWireframeMode(bool enable) override;
            void SetCullingMode(bool enable) override;

        private:
            void SetupDefaultRenderPasses();

            // Just to test
            bool m_WireframeMode  = false;
            bool m_CullingEnabled = false;

            VOpenGLCommonUBO m_CommonUBO; // Uniform Buffer Object :  This is just being used for Camera Uniforms currently
            VOpenGLLightDataUBO m_LightDataUBO; // Used to upload data like the number of pointlights
            
            // Shader Storage Buffer Objects
            // Point Light Data
            VOpenGLSSBO<Vantor::Renderer::VPointLightData> m_PointLightSSBO;

            std::unordered_map<VERenderPassType, std::unique_ptr<VRenderPass>> m_RenderPasses;
    };

    // TODO : 2D Sprite batch implementation for efficient 2D rendering
    // struct VSpriteVertex
    // {
    //     Vantor::Math::VVector3 Position;
    //     Vantor::Math::VVector2 TexCoord;
    //     Vantor::Math::VVector4 Color;
    //     float TextureIndex;
    // };

    // class VSpriteBatch
    // {
    //     public:
    //         VSpriteBatch(uint32_t maxSprites = 10000);
    //         ~VSpriteBatch();

    //         void Initialize();
    //         void Begin();
    //         void DrawSprite(VTexture* texture, const Vantor::Math::VVec2& position,
    //                       const Vantor::Math::VVec2& size, float rotation,
    //                       const Vantor::Math::VVec4& color);
    //         void DrawQuad(const Vantor::Math::VVec2& position, const Vantor::Math::VVec2& size,
    //                     const Vantor::Math::VVec4& color);
    //         void End();
    //         void Flush();
    //         void Cleanup();

    //     private:
    //         void CreateBuffers();
    //         float GetTextureIndex(VTexture* texture);

    //         uint32_t m_MaxSprites;
    //         uint32_t m_SpriteCount = 0;
    //         uint32_t m_IndexCount = 0;

    //         GLuint m_VAO = 0;
    //         GLuint m_VBO = 0;
    //         GLuint m_EBO = 0;

    //         std::vector<VSpriteVertex> m_Vertices;
    //         std::vector<uint32_t> m_Indices;
    //         std::vector<VTexture*> m_Textures;
    //         VTexture* m_WhiteTexture = nullptr;

    //         std::unique_ptr<VShader> m_SpriteShader;
    // };

    // // OpenGL 2D Render Path Implementation
    // class VRenderPath2DGL : public VRenderPath2D
    // {
    //     public:
    //         VRenderPath2DGL();
    //         virtual ~VRenderPath2DGL() = default;

    //         // Core rendering methods
    //         void Initialize(VRenderDevice* device) override;
    //         void Render(VCommandBuffer* commandBuffer) override;
    //         void Shutdown() override;

    //         // Render target management
    //         void SetRenderTarget(VRenderTarget* target) override;
    //         VRenderTarget* GetRenderTarget() const override;

    //         // Viewport configuration
    //         void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
    //         void GetViewport(uint32_t& x, uint32_t& y, uint32_t& width, uint32_t& height) const override;

    //         // Render pass management
    //         void AddRenderPass(std::unique_ptr<VRenderPass> pass) override;
    //         void RemoveRenderPass(VRenderPassType type) override;
    //         VRenderPass* GetRenderPass(VRenderPassType type) const override;

    //         // Statistics and profiling
    //         const VRenderStats& GetRenderStats() const override;
    //         void ResetRenderStats() override;

    //         // 2D specific methods
    //         void DrawSprite(VTexture* texture, const Vantor::Math::VVec2& position,
    //                       const Vantor::Math::VVec2& size, float rotation,
    //                       const Vantor::Math::VVec4& color) override;

    //         void DrawQuad(const Vantor::Math::VVec2& position, const Vantor::Math::VVec2& size,
    //                     const Vantor::Math::VVec4& color) override;

    //         void DrawText(const std::string& text, const Vantor::Math::VVec2& position,
    //                     float scale, const Vantor::Math::VVec4& color) override;

    //         void DrawLine(const Vantor::Math::VVec2& start, const Vantor::Math::VVec2& end,
    //                     float thickness, const Vantor::Math::VVec4& color) override;

    //         // Projection matrix management
    //         void SetProjectionMatrix(const Vantor::Math::VMat4& projection) override;
    //         const Vantor::Math::VMat4& GetProjectionMatrix() const override;

    //         void SetOrthographicProjection(float left, float right, float bottom, float top) override;

    //         // Batching control
    //         void BeginBatch() override;
    //         void EndBatch() override;
    //         void FlushBatch() override;

    //         // Blend mode configuration
    //         void SetBlendMode(bool enable) override;
    //         bool IsBlendModeEnabled() const override;

    //     private:
    //         void SetupDefaultShaders();
    //         void UpdateRenderStats();

    //         std::unique_ptr<VSpriteBatch> m_SpriteBatch;
    //         std::unique_ptr<VShader> m_LineShader;
    //         std::unique_ptr<VShader> m_TextShader;

    //         // Line rendering
    //         GLuint m_LineVAO = 0;
    //         GLuint m_LineVBO = 0;

    //         std::unordered_map<VRenderPassType, std::unique_ptr<VRenderPass>> m_RenderPasses;
    // };
} // namespace Vantor::RenderDevice