#pragma once

#include "../Interface/VSTD_Panel.hpp"

#include <Shared/imgui/imgui.h>
#include <Studio/Gizmo/imoguizmo.hpp>

#include <memory>

namespace Vantor::Studio {
    class VSViewportPanel : public VSEditorPanel {
        public:
            VSViewportPanel() {
                m_isTitleBarVisible = false;
            }
            
            virtual ~VSViewportPanel() = default;

            void OnImGuiRender() override {                
                // Get the current window position and size for the viewport
                ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
                ImVec2 viewportMinRegion = ImGui::GetWindowContentRegionMin();
                ImVec2 viewportMaxRegion = ImGui::GetWindowContentRegionMax();
                ImVec2 viewportOffset = ImGui::GetWindowPos();
                
                // Calculate actual viewport bounds
                ImVec2 viewportMin = ImVec2(viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y);
                ImVec2 viewportMax = ImVec2(viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y);
                
                // Render the viewport image
                ImGui::Image((void*)(intptr_t)texture, ImVec2((float)m_width, (float)m_height), ImVec2(0, 1), ImVec2(1, 0));
                
                // Setup and draw gizmo in the corner of the viewport
                const float gizmoSize = 100.0f;
                const float padding = 10.0f;
                
                // Position gizmo in top-right corner of viewport
                ImOGuizmo::SetRect(
                    viewportMax.x - gizmoSize - padding, 
                    viewportMin.y + padding, 
                    gizmoSize
                );
                
                ImOGuizmo::SetDrawList();
                
                // Draw the gizmo - it will handle its own interaction
                bool gizmoInteracted = ImOGuizmo::DrawGizmo(
                    m_ViewMatrix.Data(), 
                    m_ProjectionMatrix.Data(), 
                    5.0f  // pivot distance for interactivity
                );
            }
            const char* GetName() const override { return "Viewport"; }

            void SetRenderTarget(Vantor::RenderDevice::VRenderTarget* target) { m_OutBuffer = target; }

        private:
            Vantor::RenderDevice::VRenderTarget* m_OutBuffer;

            unsigned int m_width = 1920;
            unsigned int m_height = 1080;

            // Camera matrices for 3D rendering
            Vantor::Math::VMat4 m_ViewMatrix = Vantor::Math::VMat4::Identity();
            Vantor::Math::VMat4 m_ProjectionMatrix = Vantor::Math::VMat4::Identity();
    };
}