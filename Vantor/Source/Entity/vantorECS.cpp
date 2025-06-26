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
 *  Date: 2025-06-26
 *
 *  File: vantorECS.cpp
 *  Last Change: Automatically updated
 */

#include "vantorECS.h"

// TODO: Move this away to Scene and delete Entity Folder
namespace Vantor::Entity
{

    // Registry implementation
    entt::registry &Registry::Get()
    {
        static entt::registry instance;
        return instance;
    }

    void Registry::Clear() { Get().clear(); }

    // Entity implementation
    Entity::Entity() : m_Entity(entt::null) {}

    Entity::Entity(entt::entity handle) : m_Entity(handle) {}

    Entity Entity::Create() { return Entity(Registry::Get().create()); }

    void Entity::Destroy()
    {
        if (IsValid())
        {
            Registry::Get().destroy(m_Entity);
            m_Entity = entt::null;
        }
    }

    bool Entity::IsValid() const { return Registry::Get().valid(m_Entity); }

    entt::entity Entity::GetHandle() const { return m_Entity; }

    bool Entity::operator==(const Entity &other) const { return m_Entity == other.m_Entity; }

    bool Entity::operator!=(const Entity &other) const { return !(*this == other); }

} // namespace Vantor::Entity