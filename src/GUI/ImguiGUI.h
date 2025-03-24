/*
 *    				~ CHIFEngine ~
 *               
 * Copyright (c) 2025 Lukas Rennhofer
 *
 * Licensed under the MIT License. See LICENSE file for more details.
 *
 * Author: Lukas Rennhofer
 * Date: 2025-03-08
 *
 * File: GUI.h
 * Last Change: Added FPS-History Ability
 */
 

#pragma once

#include "../Platform/Window.h"
#include "../Graphics/drawableObject.h"

#include "../External/imgui/imgui.h"
#include "../External/imgui/imgui_impl_sdl2.h"
#include "../External/imgui/imgui_impl_opengl3.h"
#include "../Utils/utils.h"

#include <list>
#include <vector>
#include <format> 

namespace chif::GUI {
	class GUI : public chif::Graphics::drawableObject
	{
	public:
		GUI(chif::Platform::Window& w);
		~GUI();

		virtual void draw();
		virtual void update();

		GUI& subscribe(chif::Graphics::drawableObject* subscriber);

		// FPS Values
		static std::vector<float> fps_history;
		int index = 0;

	private:
		std::list<chif::Graphics::drawableObject*> subscribers;

	};
} // chif::GUI 