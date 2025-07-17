/*
 *  ╔═══════════════════════════════════════════════════════════════╗
 *  ║                          ~ Vantor ~                           ║
 *  ║                                                               ║
 *  ║  This file is part of the Vantor Engine.                      ║
 *  ║  Automatically formatted by vtrgFormat.py                     ║
 *  ║                                                               ║
 *  ╚═══════════════════════════════════════════════════════════════╝
 *
 *  Copyright (c) 2025 Lukas Rennhofer
 *  Licensed under the GNU General Public License, Version 3.
 *  See LICENSE file for more details.
 *
 *  Author: Lukas Rennhofer
 *  Date: 2025-07-16
 *
 *  File: VRE_MaterialLibrary.cpp
 *  Last Change: Automatically updated
 */

#include "../Core/BackLog/VCO_Backlog.hpp" // Backlog
#include "VRE_MaterialLibrary.hpp"
#include "../Core/BackLog/VCO_Backlog.hpp" // Backlog

namespace Vantor::Renderer
{
    void VMaterialLibrary::Initialize()
    {
        SetupDefaultMaterials();
    }

    void VMaterialLibrary::Shutdown()
    {
        m_DefaultMaterials.clear();
        m_GlobalMaterials.clear();
    }

    std::shared_ptr<VMaterial> VMaterialLibrary::CreateMaterial(const Vantor::Core::SafeString<> &child) {
        auto found = m_DefaultMaterials.find(child.hash());

        if (found != m_DefaultMaterials.end())
        {
            VMaterial copy = found->second->Copy();
            auto mat = std::make_shared<VMaterial>(copy);
            m_GlobalMaterials.push_back(mat);
            return mat;
        }
    }

    void VMaterialLibrary::SetupDefaultMaterials() {
        // ===== Default GBuffer Material (Deffered Rendered Objects) ======
        auto GBufferShader = Vantor::Resource::VResourceManager::Instance().GetShaderProgramData("VIShaderDeffered");
        auto DefferedMaterial = std::make_shared<VMaterial>(GBufferShader.get());
        // Add to map
        m_DefaultMaterials[Vantor::Core::SafeString<>("VDefault").hash()] = DefferedMaterial;
    }

    // TODO: Other functions tomorrow
} // namespace Vantor::Renderer