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
#include <Graphics/Public/Model/VGFX_Model.hpp>

#include <memory>

namespace VE::Internal::RHI {
    class IRHIDevice;
}

namespace VE::Asset {

    class VModelAsset : public VBaseAsset {
    public:
        VModelAsset(const std::string& path);
        virtual ~VModelAsset();

        // Override VBaseAsset methods
        bool Load() override;
        void Unload() override;
        bool IsValid() const override;

        // Model-specific getters
        std::shared_ptr<VE::Graphics::VModel> GetModel() const { return m_Model; }
        const VE::Graphics::VModel* GetModelPtr() const { return m_Model.get(); }

        // GPU resource management
        bool CreateGPUResources(VE::Internal::RHI::IRHIDevice* device);
        void DestroyGPUResources();
        bool HasGPUResources() const { return m_HasGPUResources; }

        // Model properties
        uint32_t GetMeshCount() const;
        const std::string& GetModelPath() const;

    private:
        std::shared_ptr<VE::Graphics::VModel> m_Model;
        bool m_HasGPUResources = false;

        // Helper methods
        bool IsModelFile(const std::string& path) const;
    };

    using ModelAssetPtr = TypedAssetPtr<VModelAsset>;

}
