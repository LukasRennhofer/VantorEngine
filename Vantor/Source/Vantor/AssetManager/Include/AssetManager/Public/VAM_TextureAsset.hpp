/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

#include <AssetManager/Public/VAM_Asset.hpp>

#include <RHI/Interface/VRHI_Device.hpp>

#include <memory>

namespace VE::Asset {

    struct VTextureData {
        uint8_t* pixels = nullptr;
        int width = 0;
        int height = 0;
        int channels = 0;
        bool isHDR = false;
    };

    class VTextureAsset : public VBaseAsset {
    public:
        VTextureAsset(const std::string& path);
        virtual ~VTextureAsset();

        // Override VBaseAsset methods
        bool Load() override;
        void Unload() override;
        bool IsValid() const override;

        // Texture-specific getters
        const VTextureData& GetTextureData() const { return m_TextureData; }
        std::shared_ptr<VE::Internal::RHI::IRHITexture> GetRHITexture() const { return m_RHITexture; }

        // Texture properties
        int GetWidth() const { return m_TextureData.width; }
        int GetHeight() const { return m_TextureData.height; }
        int GetChannels() const { return m_TextureData.channels; }
        bool IsHDR() const { return m_TextureData.isHDR; }

        // Create RHI texture from loaded data
        bool CreateRHITexture(VE::Internal::RHI::IRHIDevice* device);

    private:
        VTextureData m_TextureData;
        std::shared_ptr<VE::Internal::RHI::IRHITexture> m_RHITexture;

        bool LoadSTBImage();
        void FreeTextureData();
    };

    using TextureAssetPtr = TypedAssetPtr<VTextureAsset>;

}
