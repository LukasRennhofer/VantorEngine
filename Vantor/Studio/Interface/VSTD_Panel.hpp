#pragma once

#include <string>

namespace Vantor::Studio {
    class VSEditorPanel {
        public:
            virtual ~VSEditorPanel() = default;

            virtual void OnImGuiRender() = 0;
            virtual const char* GetName() const = 0;

            bool isVisible() { return m_isVisible; }
            bool isTitleBarVisible() {return m_isTitleBarVisible; }

        protected:
            bool m_isVisible = true;
            bool m_isTitleBarVisible = true;
    };
}