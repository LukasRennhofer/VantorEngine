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
 *  File: vantorECS.h
 *  Last Change: Automatically updated
 */

// EnTT
#include <entt/entt.hpp>
#include <string>

/*
This header is being used for Vantors base entity component system and its
implementation with the EnTT Framework e.g: using namespace vantor::Entity;

    void CreatePlayer() {
        Entity player = Entity::Create();
        player.AddComponent<TransformComponent>(0.0f, 5.0f, 0.0f);
        player.AddComponent<TagComponent>("Player");
    }

*/

namespace vantor::Entity
{

    class Entity;

    class Registry
    {
        public:
            static entt::registry &Get();
            static void            Clear();
    };

    class Entity
    {
        public:
            Entity();
            explicit Entity(entt::entity handle);

            static Entity Create();

            void Destroy();

            template <typename T, typename... Args> T &AddComponent(Args &&...args)
            {
                return Registry::Get().emplace<T>(m_Entity, std::forward<Args>(args)...);
            }

            template <typename T> void RemoveComponent() { Registry::Get().remove<T>(m_Entity); }

            template <typename T> T &GetComponent() { return Registry::Get().get<T>(m_Entity); }

            template <typename T> bool HasComponent() const { return Registry::Get().all_of<T>(m_Entity); }

            bool         IsValid() const;
            entt::entity GetHandle() const;

            bool operator==(const Entity &other) const;
            bool operator!=(const Entity &other) const;

        private:
            entt::entity m_Entity;
    };

} // namespace vantor::Entity
