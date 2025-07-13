#pragma once

#include <Shared/glad/glad.h>

#include "../../Core/Types/VCO_BTypes.hpp" // Singleton

// Idea and States from Cell Engine @JoeyDeVries

namespace Vantor::RenderDevice {

    // This is a Singleton, because it can be used widely in the whole OpenGLRenderDevice to reduce and track GPU Uploads
    class VOpenGLStateChache : public Vantor::Core::Types::VSingleton<VOpenGLStateChache>
    {
            friend class Vantor::Core::Types::VSingleton<VOpenGLStateChache>;
    private:
        // State Cache Toggles
        bool m_DepthTest = false;
        bool m_Blend = false;
        bool m_CullFace = false;

        GLenum m_DepthFunc = GL_LESS;
        GLenum m_BlendSrc = GL_ONE;
        GLenum m_BlendDst = GL_ZERO;
        GLenum m_FrontFace = GL_CCW;
        GLenum m_PolygonMode = GL_FILL;

        unsigned int m_UsedShaderID = 0;

    public:

        // Private constructor/destructor
        VOpenGLStateChache() = default;
        ~VOpenGLStateChache() = default;

        // API methods
        void SetDepthTest(bool enable);
        void SetDepthFunc(GLenum depthFunc);
        void SetBlend(bool enable);
        void SetBlendFunc(GLenum src, GLenum dst);
        void SetCull(bool enable);
        void SetCullFace(GLenum face);
        void SetPolygonMode(GLenum mode);

        // TODO: Implement with Shader
        void SwitchUsedShader(unsigned int ID);
    };
}