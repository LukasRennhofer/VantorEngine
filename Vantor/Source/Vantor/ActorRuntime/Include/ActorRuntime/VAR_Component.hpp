/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once
#include <memory>
#include <string>

namespace VE::Internal::ActorRuntime
{

    class AActor; // Forward declaration

    class CComponent
    {
        public:
            explicit CComponent(AActor *owner) : owner(owner) {}
            virtual ~CComponent() = default;

            AActor *GetOwner() const { return owner; }

        private:
            AActor *owner;
    };

    using VComponentPtr = std::shared_ptr<CComponent>;
} // namespace VE::Internal::ActorRuntime