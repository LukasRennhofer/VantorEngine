#pragma once

#include "Interface/VSTD_PanelManager.hpp"
#include "../Source/Core/Types/VCO_ClassTypes.hpp"
#include "../Source/RenderDevice/Interface/VRD_RenderTarget.hpp"
#include <string>

namespace Vantor::Studio {
    class VStudioManager :  public Vantor::Core::Types::VSingleton<VStudioManager>
    {
        friend class Vantor::Core::Types::VSingleton<VStudioManager>;

        public:
            ~VStudioManager() override;
            
            // Core functionality
            void Initialize();
            void Shutdown();
            void RenderInterface(Vantor::RenderDevice::VRenderTarget* target = nullptr);

        private:
            VStudioManager() = default;

            // Panel management
            void SetupDefaultPanels();
            
            // UI rendering
            void RenderMainMenuBar();
            void RenderDialogs();
            
            // Styling
            void SetupStyle();
            
            // GameObject creation
            void CreateEmptyGameObject();
            void CreatePrimitive(const std::string& primitiveType);
            void CreateLight(const std::string& lightType);

            // Members
            VSEditorPanelManager m_PanelManager;
            bool m_ShowAboutDialog = false;
    };
}