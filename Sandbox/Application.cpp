#include "common.h"
#include "Objects/Geometry.hpp"

#include "LoadMesh.hpp"

int main() {
    // Create a OpenGL RenderDevice (VANTOR_API_OPENGL must be compiled)
    auto RenderDevice = vCreateRenderDevice(VRenderAPI::OPENGL);

    VApplication app;
    // Application Creation Data
    VApplicationCreateInfo appInfo;
    appInfo.windowWidth = 1280;
    appInfo.windowHeight = 720;
    appInfo.windowTitle = "Vantor Sandbox";

    app.Initialize(appInfo);

    VVector2 mouseDelta;
    auto camera = vCreateActor<VFlyCamera>(VVector3(1.0f, 1.0f, 1.0f));
    camera->MouseSensitivty  = 0.05f;

    VTextureSampler sampler;
    sampler.minFilter       = VTextureFilter::Linear;
    sampler.magFilter       = VTextureFilter::Linear;
    sampler.wrapS           = VTextureWrap::Repeat;
    sampler.wrapT           = VTextureWrap::Repeat;
    sampler.generateMipmaps = false;

    auto cube = CreateCubeObject(RenderDevice.get(), {2.0f, 2.0f, 2.0f});
    cube->GetComponent<VMaterialComponent>()->GetMaterial()->Color = VColor::Yellow();
    auto plane = CreatePlaneObject(RenderDevice.get(), 10.0f, 10.0f, 0.0f);
    
    // Point Lights 
    VPointLightData pointLight1;
    pointLight1.position = { 2.0f, 2.0f, 2.0f }; // Slightly above and in front of the object
    pointLight1.ambient  = { 0.05f, 0.05f, 0.05f };
    pointLight1.diffuse  = {  1.0f, 1.0f, 1.0f };  // Full white diffuse for brightness
    pointLight1.specular = { 0.1f, 0.1f, 0.1f };
    pointLight1.constant = 1.0f;
    pointLight1.linear   = 0.09f;
    pointLight1.quadratic = 0.032f;
    pointLight1.radius = 5.0f;

    VAmbientLightData ambientLight {
        .ambientColor = VVector3(0.02f, 0.02f, 0.05f)
    };

    VDirectionalLightData sunLight {
        .direction = VVector3(-0.3f, -1.0f, -0.5f),
        .ambient   = VColor::Red().toFloat3(),
        .diffuse   = VVector3(0.6f, 0.6f, 0.9f),
        .specular  = VVector3(0.3f, 0.3f, 0.4f)
    };

    VSpotLightData spotLight {
        .position     = camera->Position,
        .constant     = 1.0f,
        .direction    = camera->Forward,
        .linear       = 0.09f,
        .ambient      = VVector3(0.02f, 0.02f, 0.02f),
        .quadratic    = 0.032f,
        .diffuse      = VVector3(1.0f, 1.0f, 0.8f),
        .cutOff       = std::cos(Vantor::Math::DegToRad(12.5f)),
        .specular     = VVector3(1.0f, 1.0f, 1.0f),
        .outerCutOff  = std::cos(Vantor::Math::DegToRad(17.5f)),
        .radius       = 15.0f
    };

    glfwSwapInterval(0); // VSync OFF : TODO: Implement in Context

    // vLogError("Game", "This is a test error! (located in Application.cpp)");

    // RenderPath Test
    auto renderpath = RenderDevice->CreateRenderPath3D();

    vStudioInitialize();

    camera->SetPerspective(45.0f, (float)Vantor::VServiceLocator::GetContextWidth()/(float)Vantor::VServiceLocator::GetContextHeight(), 0.1f, 100.0f);
    
    renderpath->SetCamera(camera.get());

    auto deviceone = Vantor::Input::CreateInputDevice(app.GetWindow()); // TODO: Write vCreateInputDevice(VWindow* window); function
    app.GetInputManager()->AddDevice(deviceone);

    app.GetInputManager()->MapAction("fire", VInputButton{VInputDeviceType::Keyboard, (int)VInputKey::KEY_ESCAPE});

    while (app.IsRunning()) {
            app.BeginFrame();

            if (app.GetInputManager()->WasPressed(VInputKey::KEY_ESCAPE, VInputDeviceType::Keyboard)) {
                app.Break();
            }

            if (!app.GetInputManager()->devices[0]->GetMouseCursorState()) {
                 if (app.GetInputManager()->IsPressed(VInputKey::KEY_W, VInputDeviceType::Keyboard)) {
                    camera->InputKey(app.GetDeltaTime(), VCameraMovement::CAMERA_FORWARD);
                } else if (app.GetInputManager()->IsPressed(VInputKey::KEY_A, VInputDeviceType::Keyboard)) {
                    camera->InputKey(app.GetDeltaTime(), VCameraMovement::CAMERA_LEFT);
                } else if (app.GetInputManager()->IsPressed(VInputKey::KEY_D, VInputDeviceType::Keyboard)) {
                    camera->InputKey(app.GetDeltaTime(), VCameraMovement::CAMERA_RIGHT);
                } else if (app.GetInputManager()->IsPressed(VInputKey::KEY_S, VInputDeviceType::Keyboard)) {
                    camera->InputKey(app.GetDeltaTime(), VCameraMovement::CAMERA_BACK);
                } else if (app.GetInputManager()->IsPressed(VInputKey::KEY_E, VInputDeviceType::Keyboard)) {
                    camera->InputKey(app.GetDeltaTime(), VCameraMovement::CAMERA_UP);
                } else if (app.GetInputManager()->IsPressed(VInputKey::KEY_Q, VInputDeviceType::Keyboard)) {
                    camera->InputKey(app.GetDeltaTime(), VCameraMovement::CAMERA_DOWN);
                } 

                mouseDelta = app.GetInputManager()->devices[0]->GetMouseDelta();
                camera->InputMouse(mouseDelta.x, -mouseDelta.y);
            }
            
            if (app.GetInputManager()->WasPressed(VInputKey::KEY_TAB, VInputDeviceType::Keyboard)) {
                app.GetInputManager()->devices[0]->SetMouseCursorState(!app.GetInputManager()->devices[0]->GetMouseCursorState());
            }

            camera->Update(app.GetDeltaTime());

            renderpath->PushRender(cube.get());
            renderpath->PushRender(plane.get());

            spotLight.position  = camera->Position;
            spotLight.direction = camera->Forward;
            renderpath->PushSpotLight(spotLight);

            // renderpath->PushPointLight(pointLight1);
            renderpath->SetAmbientLight(ambientLight);
            renderpath->PushSpotLight(spotLight);

            renderpath->Render();

            vStudioRender(renderpath->GetOutBuffer().get());

            app.EndFrame();
    }

    renderpath->Shutdown();

    vStudioShutdown();

	app.Shutdown();

    return 0;
}