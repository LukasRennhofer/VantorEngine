/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#include <Vantor/Vantor.hpp>

#include <iostream>

// Template using OpenGL RHI and GLFW

// Create RHI device
auto device = Vantor::RHI::VRDCoordinator::Instance().CreateDevice(Vantor::RHI::EGraphicsAPI::OPENGL);

// Vertex structure with position + color
struct Vertex {
    Vantor::Math::VVector3 position;
    Vantor::Math::VVector3 color;
};

void ResizeCallback(int w, int h) {
        device->SetViewport(0, 0, w, h);
}

int main() {

    // Create the window
    Vantor::Context::VWindow* window = new Vantor::Context::VWindow(1920, 1080, "Legendary Triangle");
    

    if (!device->Initialize()) {
        std::cerr << "Failed to initialize RHI device" << std::endl;
        return 1;
    }

    window->setResizeCallback(ResizeCallback);


    // Define a colorful triangle
    Vantor::Core::Container::TVector<Vertex> vertices = {
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}}, // Red
        {{ 0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}}, // Green
        {{ 0.0f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}  // Blue
    };
    Vantor::Core::Container::TVector<uint32_t> indices = {0, 1, 2};

    // Vertex layout: position + color
    Vantor::RHI::VVertexLayout layout;
    layout.stride = sizeof(Vertex);
    layout.attributes = {
        {0, Vantor::RHI::ERHIFormat::R32G32B32_FLOAT, offsetof(Vertex, position)},
        {1, Vantor::RHI::ERHIFormat::R32G32B32_FLOAT, offsetof(Vertex, color)}
    };

    auto mesh = device->CreateMesh(
        vertices.data(),
        vertices.size() * sizeof(Vertex),
        indices.data(),
        indices.size(),
        layout
    );

    // Vertex shader
    std::string vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;
        layout(location = 1) in vec3 aColor;
        out vec3 fragColor;
        void main() {
            gl_Position = vec4(aPos, 1.0);
            fragColor = aColor;
        }
    )";

    // Fragment shader
    std::string fragmentShaderSource = R"(
        #version 330 core
        in vec3 fragColor;
        out vec4 FragColor;
        void main() {
            FragColor = vec4(fragColor, 1.0);
        }
    )";

    auto shader = device->CreateShader(vertexShaderSource, fragmentShaderSource);

    device->SetViewport(0, 0, 1920, 1080);

    while (!window->shouldWindowClose()) {
        window->pollEvents();

        device->Clear(0.1f, 0.1f, 0.1f, 1.0f); // Dark background
        device->BindShader(shader);
        device->DrawMesh(mesh);

        window->swapBuffers();
    }

    return 0;
}
