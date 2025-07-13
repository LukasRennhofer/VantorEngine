#pragma once

#include <Shared/glad/glad.h>

// Rendering Data
#include "../../Renderer/Camera/VRE_Camera.hpp"
#include "../../Renderer/Light/VRE_LightData.hpp"

namespace Vantor::RenderDevice {
    class VOpenGLUBO
    {
    public:
        VOpenGLUBO();
        ~VOpenGLUBO();

        void Update(
            Vantor::Renderer::Camera* camera,
            const std::vector<Vantor::Renderer::VPointLightData>& pointLights);

        void Bind() const;

    private:
        GLuint m_UBO = 0;
        static constexpr size_t FORWARD_MAX_POINT_LIGHTS = 8;

        struct UBOData
        {
            Vantor::Math::VMat4 view;
            Vantor::Math::VMat4 projection;
            Vantor::Math::VVector3 cameraPosition;
            float padding0;

            int numPointLights;
            float padding1[3]; // padding to align next array to std140 rules

            Vantor::Renderer::VPointLightData pointLights[FORWARD_MAX_POINT_LIGHTS];
        } m_Data;
    };
}