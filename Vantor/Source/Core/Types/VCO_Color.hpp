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
 *  File: VCO_Color.hpp
 *  Last Change: Automatically updated
 */

#pragma once

// Math
#include <cstdint>

#include "../../Math/Linear/VMA_Vector.hpp"
#include "../../Math/VMA_Common.hpp"

namespace Vantor::Core::Types
{
    struct VColor
    {
            uint32_t rgba = 0;

            constexpr VColor(uint32_t rgba) : rgba(rgba) {}
            constexpr VColor(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 255)
                : rgba(uint32_t(r) | (uint32_t(g) << 8) | (uint32_t(b) << 16) | (uint32_t(a) << 24))
            {
            }

            constexpr VColor(const char *hex)
            {
                rgba           = 0;
                uint32_t shift = 0u;
                for (int i = 0; i < 9; ++i)
                {
                    const char c = hex[i];
                    switch (c)
                    {
                        case '0' ... '9':
                            rgba |= (c - '0') << shift;
                            shift += 4u;
                            break;
                        case 'A' ... 'F':
                            rgba |= (c - 'A' + 10) << shift;
                            shift += 4u;
                            break;
                        case '#':
                            break;
                        default:
                        case 0:
                            return;
                    }
                }
            }

            constexpr uint8_t getR() const { return (rgba >> 0) & 0xFF; }
            constexpr uint8_t getG() const { return (rgba >> 8) & 0xFF; }
            constexpr uint8_t getB() const { return (rgba >> 16) & 0xFF; }
            constexpr uint8_t getA() const { return (rgba >> 24) & 0xFF; }

            constexpr void setR(uint8_t value) { *this = VColor(value, getG(), getB(), getA()); }
            constexpr void setG(uint8_t value) { *this = VColor(getR(), value, getB(), getA()); }
            constexpr void setB(uint8_t value) { *this = VColor(getR(), getG(), value, getA()); }
            constexpr void setA(uint8_t value) { *this = VColor(getR(), getG(), getB(), value); }

            Vantor::Math::VVector3 toFloat3() const
            {
                return Vantor::Math::VVector3(((rgba >> 0) & 0xFF) / 255.0f, ((rgba >> 8) & 0xFF) / 255.0f, ((rgba >> 16) & 0xFF) / 255.0f);
            }

            Vantor::Math::VVector4 toFloat4() const
            {
                return Vantor::Math::VVector4(((rgba >> 0) & 0xFF) / 255.0f, ((rgba >> 8) & 0xFF) / 255.0f, ((rgba >> 16) & 0xFF) / 255.0f,
                                              ((rgba >> 24) & 0xFF) / 255.0f);
            }

            operator Vantor::Math::VVector3() const { return toFloat3(); }
            operator Vantor::Math::VVector4() const { return toFloat4(); }
            operator uint32_t() const { return rgba; }

            template <std::size_t capacity> struct char_return
            {
                    char      text[capacity] = {};
                    constexpr operator const char *() const { return text; }
            };

            constexpr const char_return<9> to_hex() const
            {
                char_return<9> ret;
                for (int i = 0; i < 8; ++i)
                {
                    const uint8_t v = (rgba >> (i * 4)) & 0xF;
                    ret.text[i]     = v < 10 ? ('0' + v) : ('A' + v - 10);
                }
                return ret;
            }

            static constexpr VColor fromFloat4(const Vantor::Math::VVector4 &value)
            {
                return VColor((uint8_t) (value.x * 255), (uint8_t) (value.y * 255), (uint8_t) (value.z * 255), (uint8_t) (value.w * 255));
            }

            static constexpr VColor fromFloat3(const Vantor::Math::VVector3 &value)
            {
                return VColor((uint8_t) (value.x * 255), (uint8_t) (value.y * 255), (uint8_t) (value.z * 255));
            }

            static VColor lerp(VColor a, VColor b, float i) { return fromFloat4(Vantor::Math::Lerp(a.toFloat4(), b.toFloat4(), i)); }

            // Preset VColors
            static constexpr VColor Red() { return VColor(255, 0, 0, 255); }
            static constexpr VColor Green() { return VColor(0, 255, 0, 255); }
            static constexpr VColor Blue() { return VColor(0, 0, 255, 255); }
            static constexpr VColor Black() { return VColor(0, 0, 0, 255); }
            static constexpr VColor White() { return VColor(255, 255, 255, 255); }
            static constexpr VColor Yellow() { return VColor(255, 255, 0, 255); }
            static constexpr VColor Purple() { return VColor(255, 0, 255, 255); }
            static constexpr VColor Cyan() { return VColor(0, 255, 255, 255); }
            static constexpr VColor Transparent() { return VColor(0, 0, 0, 0); }
            static constexpr VColor Gray() { return VColor(127, 127, 127, 255); }
            static constexpr VColor Ghost() { return VColor(127, 127, 127, 127); }
            static constexpr VColor Booger() { return VColor(127, 127, 127, 200); }
            static constexpr VColor Shadow() { return VColor(0, 0, 0, 100); }

            static constexpr VColor Warning() { return 0xFF66FFFF; }
            static constexpr VColor Error() { return 0xFF6666FF; }
    };
} // namespace Vantor::Core::Types