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
 *  File: vantorOpenGLPBR.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "../vantorOpenGLTexture.hpp"
#include "../../../Renderer/Camera/vantorCamera.hpp"
#include "../../../../Core/Scene/vantorSceneNode.hpp"
#include "../vantorOpenGLRenderTarget.hpp"

#include <vector>

namespace vantor::Graphics::RenderDevice::OpenGL
{
    class Renderer;
    class Material;
    class Mesh;
    class Texture;
    class Shader;
    class PostProcessor;

    // ====== Base PBRCapture ======
    struct PBRCapture
    {
            TextureCube *Irradiance  = nullptr;
            TextureCube *Prefiltered = nullptr;

            glm::vec3 Position;
            float     Radius;
    };

    class PBR
    {
            friend Renderer;
            friend PostProcessor;

        private:
            Renderer *m_Renderer;

            std::vector<PBRCapture *> m_CaptureProbes;
            PBRCapture               *m_SkyCapture;
            RenderTarget             *m_RenderTargetBRDFLUT;

            Material          *m_PBRHdrToCubemap;
            Material          *m_PBRIrradianceCapture;
            Material          *m_PBRPrefilterCapture;
            Material          *m_PBRIntegrateBRDF;
            Mesh              *m_PBRCaptureCube;
            vantor::SceneNode *m_SceneEnvCube;

            Shader *m_ProbeCaptureShader;
            Shader *m_ProbeCaptureBackgroundShader;

            // debug
            Mesh   *m_ProbeDebugSphere;
            Shader *m_ProbeDebugShader;

        public:
            PBR(Renderer *renderer);
            ~PBR();

            // sets the combined irradiance/pre-filter global environment
            // skylight
            void SetSkyCapture(PBRCapture *capture);
            // adds a processed PBR capture to the list of irradiance probes
            void AddIrradianceProbe(PBRCapture *capture, glm::vec3 position, float radius);
            // removes all irradiance probe entries from the global GI grid
            void ClearIrradianceProbes();
            // generate an irradiance and pre-filter map out of a 2D
            // equirectangular map (preferably HDR)
            PBRCapture *ProcessEquirectangular(Texture *envMap);
            // generate an irradiance and pre-filter map out of a cubemap
            // texture
            PBRCapture *ProcessCube(TextureCube *capture, bool prefilter = true);

            // retrieves the environment skylight
            PBRCapture *GetSkyCapture();
            // retrieve all pushed irradiance probes
            std::vector<PBRCapture *> GetIrradianceProbes(glm::vec3 queryPos, float queryRadius);

            // renders all reflection/irradiance probes for
            // visualization/debugging.
            void RenderProbes();
    };
} // namespace vantor::Graphics::RenderDevice::OpenGL