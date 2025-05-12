/*
 *  ╔═══════════════════════════════════════════════════════════════╗
 *  ║                          ~ Vantor ~                           ║
 *  ║                                                               ║
 *  ║  This file is part of the Vantor Engine.                      ║
 *  ║  Automatically formatted by vantorFormat.py                   ║
 *  ║                                                               ║
 *  ╚═══════════════════════════════════════════════════════════════╝
 *
 *  Copyright (c) 2025 Lukas Rennhofer
 *  Licensed under the GNU General Public License, Version 3.
 *  See LICENSE file for more details.
 *
 *  Author: Lukas Rennhofer
 *  Date: 2025-05-12
 *
 *  File: vantorOpenGLPostProcessor.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "../../Renderer/Camera/vantorCamera.hpp"

namespace vantor::Graphics::RenderDevice::OpenGL
{
    class Texture;
    class RenderTarget;
    class Shader;
    class Renderer;

    class PostProcessor
    {
        public:
            // resulting post-processor intermediate outputs
            Texture *DownSampledHalfOutput;
            Texture *DownSampledQuarterOutput;
            Texture *DownSampledEightOutput;
            Texture *DownSampledSixteenthOutput;
            Texture *BlurredEightOutput;
            Texture *BlurredSixteenthOutput;
            Texture *SSAOOutput;
            Texture *BloomOutput1;
            Texture *BloomOutput2;
            Texture *BloomOutput3;
            Texture *BloomOutput4;
            Texture *SSROutput;

            // toggles
            bool Sepia      = false;
            bool Vignette   = true;
            bool Bloom      = true;
            bool SSAO       = true;
            bool TXAA       = false;
            bool SSR        = false;
            bool MotionBlur = true;

            // ssao
            int SSAOKernelSize = 32;

            // motion-blur
            float FPSTarget = 60.0;

        private:
            // global post-process state
            Shader       *m_PostProcessShader;
            RenderTarget *m_RTOutput;

            // ssao
            RenderTarget *m_SSAORenderTarget;
            Shader       *m_SSAOShader;
            Shader       *m_SSAOBlurShader;
            Texture      *m_SSAONoise;
            // bloom
            RenderTarget *m_BloomRenderTarget0;
            RenderTarget *m_BloomRenderTarget1;
            RenderTarget *m_BloomRenderTarget2;
            RenderTarget *m_BloomRenderTarget3;
            RenderTarget *m_BloomRenderTarget4;
            Shader       *m_BloomShader;
            Shader       *m_BloomBlurShader;
            // downsample
            RenderTarget *m_DownSampleRTHalf;
            RenderTarget *m_DownSampleRTQuarter;
            RenderTarget *m_DownSampleRTEight;
            RenderTarget *m_DownSampleRTSixteenth;
            Shader       *m_DownSampleShader;
            // (lower resolution) downsampled blurs
            RenderTarget *m_DownSampleBlurRTEight;
            RenderTarget *m_DownSampleBlurRTSixteenth;
            // blur
            RenderTarget *m_GaussianRTHalf_H;
            RenderTarget *m_GaussianRTQuarter_H;
            RenderTarget *m_GaussianRTEight_H;
            RenderTarget *m_GaussianRTSixteenth_H;
            Shader       *m_OnePassGaussianShader;
            // ssr
            RenderTarget *m_SSRRT;
            Shader       *m_SSRShader;

        public:
            PostProcessor(Renderer *renderer);
            ~PostProcessor();

            void UpdateRenderSize(unsigned int width, unsigned int height);

            // process stages
            void ProcessPreLighting(Renderer *renderer, RenderTarget *gBuffer, vantor::Graphics::Camera *camera);
            void ProcessPostLighting(Renderer *renderer, RenderTarget *gBuffer, RenderTarget *output, vantor::Graphics::Camera *camera);

            // blit all combined post-processing steps to default framebuffer
            void Blit(Renderer *renderer, Texture *soruce);

        private:
            Texture *downsample(Renderer *renderer, Texture *src, RenderTarget *dst);
            Texture *blur(Renderer *renderer, Texture *src, RenderTarget *dst, int count);
    };
} // namespace vantor::Graphics::RenderDevice::OpenGL