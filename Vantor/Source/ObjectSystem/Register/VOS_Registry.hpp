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
 *  File: VOS_Registry.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include "../VOS_Object.hpp"

#include <unordered_map>
#include <memory>
#include <typeindex>
#include <cassert>

namespace Vantor::Object
{

    class VORegistry
    {
        public:
            VORegistry() = delete;

            template <typename T, typename... Args> static std::shared_ptr<T> CreateEntity(Args &&...args)
            {
                static_assert(std::is_base_of_v<VObject, T>, "T must inherit from VObject");
                auto entity          = std::make_shared<T>(std::forward<Args>(args)...);
                entity->id           = nextID++;
                entities[entity->id] = entity;
                return entity;
            }

            static std::shared_ptr<VObject> GetEntity(VObjectID id)
            {
                auto it = entities.find(id);
                if (it != entities.end())
                {
                    return it->second;
                }
                return nullptr;
            }

            static void DestroyEntity(VObjectID id) { entities.erase(id); }

        private:
            inline static std::unordered_map<VObjectID, std::shared_ptr<VObject>> entities;
            inline static VObjectID                                               nextID = 0;
    };

} // namespace Vantor::Object
