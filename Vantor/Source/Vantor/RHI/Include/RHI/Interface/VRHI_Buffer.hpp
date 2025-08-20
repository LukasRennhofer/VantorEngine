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

enum class EBufferUsage
{
    Static,    // Data won't change
    Dynamic,   // Data will change occasionally
    Stream     // Data will change every frame
};

class IRHIBuffer
{
public:
    virtual ~IRHIBuffer() = default;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;
    virtual void UpdateData(const void* data, uint32_t size, uint32_t offset = 0) = 0;
    virtual void* Map() = 0;
    virtual void Unmap() = 0;

    virtual uint32_t GetSize() const = 0;
    virtual uint32_t GetHandle() const = 0;
};

} // namespace Lab::RHI
