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

    private:
        // OpenGL Objects
        unsigned int m_VAO = 0;
        unsigned int m_VBO = 0;
    };
}