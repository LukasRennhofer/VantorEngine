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
 *  Date: 2025-07-01
 *
 *  File: VOS_Component.hpp
 *  Last Change: Automatically updated
 */

#pragma once
#include <memory>
#include <string>

namespace Vantor::Object
{

    class VObject; // Forward declaration

    class VComponent
    {
        public:
            explicit VComponent(VObject *owner) : owner(owner) {}
            virtual ~VComponent() = default;

            VObject *GetOwner() const { return owner; }

        private:
            VObject *owner;
    };

    using VComponentPtr = std::shared_ptr<VComponent>;
} // namespace Vantor::Object
