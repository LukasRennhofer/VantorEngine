/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#include <AssetManager/Public/VAM_TextureAsset.hpp>

#include <RHI/Interface/VRHI_Device.hpp>

// STB Image includes
#define STB_IMAGE_IMPLEMENTATION
#include <Shared/STB/stb_image.h>

#include <iostream>
#include <filesystem>

namespace VE::Asset {

    VTextureAsset::VTextureAsset(const std::string& path)
        : VBaseAsset(path, EAssetType::Texture)
        , m_TextureData{}
        , m_RHITexture(nullptr)
    {
    }

    VTextureAsset::~VTextureAsset()
    {
        Unload();
    }

    bool VTextureAsset::Load()
    {
        if (GetState() == EAssetState::Loaded)
            return true;

        if (GetState() == EAssetState::Loading)
            return false;

        SetState(EAssetState::Loading);

        // Check if file exists
        if (!std::filesystem::exists(GetPath()))
        {
            std::cerr << "VTextureAsset::Load() - File not found: " << GetPath() << std::endl;
            SetState(EAssetState::Failed);
            return false;
        }

        // Load image data using STB
        if (!LoadSTBImage())
        {
            std::cerr << "VTextureAsset::Load() - Failed to load image: " << GetPath() << std::endl;
            SetState(EAssetState::Failed);
            return false;
        }

        SetState(EAssetState::Loaded);
        std::cout << "VTextureAsset::Load() - Successfully loaded: " << GetPath() 
                  << " (" << m_TextureData.width << "x" << m_TextureData.height 
                  << ", " << m_TextureData.channels << " channels)" << std::endl;
        
        return true;
    }

    void VTextureAsset::Unload()
    {
        FreeTextureData();
        m_RHITexture.reset();
        SetState(EAssetState::Unloaded);
    }

    bool VTextureAsset::IsValid() const
    {
        return GetState() == EAssetState::Loaded && 
               m_TextureData.pixels != nullptr && 
               m_TextureData.width > 0 && 
               m_TextureData.height > 0;
    }

    bool VTextureAsset::CreateRHITexture(VE::Internal::RHI::IRHIDevice* device)
    {
        if (!IsValid() || !device)
        {
            std::cerr << "VTextureAsset::CreateRHITexture() - Invalid texture data or device" << std::endl;
            return false;
        }

        if (m_RHITexture)
        {
            // Already created
            return true;
        }

        // Determine RHI format based on channels
        VE::Internal::RHI::ERHIFormat format;
        switch (m_TextureData.channels)
        {
            case 1:
                format = VE::Internal::RHI::ERHIFormat::R8_UNORM;
                break;
            case 3:
                format = VE::Internal::RHI::ERHIFormat::R8G8B8_UNORM;
                break;
            case 4:
                format = VE::Internal::RHI::ERHIFormat::R8G8B8A8_UNORM;
                break;
            default:
                std::cerr << "VTextureAsset::CreateRHITexture() - Unsupported channel count: " 
                          << m_TextureData.channels << std::endl;
                return false;
        }

        // Create RHI texture
        m_RHITexture = device->CreateTexture(
            m_TextureData.width,
            m_TextureData.height,
            format,
            m_TextureData.pixels,
            VE::Internal::RHI::ETextureType::Texture2D
        );

        if (!m_RHITexture)
        {
            std::cerr << "VTextureAsset::CreateRHITexture() - Failed to create RHI texture" << std::endl;
            return false;
        }

        std::cout << "VTextureAsset::CreateRHITexture() - Successfully created RHI texture for: " 
                  << GetPath() << std::endl;
        
        return true;
    }

    bool VTextureAsset::LoadSTBImage()
    {
        // Free any existing data
        FreeTextureData();

        // Set STB to flip images vertically (OpenGL expects bottom-left origin)
        stbi_set_flip_vertically_on_load(true);

        // Check if HDR
        m_TextureData.isHDR = stbi_is_hdr(GetPath().c_str());

        if (m_TextureData.isHDR)
        {
            // Load HDR image as float
            float* hdrData = stbi_loadf(GetPath().c_str(), 
                                       &m_TextureData.width, 
                                       &m_TextureData.height, 
                                       &m_TextureData.channels, 
                                       0);
            
            if (!hdrData)
            {
                std::cerr << "VTextureAsset::LoadSTBImage() - STB failed to load HDR image: " 
                          << stbi_failure_reason() << std::endl;
                return false;
            }

            // Convert float data to byte data for now (we can add HDR support later)
            size_t pixelCount = m_TextureData.width * m_TextureData.height * m_TextureData.channels;
            m_TextureData.pixels = new uint8_t[pixelCount];
            
            for (size_t i = 0; i < pixelCount; ++i)
            {
                // Simple tone mapping: clamp and convert to [0, 255]
                float value = hdrData[i];
                value = (value > 1.0f) ? 1.0f : (value < 0.0f) ? 0.0f : value;
                m_TextureData.pixels[i] = static_cast<uint8_t>(value * 255.0f);
            }

            stbi_image_free(hdrData);
        }
        else
        {
            // Load regular LDR image
            m_TextureData.pixels = stbi_load(GetPath().c_str(), 
                                           &m_TextureData.width, 
                                           &m_TextureData.height, 
                                           &m_TextureData.channels, 
                                           0);
            
            if (!m_TextureData.pixels)
            {
                std::cerr << "VTextureAsset::LoadSTBImage() - STB failed to load image: " 
                          << stbi_failure_reason() << std::endl;
                return false;
            }
        }

        return true;
    }

    void VTextureAsset::FreeTextureData()
    {
        if (m_TextureData.pixels)
        {
            if (m_TextureData.isHDR)
            {
                // We allocated this ourselves for HDR conversion
                delete[] m_TextureData.pixels;
            }
            else
            {
                // STB allocated this
                stbi_image_free(m_TextureData.pixels);
            }
            
            m_TextureData.pixels = nullptr;
        }

        m_TextureData.width = 0;
        m_TextureData.height = 0;
        m_TextureData.channels = 0;
        m_TextureData.isHDR = false;
    }

}
