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

enum class ETextureFormat
{
    RGB8,
    RGBA8,
    RGB16F,
    RGBA16F,
    RGB32F,
    RGBA32F,
    Depth24Stencil8
};

enum class ETextureFilter
{
    Nearest,
    Linear
};

enum class ETextureWrap
{
    Repeat,
    ClampToEdge,
    ClampToBorder
};

class IRHITexture
{
public:
    virtual ~IRHITexture() = default;

    virtual void Bind(uint32_t slot = 0) = 0;
    virtual void Unbind() = 0;
    virtual void UpdateData(const void* data, uint32_t width, uint32_t height) = 0;
    
    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;
    virtual uint32_t GetHandle() const = 0;
    virtual ETextureFormat GetFormat() const = 0;

    virtual void SetFilter(ETextureFilter minFilter, ETextureFilter magFilter) = 0;
    virtual void SetWrap(ETextureWrap wrapS, ETextureWrap wrapT) = 0;
};

} // namespace Vantor::RHI
