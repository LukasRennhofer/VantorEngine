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
 *  File: VOS_Object.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include "Component/VOS_Component.hpp"

#include <memory>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include <string>
#include <cstdint>
#include <algorithm>

/*
 * ========================================================================================
 * VObject — Core Entity Base Class
 * ========================================================================================
 *
 * VObject represents a generic game entity or object in the engine. It acts as a base class
 * that all custom objects (e.g., Player, Camera, PointLight, ...) should inherit from.
 *
 * It provides the following core systems:
 *
 * 1. Unique ID System:
 *    - Every VObject has a globally unique ID (VObjectID) assigned on creation.
 *
 * 2. Component System (Component-Based Architecture):
 *    - Supports dynamic addition and retrieval of components using type-safe templates.
 *    - Components must derive from `VComponent`.
 *    - Stored in an internal map keyed by `std::type_index`.
 *    - Methods:
 *      - AddComponent<T>(args...)     → adds and constructs component T
 *      - GetComponent<T>()            → returns component T if it exists
 *      - HasComponent<T>()            → checks if component T exists
 *      - RemoveComponent(type_index)  → removes a component by type
 *
 * 3. Scene Graph / Hierarchical Object Structure:
 *    - Supports parent-child relationships between objects.
 *    - Used for transforms, grouping, and logical object trees (e.g., Player → Weapon).
 *    - Children are stored as shared_ptrs; parent is held as weak_ptr to avoid cycles.
 *    - Methods:
 *      - AddChild(child)              → attach a child object
 *      - RemoveChild(child)           → detach a specific child
 *      - GetParent()                  → returns parent (if any)
 *      - GetChildren()                → returns list of children
 *
 * 4. Inheritance:
 *    - Intended to be inherited by all custom game object types.
 *    - Virtual destructor for safe polymorphic deletion.
 *
 * Example Usage:
 *    auto player = Vantor::Object::VORegistry::CreateEntity<Player>();
 *    player->AddComponent<TransformComponent>();
 *    player->AddChild(swordEntity);
 *
 * This class is central to the engine's object model.
 * ========================================================================================
 */

namespace Vantor::Object
{

    using VObjectID = uint64_t;

    class VObject : public std::enable_shared_from_this<VObject>
    {
        public:
            VObject() : id(nextID++) {}
            virtual ~VObject() = default;

            VObjectID GetID() const { return id; }

            // Component management

            template <typename T, typename... Args> std::shared_ptr<T> AddComponent(Args &&...args)
            {
                static_assert(std::is_base_of<VComponent, T>::value, "T must derive from VComponent");
                auto comp                              = std::make_shared<T>(this, std::forward<Args>(args)...);
                components[std::type_index(typeid(T))] = comp;
                return comp;
            }

            template <typename T> std::shared_ptr<T> GetComponent() const
            {
                auto it = components.find(std::type_index(typeid(T)));
                if (it != components.end())
                {
                    return std::static_pointer_cast<T>(it->second);
                }
                return nullptr;
            }

            template <typename T> bool HasComponent() const { return components.count(std::type_index(typeid(T))) > 0; }

            void RemoveComponent(std::type_index compType) { components.erase(compType); }

            // Scene graph
            void AddChild(std::shared_ptr<VObject> child)
            {
                if (child)
                {
                    child->parent = shared_from_this();
                    children.push_back(std::move(child));
                }
            }

            void RemoveChild(std::shared_ptr<VObject> child)
            {
                if (!child) return;
                auto it = std::find(children.begin(), children.end(), child);
                if (it != children.end())
                {
                    (*it)->parent.reset();
                    children.erase(it);
                }
            }

            std::weak_ptr<VObject> GetParent() const { return parent; }

            const std::vector<std::shared_ptr<VObject>> &GetChildren() const { return children; }

            VObjectID id; // The Entity Object ID

        private:
            std::unordered_map<std::type_index, VComponentPtr> components;

            std::vector<std::shared_ptr<VObject>> children;
            std::weak_ptr<VObject>                parent;

            static VObjectID nextID;
    };

    // Initialize static member
    inline VObjectID VObject::nextID = 1;

} // namespace Vantor::Object
