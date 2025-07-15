#include "VRE_MaterialLibrary.hpp"

#include "../Core/BackLog/VCO_Backlog.hpp" // Backlog

namespace Vantor::Renderer {
    void VMaterialLibrary::Initialize() {
        // TODO
    }
    
    void VMaterialLibrary::Shutdown() {
        m_DefaultMaterials.clear();
        m_GlobalMaterials.clear();
    }

    // TODO: Other functions tomorrow
}