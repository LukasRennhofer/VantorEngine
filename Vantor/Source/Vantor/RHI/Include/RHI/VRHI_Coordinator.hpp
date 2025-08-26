/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

#include <RHI/Interface/VRHI_Device.hpp>

#ifdef VANTOR_API_OPENGL
#include <RHI/OpenGL/VRHI_OpenGLDevice.hpp>
#endif

#include <Core/Types/VCO_Singleton.hpp>

#include <memory>
#include <iostream>

namespace VE::Internal::RHI {

class VRDCoordinator : public VE::Internal::Core::Types::TSingleton<VRDCoordinator>
{
    friend class VE::Internal::Core::Types::TSingleton<VRDCoordinator>; // Allow singleton access

public:
    std::shared_ptr<IRHIDevice> CreateDevice(EGraphicsAPI api)
    {
        switch (api)
        {
            case EGraphicsAPI::OPENGL:
            #ifdef VANTOR_API_OPENGL
                return std::make_shared<OpenGLDevice>();
            #else
                std::cout << "OpenGL support not compiled in!" << std::endl;
                return nullptr;
            #endif

            default:
                std::cout << "Unknown RHI API requested!" << std::endl;
                return nullptr;
        }
    }

private:
    VRDCoordinator() = default;
};

} // namespace VE::Internal::RHI



