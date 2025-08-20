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

namespace Vantor::RHI {

OpenGLBuffer::OpenGLBuffer(ERHIBufferType type, uint32_t size, const void* data)
    : OpenGLBuffer(type, size, EBufferUsage::Static, data)
{
}

OpenGLBuffer::OpenGLBuffer(ERHIBufferType type, uint32_t size, EBufferUsage usage, const void* data)
    : m_buffer(0), m_size(size), m_type(type), m_usage(usage), m_mapped(false)
{
    glGenBuffers(1, &m_buffer);
    glBindBuffer(BufferTypeToGL(m_type), m_buffer);
    glBufferData(BufferTypeToGL(m_type), m_size, data, BufferUsageToGL(m_usage));
    glBindBuffer(BufferTypeToGL(m_type), 0);
}

OpenGLBuffer::~OpenGLBuffer()
{
    if (m_mapped)
    {
        Unmap();
    }
    
    if (m_buffer != 0)
    {
        glDeleteBuffers(1, &m_buffer);
    }
}

void OpenGLBuffer::Bind()
{
    glBindBuffer(BufferTypeToGL(m_type), m_buffer);
}

void OpenGLBuffer::Unbind()
{
    glBindBuffer(BufferTypeToGL(m_type), 0);
}

void OpenGLBuffer::UpdateData(const void* data, uint32_t size, uint32_t offset)
{
    if (offset + size > m_size)
    {
        
        std::cerr << "Buffer update out of bounds: offset=" << offset 
                  << ", size=" << size << ", buffer_size=" << m_size << std::endl;
        return;
    }

    glBindBuffer(BufferTypeToGL(m_type), m_buffer);
    
    if (offset == 0 && size == m_size)
    {
        // Replace entire buffer
        glBufferData(BufferTypeToGL(m_type), m_size, data, BufferUsageToGL(m_usage));
    }
    else
    {
        // Update partial buffer
        glBufferSubData(BufferTypeToGL(m_type), offset, size, data);
    }
    
    glBindBuffer(BufferTypeToGL(m_type), 0);
}

void* OpenGLBuffer::Map()
{
    if (m_mapped)
    {
        std::cerr << "Buffer is already mapped" << std::endl;
        return nullptr;
    }

    glBindBuffer(BufferTypeToGL(m_type), m_buffer);
    void* ptr = glMapBuffer(BufferTypeToGL(m_type), GL_READ_WRITE);
    
    if (ptr != nullptr)
    {
        m_mapped = true;
    }
    else
    {
        std::cerr << "Failed to map buffer" << std::endl;
    }
    
    return ptr;
}

void OpenGLBuffer::Unmap()
{
    if (!m_mapped)
    {
        std::cerr << "Buffer is not mapped" << std::endl;
        return;
    }

    glBindBuffer(BufferTypeToGL(m_type), m_buffer);
    
    if (glUnmapBuffer(BufferTypeToGL(m_type)) == GL_FALSE)
    {
        std::cerr << "Buffer data was corrupted during mapping" << std::endl;
    }
    
    glBindBuffer(BufferTypeToGL(m_type), 0);
    m_mapped = false;
}

GLenum OpenGLBuffer::BufferTypeToGL(ERHIBufferType type)
{
    switch (type)
    {
        case ERHIBufferType::Vertex: return GL_ARRAY_BUFFER;
        case ERHIBufferType::Index: return GL_ELEMENT_ARRAY_BUFFER;
        case ERHIBufferType::Uniform: return GL_UNIFORM_BUFFER;
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
    
}