/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

#include <cstdint>
#include <memory>

namespace VE::Internal::RHI
{

class IRHIBuffer; // forward declaration, because why not

enum class EPrimitiveType
{
    Triangles,
    TriangleStrip,
    Lines,
    LineStrip,
    Points
};

class IRHIMesh
{
public:
    virtual ~IRHIMesh() = default;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;
    virtual void Draw() = 0;
    virtual void Draw(EPrimitiveType primitiveType) = 0;

    virtual uint32_t GetVertexCount() const = 0;
    virtual uint32_t GetIndexCount() const = 0;

    // IDK why I am exposing the buffers so early 
    // but i guess Terry A. Davis would want it
    virtual std::shared_ptr<IRHIBuffer> GetVertexBuffer() const = 0;
    virtual std::shared_ptr<IRHIBuffer> GetIndexBuffer() const = 0;
};

} // namespace VE::Internal::RHI
