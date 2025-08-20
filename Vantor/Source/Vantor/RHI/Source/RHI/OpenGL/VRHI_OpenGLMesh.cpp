/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#include <RHI/OpenGL/VRHI_OpenGLMesh.hpp>

#include <iostream>

namespace Vantor::RHI
{

OpenGLMesh::OpenGLMesh(const void* vertexData, uint32_t vertexSize, const void* indexData, uint32_t indexCount, const VVertexLayout& layout)
    : m_vao(0), m_vbo(0), m_ebo(0), m_vertexCount(0), m_indexCount(indexCount), m_hasIndices(indexData != nullptr)
{
    // Calculate vertex count
    m_vertexCount = vertexSize / layout.stride;

    // Generate and bind VAO
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    // Create and fill vertex buffer
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexSize, vertexData, GL_STATIC_DRAW);

    // Create and fill index buffer if indices are provided
    if (m_hasIndices)
    {
        glGenBuffers(1, &m_ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(uint32_t), indexData, GL_STATIC_DRAW);
    }

    // Setup vertex attributes
    SetupVertexAttributes(layout);

    // Unbind VAO
    glBindVertexArray(0);
}

OpenGLMesh::~OpenGLMesh()
{
    if (m_ebo != 0)
        glDeleteBuffers(1, &m_ebo);
    if (m_vbo != 0)
        glDeleteBuffers(1, &m_vbo);
    if (m_vao != 0)
        glDeleteVertexArrays(1, &m_vao);
}

void OpenGLMesh::Bind()
{
    glBindVertexArray(m_vao);
}

void OpenGLMesh::Unbind()
{
    glBindVertexArray(0);
}

void OpenGLMesh::Draw()
{
    Draw(EPrimitiveType::Triangles);
}

void OpenGLMesh::Draw(EPrimitiveType primitiveType)
{
    GLenum glPrimitiveType = PrimitiveTypeToGL(primitiveType);
    
    if (m_hasIndices)
    {
        glDrawElements(glPrimitiveType, m_indexCount, GL_UNSIGNED_INT, 0);
    }
    else
    {
        glDrawArrays(glPrimitiveType, 0, m_vertexCount);
    }
}

void OpenGLMesh::SetupVertexAttributes(const VVertexLayout& layout)
{
    for (const auto& attribute : layout.attributes)
    {
        uint32_t componentCount = 0;
        GLenum type = GL_FLOAT;
        bool normalized = false;

        switch (attribute.format)
        {
            case ERHIFormat::R32_FLOAT:
                componentCount = 1;
                type = GL_FLOAT;
                break;
            case ERHIFormat::R32G32_FLOAT:
                componentCount = 2;
                type = GL_FLOAT;
                break;
            case ERHIFormat::R32G32B32_FLOAT:
                componentCount = 3;
                type = GL_FLOAT;
                break;
            case ERHIFormat::R8G8B8A8_UNORM:
                componentCount = 4;
                type = GL_UNSIGNED_BYTE;
                normalized = true;
                break;
            case ERHIFormat::R8G8B8_UNORM:
                componentCount = 3;
                type = GL_UNSIGNED_BYTE;
                normalized = true;
                break;
            default:
                std::cerr << "Unsupported vertex attribute format" << std::endl;
                continue;
        }

        glEnableVertexAttribArray(attribute.location);
        glVertexAttribPointer(
            attribute.location,
            componentCount,
            type,
            normalized ? GL_TRUE : GL_FALSE,
            layout.stride,
            reinterpret_cast<void*>(static_cast<uintptr_t>(attribute.offset))
        );
    }
}

GLenum OpenGLMesh::PrimitiveTypeToGL(EPrimitiveType type)
{
    switch (type)
    {
        case EPrimitiveType::Triangles: return GL_TRIANGLES;
        case EPrimitiveType::TriangleStrip: return GL_TRIANGLE_STRIP;
        case EPrimitiveType::Lines: return GL_LINES;
        case EPrimitiveType::LineStrip: return GL_LINE_STRIP;
        case EPrimitiveType::Points: return GL_POINTS;
        default: return GL_TRIANGLES;
    }
}

GLenum OpenGLMesh::RHIFormatToGL(ERHIFormat format)
{
    switch (format)
    {
        case ERHIFormat::R32_FLOAT: return GL_FLOAT;
        case ERHIFormat::R32G32_FLOAT: return GL_FLOAT;
        case ERHIFormat::R32G32B32_FLOAT: return GL_FLOAT;
        case ERHIFormat::R8G8B8A8_UNORM: return GL_UNSIGNED_BYTE;
        case ERHIFormat::R8G8B8_UNORM: return GL_UNSIGNED_BYTE;
        default: return GL_FLOAT;
    }
}

uint32_t OpenGLMesh::GetFormatSize(ERHIFormat format)
{
    switch (format)
    {
        case ERHIFormat::R32_FLOAT: return 4;
        case ERHIFormat::R32G32_FLOAT: return 8;
        case ERHIFormat::R32G32B32_FLOAT: return 12;
        case ERHIFormat::R8G8B8A8_UNORM: return 4;
        case ERHIFormat::R8G8B8_UNORM: return 3;
        default: return 4;
    }
}
}
