#pragma once

#include "../Interface/VSTD_Panel.hpp"
#include "../../Source/ObjectSystem/VOS_Object.hpp"
#include "../../Source/ObjectSystem/Register/VOS_Registry.hpp"
#include "../../Source/ObjectSystem/Component/VOS_Base.hpp"

#include <memory>

namespace Vantor::Studio {
    class VScenePanel : public VSEditorPanel {
        public:
            VScenePanel();
            virtual ~VScenePanel() = default;

            void OnImGuiRender() override;
            const char* GetName() const override { return "Scene Hierarchy"; }

        private:
            void DrawObjectNode(std::shared_ptr<Vantor::Object::VObject> object);
            void DrawObjectPropertiesPanel();
            void DrawComponents(std::shared_ptr<Vantor::Object::VObject> object);
            void DrawTransformComponent(Vantor::Object::VTransformComponent* transform);
            void DrawMaterialComponent(Vantor::Object::VMaterialComponent* material);
            void DrawMeshComponent(Vantor::Object::VMeshComponent* mesh);
            void DrawTagComponent(Vantor::Object::VTagComponent* tag);

            std::shared_ptr<Vantor::Object::VObject> m_SelectedObject = nullptr;
            bool m_ShowProperties = true;
    };
}