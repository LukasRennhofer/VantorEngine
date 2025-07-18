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
 *  File: VRE_MaterialLibrary.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include <map>

#include "../Core/Container/VCO_SafeString.hpp" // SafeString
#include "../Core/Container/VCO_Vector.hpp" // Vector
#include "../Core/Types/VCO_ClassTypes.hpp" // VSingleton

#include "../Core/VCO_Types.hpp"

#include "VRE_Material.hpp"

#include "../Resource/VRES_Manager.hpp"

namespace Vantor::Renderer
{
    // MaterialLibrary Implementation
    class VMaterialLibrary : public Vantor::Core::Types::VSingleton<VMaterialLibrary>
    {
            friend class Vantor::Core::Types::VSingleton<VMaterialLibrary>;

        private:
            // Materials
            std::map<s32, std::shared_ptr<VMaterial>> m_DefaultMaterials;
            Vantor::Core::Vector<std::shared_ptr<VMaterial>>   m_GlobalMaterials;

        public:
            void Initialize();
            void Shutdown();

            // Sets up a default Material
            std::shared_ptr<VMaterial> CreateMaterial(const Vantor::Core::SafeString<> &child);

            // TODO:
            // VMaterial* CreateCustomMaterial(const std::string& child);

        protected:
            void SetupDefaultMaterials();
            // void SetupInternalMaterials();
    };
} // namespace Vantor::Renderer