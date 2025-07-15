#pragma once

#include <map>

#include "../Core/Container/VCO_Vector.hpp" // Vector
#include "../Core/Types/VCO_BTypes.hpp" // VSingleton

#include "VRE_Material.hpp"

namespace Vantor::Renderer {
    // MaterialLibrary Implementation
    class VMaterialLibrary : public Vantor::Core::Types::VSingleton<VMaterialLibrary>
    {
        friend class Vantor::Core::Types::VSingleton<VMaterialLibrary>;

        private:
            // Materials
            std::map<unsigned int, std::shared_ptr<VMaterial>> m_DefaultMaterials;
            Vantor::Core::Vector<std::shared_ptr<VMaterial>> m_GlobalMaterials;

            // RenderPath Shaders
            std::shared_ptr<Vantor::RenderDevice::VShader> m_LightingShader;
        
        public:
            void Initialize();
            void Shutdown();

            // Sets up a default Material
            std::shared_ptr<VMaterial> CreateMaterial(const std::string& child);

            std::shared_ptr<Vantor::RenderDevice::VShader> InternalGetLightShader() { return m_LightingShader; }

            // TODO:
            // VMaterial* CreateCustomMaterial(const std::string& child);

        protected:
            void SetupDefaultMaterials();
            void SetupInternalMaterials();
    };     
}