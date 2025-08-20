/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

#include <RHI/Interface/VRHI_Mesh.hpp>
#include <RHI/Interface/VRHI_Device.hpp>

#include <Shared/glad/glad.h>

// TODO: Implement with Math Module

namespace Vantor::RHI
{

class OpenGLMesh : public IRHIMesh
{
public:
    OpenGLMesh(const void* vertexData, uint32_t vertexSize, const void* indexData, uint32_t indexCount, const VVertexLayout& layout);
    virtual ~OpenGLMesh();

    // IRHIMesh implementation
    void Bind() override;
    void Unbind() override;
    void Draw() override;
    void Draw(EPrimitiveType primitiveType) override;

    uint32_t GetVertexCount() const override { return m_vertexCount; }
    uint32_t GetIndexCount() const override { return m_indexCount; }

    // Static utility functions
    static GLenum PrimitiveTypeToGL(EPrimitiveType type);
    static GLenum RHIFormatToGL(ERHIFormat format);
    static uint32_t GetFormatSize(ERHIFormat format);

private:
    uint32_t m_vao;
    uint32_t m_vbo;
    uint32_t m_ebo;
    uint32_t m_vertexCount;
    uint32_t m_indexCount;
    bool m_hasIndices;

    void SetupVertexAttributes(const VVertexLayout& layout);
};

} // namespace Lab
