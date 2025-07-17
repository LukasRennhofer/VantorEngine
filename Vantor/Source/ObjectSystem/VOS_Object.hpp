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
 *  Date: 2025-07-16
 *
 *  File: VOS_Object.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include <algorithm>
#include <cstdint>
#include <memory>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>

#include "Component/VOS_Component.hpp"
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

            template <typename T, typename... Args> void AddComponentVoid(Args &&...args)
            {
                static_assert(std::is_base_of<VComponent, T>::value, "T must derive from VComponent");
                auto comp = AddComponent<T>(std::forward<Args>(args)...);
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