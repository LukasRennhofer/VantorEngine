#include "VSTD_PanelManager.hpp"

#include <Shared/imgui/imgui.h>

namespace Vantor::Studio {
    void VSEditorPanelManager::RegisterPanel(std::unique_ptr<VSEditorPanel> panel) {
        panels.push_back(std::move(panel));
    }

    void VSEditorPanelManager::OnImGuiRenderAll() {
        bool isOpen = false;
        for (auto& panel : panels) {
            isOpen = panel->isVisible();
            if (isOpen) {
                if (panel->isTitleBarVisible()) {
                    ImGui::Begin(panel->GetName(), &isOpen);
                } else {
                    // For panels without title bar (like viewport), remove padding and borders for a cleaner look
                    ImGui::Begin(panel->GetName(), &isOpen, 
                        ImGuiWindowFlags_NoTitleBar | 
                        ImGuiWindowFlags_NoCollapse |
                        ImGuiWindowFlags_NoScrollbar |
                        ImGuiWindowFlags_NoScrollWithMouse);
                }
                panel->OnImGuiRender();
                ImGui::End();
            }
        }
    }

}