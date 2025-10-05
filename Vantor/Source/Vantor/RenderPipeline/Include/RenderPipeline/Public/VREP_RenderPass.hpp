/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

#include <memory>
#include <cstdint>

namespace VE::Internal::RHI {
    class IRHIRenderTarget;
}

namespace VE::Render {

    class VRenderPath;

    enum class ERenderPassType : uint8_t
    {
        VD_Geometry,
        VF_Forward,
        Custom
    };
    // Base render pass interface
    class VRenderPass
    {
        public:
            virtual ~VRenderPass() = default;

            void Initialize(VRenderPath* renderPath);

            // Virtual Functions to override
            virtual void Execute() = 0;
            virtual void Cleanup() = 0;

            virtual ERenderPassType GetType() const = 0;

            // Enable/disable the pass
            void SetEnabled(bool enabled) { m_Enabled = enabled; }
            bool IsEnabled() const { return m_Enabled; }

            // Set Input/Get Ouput of the Pass
            void SetInputTarget(VE::Internal::RHI::IRHIRenderTarget* input) { m_IN  = input; }
            VE::Internal::RHI::IRHIRenderTarget* GetOutputTarget() { return m_OUT; }

        protected:
            bool m_Enabled = true;

            VE::Internal::RHI::IRHIRenderTarget* m_IN = nullptr;
            VE::Internal::RHI::IRHIRenderTarget* m_OUT = nullptr;

            VRenderPath* m_RenderPath;
    };
}