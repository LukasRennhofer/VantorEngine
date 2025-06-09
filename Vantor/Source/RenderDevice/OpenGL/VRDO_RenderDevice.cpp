#include "VRDO_RenderDevice.hpp"

namespace Vantor::RenderDevice {
    void VRDeviceOpenGL::BeginFrame() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void VRDeviceOpenGL::EndFrame() {
        // TODO: flush, prepare for present
    }

    void VRDeviceOpenGL::Present() {
        // TODO: Typically handled in the main loop with glfwSwapBuffers(window)
    }

    // TODO: make arguemnts to shader code not path
    std::shared_ptr<VShader> VRDeviceOpenGL::CreateShader(const char* vertexCode, const char* fragmentCode) {
        return std::make_shared<VOpenGLShader>(vertexCode, fragmentCode);
    }

    VERenderAPI VRDeviceOpenGL::GetRenderDeviceAPI() const {
        return VERenderAPI::OPENGL;
    }

    std::string VRDeviceOpenGL::GetRenderDeviceName() const {
        return "VRDeviceOpenGL";
    } 

    // Device Info
    std::string VRDeviceOpenGL::GetPhysicalDeviceVendor() const {
        const GLubyte* vendor = glGetString(GL_VENDOR);
        return std::string(reinterpret_cast<const char*>(vendor));
    }
    std::string VRDeviceOpenGL::GetPhysicalDeviceName() const {
        const GLubyte* renderer = glGetString(GL_RENDERER);
        return std::string(reinterpret_cast<const char*>(renderer));
    }
}