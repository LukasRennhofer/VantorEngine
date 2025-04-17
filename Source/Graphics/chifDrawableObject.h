
/*
 *    				~ chif::GraphicsEngine ~
 *               
 * Copyright (c) 2025 Lukas Rennhofer
 *
 * Licensed under the MIT License. See LICENSE file for more details.
 *
 * Author: Lukas Rennhofer
 * Date: 2025-03-08
 *
 * File: chifDrawableObject.h
 * Last Change: 
 */
 

#ifndef DRAWABLEOBJECT_H
#define DRAWABLEOBJECT_H

#include "chifSceneElements.h"

namespace chif::Graphics {
	class drawableObject
	{
	public:
		virtual void draw() = 0;
		virtual void setGui() {};

		virtual void update() {};

		static chif::Graphics::sceneElements* scene;
	};
} // NAMESPACE chif::Graphics

#endif