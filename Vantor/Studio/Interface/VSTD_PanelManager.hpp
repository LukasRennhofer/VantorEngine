#pragma once

#include <vector>
#include <memory>

#include "VSTD_Panel.hpp"

namespace Vantor::Studio {

    class VSEditorPanelManager {
    public:
        void RegisterPanel(std::unique_ptr<VSEditorPanel> panel);
        void OnImGuiRenderAll();

        template<typename T>
        T* GetPanel() {
            for (auto& panel : panels) {
                if (auto casted = dynamic_cast<T*>(panel.get()))
                    return casted;
            }
            return nullptr;
        }

    private:
        std::vector<std::unique_ptr<VSEditorPanel>> panels;
    };

}