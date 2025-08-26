/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#include <RHI/OpenGL/VRHI_OpenGLBuffer.hpp>
#include <Core/BackLog/VCO_Backlog.hpp>

#include <array>

namespace VE::Internal::RHI {

static std::array<GLuint, 4> sBoundBuffers = {0, 0, 0, 0}; // cache for Vertex, Index, Uniform, SSBO

OpenGLBuffer::OpenGLBuffer(ERHIBufferType type, uint32_t size, const void* data)
    : OpenGLBuffer(type, size, EBufferUsage::Static, data)
{
}

OpenGLBuffer::OpenGLBuffer(ERHIBufferType type, uint32_t size, EBufferUsage usage, const void* data)
    : m_buffer(0), m_size(size), m_type(type), m_usage(usage), m_mapped(false)
{
    glGenBuffers(1, &m_buffer);

    GLenum target = BufferTypeToGL(m_type);
    Bind();

    glBufferData(target, m_size, data, BufferUsageToGL(m_usage));

    Unbind();
}

OpenGLBuffer::~OpenGLBuffer()
{
    if (m_mapped)
        Unmap();

    if (m_buffer != 0)
        glDeleteBuffers(1, &m_buffer);
}

// Bind buffer and update cache
void OpenGLBuffer::Bind()
{
    GLenum target = BufferTypeToGL(m_type);
    if (sBoundBuffers[static_cast<int>(m_type)] != m_buffer)
    {
        glBindBuffer(target, m_buffer);
        sBoundBuffers[static_cast<int>(m_type)] = m_buffer;
    }
}

// Unbind buffer and update cache
void OpenGLBuffer::Unbind()
{
    GLenum target = BufferTypeToGL(m_type);
    glBindBuffer(target, 0);
    sBoundBuffers[static_cast<int>(m_type)] = 0;
}

// Bind buffer to a specific binding point (for UBO/SSBO)
void OpenGLBuffer::Bind(uint32_t slot)
{
    Bind();
    if (m_type == ERHIBufferType::Uniform)
        glBindBufferBase(GL_UNIFORM_BUFFER, slot, m_buffer);
    else if (m_type == ERHIBufferType::ShaderStorage)
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, slot, m_buffer);
}

void OpenGLBuffer::UpdateData(const void* data, uint32_t size, uint32_t offset)
{
    if (offset + size > m_size)
    {
        std::cerr << "Buffer update out of bounds: offset=" << offset
                  << ", size=" << size << ", buffer_size=" << m_size << std::endl;
        return;
    }

    Bind();

    if (offset == 0 && size == m_size)
    {
        glBufferData(BufferTypeToGL(m_type), m_size, data, BufferUsageToGL(m_usage));
    }
    else
    {
        glBufferSubData(BufferTypeToGL(m_type), offset, size, data);
    }
}

// Map buffer safely (using glMapBufferRange for dynamic usage)
void* OpenGLBuffer::Map()
{
    if (m_mapped)
    {
        std::cerr << "Buffer is already mapped" << std::endl;
        return nullptr;
    }

    Bind();

    GLenum access = GL_MAP_READ_BIT | GL_MAP_WRITE_BIT;
    if (m_usage != EBufferUsage::Static)
        access |= GL_MAP_INVALIDATE_BUFFER_BIT; // Avoid GPU stalls

    void* ptr = glMapBufferRange(BufferTypeToGL(m_type), 0, m_size, access);

    if (!ptr)
    {
        std::cerr << "Failed to map buffer" << std::endl;
        return nullptr;
    }

    m_mapped = true;
    return ptr;
}

void OpenGLBuffer::Unmap()
{
    if (!m_mapped)
    {
        std::cerr << "Buffer is not mapped" << std::endl;
        return;
    }

    Bind();

    if (glUnmapBuffer(BufferTypeToGL(m_type)) == GL_FALSE)
        std::cerr << "Buffer data was corrupted during mapping" << std::endl;

    m_mapped = false;
}

GLenum OpenGLBuffer::BufferTypeToGL(ERHIBufferType type)
{
    switch (type)
    {
        case ERHIBufferType::Vertex: return GL_ARRAY_BUFFER;
        case ERHIBufferType::Index: return GL_ELEMENT_ARRAY_BUFFER;
        case ERHIBufferType::Uniform: return GL_UNIFORM_BUFFER;
        case ERHIBufferType::ShaderStorage: return GL_SHADER_STORAGE_BUFFER;
        default: return GL_ARRAY_BUFFER;
    }
}

GLenum OpenGLBuffer::BufferUsageToGL(EBufferUsage usage)
{
    switch (usage)
    {
        case EBufferUsage::Static: return GL_STATIC_DRAW;
        case EBufferUsage::Dynamic: return GL_DYNAMIC_DRAW;
        case EBufferUsage::Stream: return GL_STREAM_DRAW;
        default: return GL_STATIC_DRAW;
    }
}

} // namespace VE::Internal::RHI
