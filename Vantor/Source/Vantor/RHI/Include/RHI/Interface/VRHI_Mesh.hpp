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

namespace Vantor::RHI
{

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
};

} // namespace Vantor::RHI
