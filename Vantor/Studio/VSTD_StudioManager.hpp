#pragma once

#include "Interface/VSTD_PanelManager.hpp"
#include <Core/Types/VCO_Singleton.hpp>
#include <string>

namespace Vantor::Studio {
    class VStudioManager :  public Vantor::Core::Types::TSingleton<VStudioManager>
    {
        friend class Vantor::Core::Types::TSingleton<VStudioManager>;

        public:
            ~VStudioManager() override;
            
            // Core functionality
            void Initialize();
            void Shutdown();
            void RenderInterface();

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