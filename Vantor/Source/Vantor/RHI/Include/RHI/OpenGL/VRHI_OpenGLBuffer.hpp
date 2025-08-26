/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

#include <RHI/Interface/VRHI_Buffer.hpp>
#include <RHI/Interface/VRHI_Device.hpp>

#include <Shared/glad/glad.h>

namespace VE::Internal::RHI
{

class OpenGLBuffer : public IRHIBuffer
{
public:
    OpenGLBuffer(ERHIBufferType type, uint32_t size, const void* data = nullptr);
    OpenGLBuffer(ERHIBufferType type, uint32_t size, EBufferUsage usage, const void* data = nullptr);
    virtual ~OpenGLBuffer();

    // IRHIBuffer implementation
    void Bind() override;
    void Bind(uint32_t slot) override;
    void Unbind() override;
    void UpdateData(const void* data, uint32_t size, uint32_t offset = 0) override;
    void* Map() override;
    void Unmap() override;

    uint32_t GetSize() const override { return m_size; }
    uint32_t GetHandle() const override { return m_buffer; }

    // OpenGL specific
    ERHIBufferType GetType() const { return m_type; }

    // Static utility functions
    static GLenum BufferTypeToGL(ERHIBufferType type);
    static GLenum BufferUsageToGL(EBufferUsage usage);

private:
    uint32_t m_buffer;
    uint32_t m_size;
    ERHIBufferType m_type;
    EBufferUsage m_usage;
    bool m_mapped;
};

}