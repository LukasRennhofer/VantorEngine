/*
*    				~ Vantor (Sandbox) ~
*
* Copyright (c) 2025 Lukas Rennhofer
*
* Licensed under the GNU General Public License, Version 3. See LICENSE file for more details.
*
* Author: Lukas Rennhofer
* Date: 25.04.2025
*
* File: Application.hpp
* Last Change:
*/

#include "pch.hpp"

void framebufferSizeFunc(GLFWwindow* window, int width, int height);
void keyFunc(GLFWwindow* window, int key, int scancode, int action, int mods);
void mousePosFunc(GLFWwindow* window, double xpos, double ypos);
void mouseButtonFunc(GLFWwindow*, int button, int action, int mods);
void mouseScrollFunc(GLFWwindow*, double xoffset, double yoffset);

vantor::Graphics::RenderDevice::OpenGL::Renderer* renderer;
vantor::Graphics::FlyCamera camera(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
float deltaTime = 0.0f;
float lastFrameTime = 0.0f;
bool keysPressed[1024];
bool keysActive[1024];
bool wireframe = false;
bool renderGUI = false;

int main(int argc, char* argv[]) {
    vantor::Application app;
	app.Initialize();

	int success;
    vantor::Platform::Window window(success);

    if (!success)
		 return -1;

    // TODO: MOVE INTO ENGINE
    glfwSetFramebufferSizeCallback(window.getWindow(), framebufferSizeFunc);
	glfwSetKeyCallback(window.getWindow(), keyFunc);
	glfwSetCursorPosCallback(window.getWindow(), mousePosFunc);
	glfwSetMouseButtonCallback(window.getWindow(), mouseButtonFunc);
	glfwSetScrollCallback(window.getWindow(), mouseScrollFunc);

    renderer = vantor::Graphics::RenderDevice::OpenGL::Init();
    
    renderer->SetRenderSize(vantor::Platform::Window::SCR_WIDTH, vantor::Platform::Window::SCR_WIDTH);
	renderer->SetCamera(&camera);

	// basic shapes
	vantor::Graphics::Geometry::Primitives::Plane plane(16, 16);
	vantor::Graphics::Geometry::Primitives::Sphere sphere(64, 64);
	vantor::Graphics::Geometry::Primitives::Sphere tSphere(256, 256);
	vantor::Graphics::Geometry::Primitives::Torus torus(2.0f, 0.4f, 32, 32);
	vantor::Graphics::Geometry::Primitives::Cube cube;

	// material setup
	vantor::Graphics::RenderDevice::OpenGL::Material* matPbr = renderer->CreateMaterial();
	vantor::Graphics::RenderDevice::OpenGL::Shader* plasmaOrbShader = vantor::Resources::LoadShader("plasma orb", "res/intern/shaders/custom/plasma_orb.vs", "res/intern/shaders/custom/plasma_orb.fs");
	vantor::Graphics::RenderDevice::OpenGL::Material* matPlasmaOrb = renderer->CreateCustomMaterial(plasmaOrbShader);
	matPlasmaOrb->Cull = false;
	matPlasmaOrb->Blend = true;
	matPlasmaOrb->BlendSrc = GL_ONE;
	matPlasmaOrb->BlendDst = GL_ONE;
	matPlasmaOrb->SetTexture("TexPerllin", vantor::Resources::LoadTexture("res/intern/perlin noise", "res/intern/textures/perlin.png"), 0);
	matPlasmaOrb->SetFloat("Strength", 1.5f);
	matPlasmaOrb->SetFloat("Speed", 0.083f);
	// configure camera
	camera.SetPerspective(glm::radians(60.0f), renderer->GetRenderSize().x / renderer->GetRenderSize().y, 0.1f, 100.0f);

	// scene setup
	vantor::SceneNode* mainTorus = vantor::Scene::MakeSceneNode(&torus, matPbr);
	vantor::SceneNode* secondTorus = vantor::Scene::MakeSceneNode(&torus, matPbr);
	vantor::SceneNode* thirdTorus = vantor::Scene::MakeSceneNode(&torus, matPbr);
	vantor::SceneNode* plasmaOrb = vantor::Scene::MakeSceneNode(&tSphere, matPlasmaOrb);

	mainTorus->AddChild(secondTorus);
	secondTorus->AddChild(thirdTorus);

	mainTorus->SetScale(1.0f);
	mainTorus->SetPosition(glm::vec3(-4.4f, 3.46f, -0.3));
	secondTorus->SetScale(0.65f);
	secondTorus->SetRotation(glm::vec4(0.0, 1.0, 0.0, glm::radians(90.0)));
	thirdTorus->SetScale(0.65f);

	plasmaOrb->SetPosition(glm::vec3(-4.0f, 4.0f, 0.25f));
	plasmaOrb->SetScale(0.6f);

	// - background
	vantor::Graphics::Background* background = new vantor::Graphics::Background;
	vantor::Graphics::RenderDevice::OpenGL::PBRCapture* pbrEnv = renderer->GetSkypCature();
	background->SetCubemap(pbrEnv->Prefiltered);
	float lodLevel = 1.5f;
	background->Material->SetFloat("lodLevel", lodLevel);

	// post processing
	vantor::Graphics::RenderDevice::OpenGL::Shader* postProcessing1 = vantor::Resources::LoadShader("postprocessing1", "res/intern/shaders/screen_quad.vs", "res/intern/shaders/custom_post_1.fs");
	vantor::Graphics::RenderDevice::OpenGL::Shader* postProcessing2 = vantor::Resources::LoadShader("res/intern/postprocessing2", "shaders/screen_quad.vs", "res/intern/shaders/custom_post_2.fs");
	vantor::Graphics::RenderDevice::OpenGL::Material* customPostProcessing1 = renderer->CreatePostProcessingMaterial(postProcessing1);
	vantor::Graphics::RenderDevice::OpenGL::Material* customPostProcessing2 = renderer->CreatePostProcessingMaterial(postProcessing2);

	// mesh 
	vantor::SceneNode* sponza = vantor::Resources::LoadMesh(renderer, "sponza", "res/intern/meshes/sponza/sponza.obj");
	sponza->SetPosition(glm::vec3(0.0, -1.0, 0.0));
	sponza->SetScale(0.01f);

	// lighting
	vantor::Graphics::DirectionalLight dirLight;
	dirLight.Direction = glm::vec3(0.2f, -1.0f, 0.25f);
	dirLight.Color = glm::vec3(1.0f, 0.89f, 0.7f);
	dirLight.Intensity = 50.0f;
	renderer->AddLight(&dirLight);

    std::vector<vantor::Graphics::PointLight> torchLights;
	{
		vantor::Graphics::PointLight torch;
		torch.Radius = 2.5;
		torch.Color = glm::vec3(1.0f, 0.3f, 0.05f);
		torch.Intensity = 50.0f;
		torch.RenderMesh = true;

		torch.Position = glm::vec3(4.85f, 0.7f, 1.43f);
		torchLights.push_back(torch);
		torch.Position = glm::vec3(4.85f, 0.7f, -2.2f);
		torchLights.push_back(torch);
		torch.Position = glm::vec3(-6.19f, 0.7f, 1.43f);
		torchLights.push_back(torch);
		torch.Position = glm::vec3(-6.19f, 0.7f, -2.2f);
		torchLights.push_back(torch);
		renderer->AddLight(&torchLights[0]);
		renderer->AddLight(&torchLights[1]);
		renderer->AddLight(&torchLights[2]);
		renderer->AddLight(&torchLights[3]);
	}

    // bake irradiance GI (with grid placement of probes)
	{
		// bottom floor - center
		renderer->AddIrradianceProbe(glm::vec3(0.0f, 0.5f, -0.5f), 3.25);
		renderer->AddIrradianceProbe(glm::vec3(3.0f, 0.5f, -0.5f), 3.25);
		renderer->AddIrradianceProbe(glm::vec3(6.0f, 0.5f, -0.5f), 3.25);
		renderer->AddIrradianceProbe(glm::vec3(8.5f, 0.5f, -0.5f), 3.25);
		renderer->AddIrradianceProbe(glm::vec3(11.4f, 0.5f, -0.5f), 4.25);
		renderer->AddIrradianceProbe(glm::vec3(-3.0f, 0.5f, -0.5f), 3.25);
		renderer->AddIrradianceProbe(glm::vec3(-6.2f, 0.5f, -0.5f), 3.25);
		renderer->AddIrradianceProbe(glm::vec3(-9.5f, 0.5f, -0.5f), 3.25);
		renderer->AddIrradianceProbe(glm::vec3(-12.1f, 0.5f, -0.5f), 4.25);
		// bottom floor - left wing
		renderer->AddIrradianceProbe(glm::vec3(0.0f, 0.5f, 4.0f), 4.0);
		renderer->AddIrradianceProbe(glm::vec3(4.0f, 0.5f, 4.0f), 4.0);
		renderer->AddIrradianceProbe(glm::vec3(8.0f, 0.5f, 4.0f), 4.0);
		renderer->AddIrradianceProbe(glm::vec3(12.0f, 0.5f, 4.0f), 4.0);
		renderer->AddIrradianceProbe(glm::vec3(-4.0f, 0.5f, 4.0f), 4.0);
		renderer->AddIrradianceProbe(glm::vec3(-8.0f, 0.5f, 4.0f), 4.0);
		renderer->AddIrradianceProbe(glm::vec3(-12.0f, 0.5f, 4.0f), 4.0);
		// bottom floor - right wing
		renderer->AddIrradianceProbe(glm::vec3(0.0f, 0.5f, -4.5f), 4.0);
		renderer->AddIrradianceProbe(glm::vec3(4.0f, 0.5f, -4.5f), 4.0);
		renderer->AddIrradianceProbe(glm::vec3(8.0f, 0.5f, -4.5f), 4.0);
		renderer->AddIrradianceProbe(glm::vec3(12.0f, 0.5f, -4.5f), 4.0);
		renderer->AddIrradianceProbe(glm::vec3(-4.0f, 0.5f, -4.5f), 4.0);
		renderer->AddIrradianceProbe(glm::vec3(-8.0f, 0.5f, -4.5f), 4.0);
		renderer->AddIrradianceProbe(glm::vec3(-12.0f, 0.5f, -4.5f), 4.0);
		// 1st floor - center wing
		renderer->AddIrradianceProbe(glm::vec3(0.0f, 5.0f, -0.5f), 4.5);
		renderer->AddIrradianceProbe(glm::vec3(4.0f, 5.0f, -0.5f), 4.0);
		renderer->AddIrradianceProbe(glm::vec3(8.0f, 5.0f, -0.5f), 4.5);
		renderer->AddIrradianceProbe(glm::vec3(12.0f, 5.0f, -0.5f), 4.5);
		renderer->AddIrradianceProbe(glm::vec3(-4.0f, 5.0f, -0.5f), 4.5);
		renderer->AddIrradianceProbe(glm::vec3(-8.0f, 5.0f, -0.5f), 4.0);
		renderer->AddIrradianceProbe(glm::vec3(-12.0f, 5.0f, -0.5f), 4.5);
		// 1st floor - left wing
		renderer->AddIrradianceProbe(glm::vec3(0.0f, 5.0f, 4.0), 4.0);
		renderer->AddIrradianceProbe(glm::vec3(4.0f, 5.0f, 4.0), 4.0);
		renderer->AddIrradianceProbe(glm::vec3(8.0f, 5.0f, 4.0), 4.0);
		renderer->AddIrradianceProbe(glm::vec3(12.0f, 5.0f, 4.0), 4.0);
		renderer->AddIrradianceProbe(glm::vec3(-4.0f, 5.0f, 4.0), 4.0);
		renderer->AddIrradianceProbe(glm::vec3(-8.0f, 5.0f, 4.0), 4.0);
		renderer->AddIrradianceProbe(glm::vec3(-11.5f, 5.0f, 4.0), 4.0);
		// 1st floor - right wing
		renderer->AddIrradianceProbe(glm::vec3(0.0f, 5.0f, -4.5f), 4.0);
		renderer->AddIrradianceProbe(glm::vec3(4.0f, 5.0f, -4.5f), 4.0);
		renderer->AddIrradianceProbe(glm::vec3(8.0f, 5.0f, -4.5f), 4.0);
		renderer->AddIrradianceProbe(glm::vec3(12.0f, 5.0f, -4.5f), 4.0);
		renderer->AddIrradianceProbe(glm::vec3(-4.0f, 5.0f, -4.5f), 4.0);
		renderer->AddIrradianceProbe(glm::vec3(-8.0f, 5.0f, -4.5f), 4.0);
		renderer->AddIrradianceProbe(glm::vec3(-11.5f, 5.0f, -4.5f), 4.0);
		// 2nd floor - center wing
		renderer->AddIrradianceProbe(glm::vec3(0.0f, 9.5f, -0.5f), 4.5);
		renderer->AddIrradianceProbe(glm::vec3(4.0f, 9.5f, -0.5f), 4.5);
		renderer->AddIrradianceProbe(glm::vec3(8.0f, 9.5f, -0.5f), 4.5);
		renderer->AddIrradianceProbe(glm::vec3(12.0f, 9.5f, -0.5f), 4.5);
		renderer->AddIrradianceProbe(glm::vec3(-4.0f, 9.5f, -0.5f), 4.5);
		renderer->AddIrradianceProbe(glm::vec3(-8.0f, 9.5f, -0.5f), 4.5);
		renderer->AddIrradianceProbe(glm::vec3(-11.5f, 9.5f, -0.5f), 4.5);

		// bake before rendering
		renderer->BakeProbes();
	}

    while (!glfwWindowShouldClose(window.window))
	{
		vantor::Graphics::RenderDevice::OpenGL::BeginFrame();

		float currentFrameTime = glfwGetTime();
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		{
			//CLOCK(UPDATE);
			if (keysPressed[GLFW_KEY_W] || keysPressed[GLFW_KEY_UP])
				camera.InputKey(deltaTime, vantor::Graphics::CAMERA_FORWARD);
			if (keysPressed[GLFW_KEY_S] || keysPressed[GLFW_KEY_DOWN])
				camera.InputKey(deltaTime, vantor::Graphics::CAMERA_BACK);
			if (keysPressed[GLFW_KEY_A] || keysPressed[GLFW_KEY_LEFT])
				camera.InputKey(deltaTime, vantor::Graphics::CAMERA_LEFT);
			if (keysPressed[GLFW_KEY_D] || keysPressed[GLFW_KEY_RIGHT])
				camera.InputKey(deltaTime, vantor::Graphics::CAMERA_RIGHT);
			if (keysPressed[GLFW_KEY_E])
				camera.InputKey(deltaTime, vantor::Graphics::CAMERA_UP);
			if (keysPressed[GLFW_KEY_Q])
				camera.InputKey(deltaTime, vantor::Graphics::CAMERA_DOWN);
			if (keysPressed[GLFW_KEY_TAB] && !keysActive[GLFW_KEY_TAB])
			{
				renderGUI = !renderGUI;
				keysActive[GLFW_KEY_TAB] = true;
			}

			camera.Update(deltaTime);

			mainTorus->SetRotation(glm::vec4(glm::vec3(1.0f, 0.0f, 0.0f), glfwGetTime() * 2.0));
			secondTorus->SetRotation(glm::vec4(glm::vec3(0.0f, 1.0f, 0.0f), glfwGetTime() * 3.0));
			thirdTorus->SetRotation(glm::vec4(glm::vec3(0.0f, 1.0f, 0.0f), glfwGetTime() * 4.0));

			for (int i = 0; i < torchLights.size(); ++i)
			{
				torchLights[i].Radius = 1.5f + 0.1 * std::cos(std::sin(glfwGetTime() * 1.37 + i * 7.31) * 3.1 + i);
				torchLights[i].Intensity = 25.0f + 5.0 * std::cos(std::sin(glfwGetTime() * 0.67 + i * 2.31) * 2.31 * i);
			}

			matPlasmaOrb->SetFloat("Time", glfwGetTime());
		}

		{
			//CLOCK(PUSH);
			renderer->PushRender(mainTorus);
			renderer->PushRender(sponza);
			renderer->PushRender(plasmaOrb);
			renderer->PushRender(background);
		}
		{
			// push post-processing calls
			//renderer->PushPostProcessor(customPostProcessing1);
			//renderer->PushPostProcessor(customPostProcessing2);
		}

		{
			//CLOCK(RENDER);
			// request Cell to render all currently pushed commands
			renderer->RenderPushedCommands();
		}

		// // GUI
		// if (renderGUI)
		// {
		// 	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// 	Cell::RenderGUI();
		// }
		// else
		// {
		// 	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		// }
        glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		vantor::Graphics::RenderDevice::OpenGL::EndFrame();
        window.swapBuffersAndPollEvents();
	}

	// clean up Cell
	vantor::Graphics::RenderDevice::OpenGL::Shutdown();

	return 0;
}

void framebufferSizeFunc(GLFWwindow* window, int width, int height)
{
	renderer->SetRenderSize(width, height);
	camera.SetPerspective(glm::radians(60.0f), (float)width / (float)height, 0.1f, 100.0f);
}

void keyFunc(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (GLFW_KEY_ESCAPE == key) {
		glfwSetWindowShouldClose(window, true);
	}
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keysPressed[key] = true;
		else if (action == GLFW_RELEASE)
		{
			keysPressed[key] = false;
			keysActive[key] = false;
		}
	}
}

bool firstMouse = true;
float lastX = 640.0f;
float lastY = 360.0f;

void mousePosFunc(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;  // reversed since y-coordinates go from bottom to left

	lastX = xpos;
	lastY = ypos;
	if (!renderGUI)
	{
		camera.InputMouse(xoffset, yoffset);
	}
}

void mouseButtonFunc(GLFWwindow*, int button, int action, int mods)
{
	// TODO: IMGUI
}

void mouseScrollFunc(GLFWwindow*, double xoffset, double yoffset)
{
	camera.InputScroll(xoffset, yoffset);
}