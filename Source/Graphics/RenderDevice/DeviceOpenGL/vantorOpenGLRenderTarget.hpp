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
 * File: vantorRenderTarget.hpp
 * Last Change:
*/

#pragma once

#include "vantorOpenGLTexture.hpp"

#include <glad/glad.h>

#include <vector>

namespace vantor::Graphics::RenderDevice::OpenGL
{
    class Renderer;
 class RenderTarget
 {
  friend Renderer;
 public:
  unsigned int ID;

  unsigned int Width;
  unsigned int Height;
  GLenum       Type;

  bool HasDepthAndStencil;
 private:
  GLenum       m_Target = GL_TEXTURE_2D;
  Texture              m_DepthStencil;
  std::vector<Texture> m_ColorAttachments;
 public:
  RenderTarget(unsigned int width, unsigned int height, GLenum type = GL_UNSIGNED_BYTE, unsigned int nrColorAttachments = 1, bool depthAndStencil = true);

  Texture *GetDepthStencilTexture();
  Texture *GetColorTexture(unsigned int index);

  void Resize(unsigned int width, unsigned int height);
  void SetTarget(GLenum target);

  //void Bind(bool clear = true, bool setViewport = true);
 };
} // vantor::Graphics::RenderDevice::OpenGL

