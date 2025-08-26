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

namespace VE::Internal::RHI
{

enum class ETextureType; // Forward Decl.
enum class ERHIFormat; // Forward Decl.

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

    // For 2D/3D/1D/Cube
    virtual void UpdateData(const void* data, uint32_t width, uint32_t height = 1, uint32_t depth = 1, uint32_t face = 0) = 0;

    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;
    virtual uint32_t GetDepth() const { return 1; } // Default slices for 2D/1D Textures
    virtual uint32_t GetHandle() const = 0;
    virtual ERHIFormat GetFormat() const = 0;
    virtual ETextureType GetType() const = 0;

    virtual void Resize(uint32_t newWidth, uint32_t newHeight, uint32_t newDepth = 1) = 0;

    virtual void SetFilter(ETextureFilter minFilter, ETextureFilter magFilter) = 0;
    virtual void SetWrap(ETextureWrap wrapS, ETextureWrap wrapT, ETextureWrap wrapR = ETextureWrap::Repeat) = 0;
};

} // namespace VE::Internal::RHI
