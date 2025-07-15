#include "../../Interface/Common/VRD_ScreenQuad.hpp"

// Resource Manager
#include "../../../Resource/VRES_Manager.hpp"

#include <Shared/glad/glad.h>

namespace Vantor::RenderDevice {
    // NOTE (LUKAS): The Texture Binding Unit is always 1!
    class VOpenGLScreenQuad : public VScreenQuad {
    public:
        VOpenGLScreenQuad();
        ~VOpenGLScreenQuad() override;

        void Draw() override;
        void DrawRaw() override;

    private:
        // OpenGL Objects
        GLuint m_VAO = 0;
        GLuint m_VBO = 0;

        float quadVertices[]; // Todo: make to resizeable vector
    };
}