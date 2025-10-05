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

#include <string>
#include <memory>

namespace VE::Internal::RHI {
    class IRHIDevice;
}

namespace VE::Asset {

    class VTextAsset : public VBaseAsset {
    public:
        VTextAsset(const std::string& path);
        virtual ~VTextAsset();

        // Override VBaseAsset methods
        bool Load() override;
        void Unload() override;
        bool IsValid() const override;

        std::string GetText() const { return m_Text; }

    private:
        
        std::string m_Text;

        // Helper methods
        bool IsTextFile(const std::string& path) const;
    };

    using TextAssetPtr = TypedAssetPtr<VTextAsset>;
}
