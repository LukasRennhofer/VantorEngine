/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

#include <RHI/Interface/VRHI_Device.hpp>
#include <RHI/Interface/VRHI_Shader.hpp>
#include <RHI/Interface/VRHI_Texture.hpp>
#include <RHI/Interface/VRHI_Mesh.hpp>
#include <RHI/Interface/VRHI_Buffer.hpp>
#include <RHI/Interface/VRHI_RenderTarget.hpp>

#include <Shared/glad/glad.h>

#include <memory>
#include <unordered_map>

namespace VE::Internal::RHI
{

class OpenGLDevice : public IRHIDevice
{
public:
    OpenGLDevice();
    virtual ~OpenGLDevice();

    // IRHIDevice implementation
    bool Initialize() override;
    void Shutdown() override;
    void Present() override;
    void Clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f) override;

    std::shared_ptr<IRHIShader> CreateShader(const std::string& vertexSource, const std::string& fragmentSource) override;
    std::shared_ptr<IRHITexture> CreateTexture(uint32_t width, uint32_t height, ERHIFormat format, const void* data = nullptr, ETextureType type = ETextureType::Texture2D, uint32_t depth = 1) override;
    std::shared_ptr<IRHIMesh> CreateMesh(const void* vertexData, uint32_t vertexSize, const void* indexData, uint32_t indexCount, const VVertexLayout& layout) override;
    std::shared_ptr<IRHIBuffer> CreateBuffer(ERHIBufferType type, uint32_t size, const void* data = nullptr) override;
    std::shared_ptr<IRHIRenderTarget> CreateRenderTarget(uint32_t width, uint32_t height, uint32_t samples = 1) override;

    void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
    // void BindShader(std::shared_ptr<IRHIShader> shader) override;
    // void BindTexture(std::shared_ptr<IRHITexture> texture, uint32_t slot = 0) override;
    // void BindRenderTarget(std::shared_ptr<IRHIRenderTarget> renderTarget) override;
    // void DrawMesh(std::shared_ptr<IRHIMesh> mesh) override;

    // OpenGL specific methods
    static GLenum RHIFormatToGL(ERHIFormat format);
    static GLenum BufferTypeToGL(ERHIBufferType type);

private:
    bool m_initialized;
    std::shared_ptr<IRHIShader> m_currentShader;
    std::shared_ptr<IRHIRenderTarget> m_currentRenderTarget;
    std::unordered_map<uint32_t, std::shared_ptr<IRHITexture>> m_boundTextures;

    void SetupDefaultState();
};

} // namespace VE::Internal::RHI
