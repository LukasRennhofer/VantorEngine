/*
 *    				~ Vantor ~
 *               
 * Copyright (c) 2025 Lukas Rennhofer
 *
 * Licensed under the GNU General Public License, Version 3. See LICENSE file for more details.
 *
 * Author: Lukas Rennhofer
 * Date: 2025-03-08
 *
 * File: vantorImgui.h
 * Last Change: Added FPS-History Ability
 */
 

#pragma once

#include "../Platform/vantorWindow.h"
#include "../Graphics/vantorDrawableObject.h"

#include "../External/imgui/imgui.h"
#include "../External/imgui/imgui_impl_sdl2.h"
#include "../External/imgui/imgui_impl_opengl3.h"
#include "../Utils/utils.h"

#include <list>
#include <vector>
#include <format> 

namespace vantor::GUI {
	class GUI : public vantor::Graphics::drawableObject
	{
	public:
		GUI(vantor::Platform::Window& w);
		~GUI();

		virtual void draw();
		virtual void update();

		GUI& subscribe(vantor::Graphics::drawableObject* subscriber);

		// FPS Values
		static std::vector<float> fps_history;
		int index = 0;

	private:
		std::list<vantor::Graphics::drawableObject*> subscribers;

	};
} // vantor::GUI 