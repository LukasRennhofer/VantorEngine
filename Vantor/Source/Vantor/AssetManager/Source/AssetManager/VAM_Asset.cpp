/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#include <AssetManager/Public/VAM_Asset.hpp>

namespace VE::Asset {

    VBaseAsset::VBaseAsset(const std::string& path, EAssetType type)
        : m_Path(path)
        , m_Type(type)
        , m_State(EAssetState::Unloaded)
        , m_RefCount(0)
    {
    }

}
