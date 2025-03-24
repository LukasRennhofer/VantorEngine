/*
 *    				~ CHIFEngine ~
 *
 * Copyright (c) 2025 Lukas Rennhofer
 *
 * Licensed under the MIT License. See LICENSE file for more details.
 *
 * Author: Lukas Rennhofer
 * Date: 2025-03-11
 *
 * File: GUI.cpp
 * Last Change: Added Dear Imgui Light theme by Pacôme Danhiez and added Demo Profiler
 */

#include "ImguiGUI.h"

namespace chif::GUI {
	// FPS History
	std::vector<float> GUI::fps_history(100, 0.0f);

	GUI::GUI(chif::Platform::Window& window)
	{
		// GUI
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGui_ImplSDL2_InitForOpenGL(window.getWindow(), chif::Platform::Window::glContext);
		ImGui_ImplOpenGL3_Init("#version 130");

		// Use Custom Fonts - TODO: Working for Linux!!
		ImGuiIO &io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF("resources/UI/Roboto-Regular.ttf", 18.0f); // Adding Main Font
		io.Fonts->Build();

		// Set Custom Styles
		ImGuiStyle &style = ImGui::GetStyle();

		// light style from Pacôme Danhiez (user itamago) https://github.com/ocornut/imgui/pull/511#issuecomment-175719267
		style.Alpha = 1.0f;
		style.FrameRounding = 3.0f;
		style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 0.94f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.39f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
	}

	void GUI::draw()
	{
			// TODO ? Or for CHIFEngine Studio
			// if (ImGui::Begin("Example Window", nullptr, ImGuiWindowFlags_MenuBar)) {

			// 	// Create a menu bar
			// 	if (ImGui::BeginMenuBar())
			// 	{
			// 		if (ImGui::BeginMenu("Menu"))
			// 		{
			// 			ImGui::MenuItem("Console");
			// 		}
			
			// 		ImGui::EndMenuBar();
			// 	}
			// 	ImGui::End();
			// }
		
			chif::Graphics::sceneElements& scene = *this->scene;

			for (chif::Graphics::drawableObject* obj : subscribers) {
				obj->setGui();
			}

			ImGui::Begin("Scene controls: ");
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "Other controls");
			if (ImGui::DragFloat3("Light Position", &scene.lightDir[0], 0.01, -1.0, 1.0)) {
				auto saturate = [](float v) { return std::min(std::max(v, 0.0f), 0.8f); };
				scene.lightDir.y = saturate(scene.lightDir.y);
				//skybox.update();
			}

			ImGui::InputFloat3("Camera Position", &(scene.cam->Position[0]));
			ImGui::ColorEdit3("Light color", (float*)&scene.lightColor);
			ImGui::ColorEdit3("Fog color", (float*)&scene.fogColor);
			ImGui::SliderFloat("Camera speed", &scene.cam->MovementSpeed, 0.0, SPEED*3.0);


			ImGui::Checkbox("Wireframe mode", &scene.wireframe);

			if (ImGui::Button("Generate seed"))
				scene.seed = genRandomVec3();
			//ImGui::SameLine();
			//ImGui::Text("Generate a new seed");
			ImGui::SameLine();
			if (ImGui::Button("Use default seed")) 
				scene.seed = glm::vec3(0.0, 0.0, 0.0);

			/*ImGui::SameLine();
			if (ImGui::Button("Default Preset")) {
				volumetricClouds.DefaultPreset();
				lightDir.y = 0.5;
			}*/
			//ImGui::SameLine();
			/*if (ImGui::Button("Sunset Preset 1")) {
				skybox.SunsetPreset();
			}
			ImGui::SameLine();
			if (ImGui::Button("Sunset Preset 2")) {
				skybox.SunsetPreset1();
			}*/

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

			ImGui::End();

			fps_history[index] = ImGui::GetIO().Framerate;
			index = (index + 1) % fps_history.size(); // Circular buffer

			ImGui::Begin("Profiler");
			ImGui::PlotLines("FPS", fps_history.data(), fps_history.size(), 0, NULL, 0.0f, 144.0f, ImVec2(0, 80));
			ImGui::End();
			
			// CONTROLLER INPUTS : TODO, JUST PROTOTYPE

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void GUI::update()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			// Pass events to ImGui
			ImGui_ImplSDL2_ProcessEvent(&event);

			// Handle SDL_QUIT event to properly close the application
			if (event.type == SDL_QUIT) {
				// Set a flag to quit the application (if applicable)
				// Example: quit = true;
			}

			// Handle window resizing
			if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
				int width = event.window.data1;
				int height = event.window.data2;
				glViewport(0, 0, width, height);
			}
		}

		// Start a new ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
	}

	GUI & GUI::subscribe(chif::Graphics::drawableObject * subscriber)
	{
		subscribers.push_back(subscriber);

		return *this;
	}


	GUI::~GUI()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}
} // NAMESPACE chif::GUI