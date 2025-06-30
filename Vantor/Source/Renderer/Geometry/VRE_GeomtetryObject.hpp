/*
 *  ╔═══════════════════════════════════════════════════════════════╗
 *  ║                          ~ Vantor ~                           ║
 *  ║                                                               ║
 *  ║  This file is part of the Vantor Engine.                      ║
 *  ║  Automatically formatted by vtrgFormat.py                     ║
 *  ║                                                               ║
 *  ╚═══════════════════════════════════════════════════════════════╝
 *
 *  Copyright (c) 2025 Lukas Rennhofer
 *  Licensed under the GNU General Public License, Version 3.
 *  See LICENSE file for more details.
 *
 *  Author: Lukas Rennhofer
 *  Date: 2025-06-30
 *
 *  File: VRE_GeomtetryObject.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include "../../RenderDevice/Interface/VRD_Mesh.hpp"

#include <memory>

namespace Vantor::Renderer
{

    enum VEGeometryObjectType
    {
        SPHERE,
        NOTSET
    };

    class VGObject
    {
        public:
            Vantor::RenderDevice::VMesh *GetMesh() { return objectMesh.get(); }

            // setters
            void SetMesh(std::shared_ptr<Vantor::RenderDevice::VMesh> meshPtr) { objectMesh = std::move(meshPtr); }

            // getters
            VEGeometryObjectType GetObjectType() { return objectType; }

        protected:
            std::shared_ptr<Vantor::RenderDevice::VMesh> objectMesh;
            VEGeometryObjectType                         objectType = NOTSET; // Set the GeometryObjectType
    };
} // namespace Vantor::Renderer