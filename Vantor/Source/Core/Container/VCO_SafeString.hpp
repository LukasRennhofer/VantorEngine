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
 *  File: VCO_SafeString.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include <algorithm>
#include <cassert>
#include <cstring>
#include <memory>

#include "../VCO_Types.hpp"

namespace Vantor::Core
{
    // Internal Implementation for a SafeString
    template <typename Alloc = std::allocator<char>> class SafeString
    {
        public:
            static constexpr size_t SSO_THRESHOLD = 23;

            SafeString() noexcept : m_size(0), m_using_sso(true) { m_sso_data[0] = '\0'; }

            SafeString(const char *str) : SafeString() { assign(str); }


            ~SafeString()
            {
                if (!m_using_sso)
                {
                    m_allocator.deallocate(m_data, m_capacity + 1);
                }
            }

            void assign(const char *str)
            {
                size_t len = std::strlen(str);
                if (len <= SSO_THRESHOLD)
                {
                    std::memcpy(m_sso_data, str, len + 1);
                    m_size      = len;
                    m_using_sso = true;
                }
                else
                {
                    m_data = m_allocator.allocate(len + 1);
                    std::memcpy(m_data, str, len + 1);
                    m_size      = len;
                    m_capacity  = len;
                    m_using_sso = false;
                }
            }

            size_t      size() const { return m_size; }
            const char *c_str() const { return m_using_sso ? m_sso_data : m_data; }

            size_t find(const char *substr) const
            {
                const char *found = std::strstr(c_str(), substr);
                return found ? static_cast<size_t>(found - c_str()) : std::string::npos;
            }

            SafeString substr(size_t pos, size_t len) const
            {
                assert(pos + len <= m_size);
                char temp[SSO_THRESHOLD + 1] = {};
                std::memcpy(temp, c_str() + pos, len);
                temp[len] = '\0';
                return SafeString(temp);
            }

            u32 hash() const noexcept
            {
                constexpr u32 FNV_offset_basis = 2166136261u;
                constexpr u32 FNV_prime = 16777619u;

                u32 hash = FNV_offset_basis;
                const char* ptr = c_str();
                for (size_t i = 0; i < m_size; ++i)
                {
                    hash ^= static_cast<u8>(ptr[i]);
                    hash *= FNV_prime;
                }
                return hash;
            }

            void replace(size_t pos, size_t len, const char *str)
            {
                assert(pos + len <= m_size);
                SafeString temp = substr(0, pos);
                temp += str;
                temp += substr(pos + len, m_size - pos - len);
                *this = temp;
            }

            SafeString &operator+=(const char *str)
            {
                size_t new_len = m_size + std::strlen(str);
                if (new_len <= SSO_THRESHOLD)
                {
                    std::strcat(m_sso_data, str);
                    m_size = new_len;
                }
                else
                {
                    char *new_data = m_allocator.allocate(new_len + 1);
                    std::strcpy(new_data, c_str());
                    std::strcat(new_data, str);
                    if (!m_using_sso) m_allocator.deallocate(m_data, m_capacity + 1);
                    m_data      = new_data;
                    m_capacity  = new_len;
                    m_size      = new_len;
                    m_using_sso = false;
                }
                return *this;
            }

            const char &operator[](size_t i) const
            {
                assert(i < m_size);
                return m_using_sso ? m_sso_data[i] : m_data[i];
            }

            char &operator[](size_t i)
            {
                assert(i < m_size);
                return m_using_sso ? m_sso_data[i] : m_data[i];
            }

            const char* begin() const { return c_str(); }
            const char* end() const { return c_str() + m_size; }

            char* begin() { return const_cast<char*>(c_str()); }
            char* end() { return const_cast<char*>(c_str() + m_size); }


        private:
            union
            {
                    char  m_sso_data[SSO_THRESHOLD + 1]; // +1 for null terminator
                    char *m_data;
            };
            size_t m_size      = 0;
            size_t m_capacity  = 0;
            bool   m_using_sso = true;
            Alloc  m_allocator;
    };

} // namespace Vantor::Core