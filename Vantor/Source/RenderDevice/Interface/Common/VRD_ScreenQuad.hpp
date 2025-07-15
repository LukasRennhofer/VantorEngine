#pragma once

#include "../VRD_Shader.hpp"

#include <memory>

namespace Vantor::RenderDevice {
    class VScreenQuad {
        public:
            VScreenQuad() = default;
            virtual ~VScreenQuad();

            virtual void Draw() = 0;
            virtual void DrawRaw() = 0;

        protected:
            std::shared_ptr<VShader> m_ShaderScreenQuad;
    };

}