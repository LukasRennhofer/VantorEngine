/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

#include <ActorRuntime/Public/VAR_Actor.hpp>

#include <memory>
#include <queue>

namespace VE {
    
    // The VWorld will manage In-Game Actors and Objects at Runtime
    // It will automatically detect different types of Actors and 
    // will exectute their Poll every Frame
    class VWorld {
        public:
            VWorld() = default;

            template <typename T, typename... Args> 
            std::shared_ptr<T> CreateActor(Args &&...args)
            {
                static_assert(std::is_base_of_v<AActor, T>, "T must inherit from AActor");
                auto entity = std::make_shared<T>(std::forward<Args>(args)...);

                // Reuse ID if available, otherwise increment
                VActorID id;
                if (!m_FreeIDs.empty())
                {
                    id = m_FreeIDs.front();
                    m_FreeIDs.pop();
                }
                else
                {
                    id = m_NextID++;
                }

                entity->id     = id;
                m_Actors[id]   = entity;
                return entity;
            }

            std::shared_ptr<AActor> GetActor(VActorID id)
            {
                auto it = m_Actors.find(id);
                if (it != m_Actors.end())
                {
                    return it->second;
                }
                return nullptr;
            }

            const std::unordered_map<VActorID, std::shared_ptr<AActor>> &GetAllActors() { return m_Actors; }

            VE::Internal::Core::Container::TVector<std::shared_ptr<AActor>> GetAllActorsList()
            {
                VE::Internal::Core::Container::TVector<std::shared_ptr<AActor>> list;
                list.reserve(m_Actors.size());
                for (const auto &[id, entity] : m_Actors)
                {
                    list.push_back(entity);
                }
                return list;
            }

            void DestroyActor(VActorID id) 
            {
                auto it = m_Actors.find(id);
                if (it != m_Actors.end())
                {
                    m_Actors.erase(it);
                    m_FreeIDs.push(id); // mark ID for reuse
                }
            }

        private:
            std::unordered_map<VActorID, std::shared_ptr<AActor>>  m_Actors;
            VActorID                                               m_NextID = 0;
            
            // pool of reusable IDs
            std::queue<VActorID>                                   m_FreeIDs;
    };
    
}