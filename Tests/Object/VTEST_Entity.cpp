// Common
#include "../VTEST_Common.hpp"

// Includes
#include "../../Vantor/Source/ObjectSystem/VOS_Object.hpp"
#include "../../Vantor/Source/ObjectSystem/Component/VOS_Component.hpp"
#include "../../Vantor/Source/ObjectSystem/Register/VOS_Registry.hpp"

VTEST_CASE("ObjectSystem", VTestObjectCreation) {
    // Parent Entity
    auto entityOne = Vantor::Object::VORegistry::CreateEntity<Vantor::Object::VObject>();
    VASSERT(entityOne->id == 0, "ID of first entity is not set to 0");

    // Child Entity
    auto entityTwo = Vantor::Object::VORegistry::CreateEntity<Vantor::Object::VObject>();
    VASSERT(entityOne->id == 1, "ID of second entity is not set to 1");

    // Build up Hierachy
    entityOne->AddChild(entityTwo);

    // Verify if child is the right one
    VASSERT(entityOne->GetChildren()[0]->id == 1, "Failed to set children for parent entity");
}