#pragma once

// Math
#include "../../Math/LinearAlgebra/VMA_Vector.hpp"
#include "../../Math/VMA_common.hpp"

#include <cstdint>

namespace Vantor::Helpers
{
    struct Color
    {
            uint32_t rgba = 0;

            constexpr Color(uint32_t rgba) : rgba(rgba) {}
            constexpr Color(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 255)
                : rgba(uint32_t(r) | (uint32_t(g) << 8) | (uint32_t(b) << 16) | (uint32_t(a) << 24))
            {
            }

            constexpr Color(const char *hex)
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

            constexpr void setR(uint8_t value) { *this = Color(value, getG(), getB(), getA()); }
            constexpr void setG(uint8_t value) { *this = Color(getR(), value, getB(), getA()); }
            constexpr void setB(uint8_t value) { *this = Color(getR(), getG(), value, getA()); }
            constexpr void setA(uint8_t value) { *this = Color(getR(), getG(), getB(), value); }

            Vantor::Math::Vec3 toFloat3() const
            {
                return Vantor::Math::Vec3(((rgba >> 0) & 0xFF) / 255.0f, ((rgba >> 8) & 0xFF) / 255.0f, ((rgba >> 16) & 0xFF) / 255.0f);
            }

            Vantor::Math::Vec4 toFloat4() const
            {
                return Vantor::Math::Vec4(((rgba >> 0) & 0xFF) / 255.0f, ((rgba >> 8) & 0xFF) / 255.0f, ((rgba >> 16) & 0xFF) / 255.0f,
                                                   ((rgba >> 24) & 0xFF) / 255.0f);
            }

            operator Vantor::Math::Vec3() const { return toFloat3(); }
            operator Vantor::Math::Vec4() const { return toFloat4(); }
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

            static constexpr Color fromFloat4(const Vantor::Math::Vec4 &value)
            {
                return Color((uint8_t) (value.x * 255), (uint8_t) (value.y * 255), (uint8_t) (value.z * 255), (uint8_t) (value.w * 255));
            }

            static constexpr Color fromFloat3(const Vantor::Math::Vec3 &value)
            {
                return Color((uint8_t) (value.x * 255), (uint8_t) (value.y * 255), (uint8_t) (value.z * 255));
            }

            static constexpr Color lerp(Color a, Color b, float i) { return fromFloat4(Vantor::Math::Lerp(a.toFloat4(), b.toFloat4(), i)); }

            // Preset Colors
            static constexpr Color Red() { return Color(255, 0, 0, 255); }
            static constexpr Color Green() { return Color(0, 255, 0, 255); }
            static constexpr Color Blue() { return Color(0, 0, 255, 255); }
            static constexpr Color Black() { return Color(0, 0, 0, 255); }
            static constexpr Color White() { return Color(255, 255, 255, 255); }
            static constexpr Color Yellow() { return Color(255, 255, 0, 255); }
            static constexpr Color Purple() { return Color(255, 0, 255, 255); }
            static constexpr Color Cyan() { return Color(0, 255, 255, 255); }
            static constexpr Color Transparent() { return Color(0, 0, 0, 0); }
            static constexpr Color Gray() { return Color(127, 127, 127, 255); }
            static constexpr Color Ghost() { return Color(127, 127, 127, 127); }
            static constexpr Color Booger() { return Color(127, 127, 127, 200); }
            static constexpr Color Shadow() { return Color(0, 0, 0, 100); }

            static constexpr Color Warning() { return 0xFF66FFFF; }
            static constexpr Color Error() { return 0xFF6666FF; }
    };
} // namespace Vantor::Helpers
