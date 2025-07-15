#pragma once

#include <Shared/glad/glad.h>

// Rendering Data
#include "../../Renderer/Camera/VRE_Camera.hpp"

namespace Vantor::RenderDevice {
    // TODO: Remove update function from VOpenGLLightDataUBO
    class VOpenGLUBO
    {
    public:
        virtual ~VOpenGLUBO() {
            if (m_Buffer != 0) {
                glDeleteBuffers(1, &m_Buffer);
                m_Buffer = 0;
            }
        }

        // Bind buffer to a binding point
        virtual void Bind(GLuint bindingPoint) const = 0;

    protected:
        GLuint m_Buffer = 0;
    };

    // Common UBO (Used for Camera)
    class VOpenGLCommonUBO : public VOpenGLUBO
    {
    public:
        VOpenGLCommonUBO();
        ~VOpenGLCommonUBO() override;

        void Update(Vantor::Renderer::Camera* Camera);

        void Bind(GLuint bindingPoint = 0) const override;


    private:
        struct UBOData
        {
            Vantor::Math::VMat4 view;
            Vantor::Math::VMat4 projection;
            Vantor::Math::VVector3 cameraPosition;
            float padding0;
        } m_Data;
    };

    // LightData UBO (Used for LightCount in Lighting Pass)
    class VOpenGLLightDataUBO : public VOpenGLUBO
    {
    public:
        VOpenGLLightDataUBO();
        ~VOpenGLLightDataUBO() override;

        void Update();

        void Bind(GLuint bindingPoint = 0) const override;

        void UploadLightData(int numPointLights);

    private:

        struct UBOData
        {
            int numPointLights;
        } m_Data;
    };
}