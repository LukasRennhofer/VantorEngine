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
 *  File: VCO_Types.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include <cstddef>
#include <cstdint>

namespace Vantor
{
    // This file is being used for all standart Types in Vantor
    // NOTE (LUKAS) : I added some descriptions to the types and
    //                how many numbers you can represent with a
    //                single type for better understanding
    // Unsigned integer type aliases for cleaner, more explicit code
    using u8  = std::uint8_t;  // 8-bit unsigned integer (0 to 255)
    using u16 = std::uint16_t; // 16-bit unsigned integer (0 to 65,535)
    using u32 = std::uint32_t; // 32-bit unsigned integer (0 to ~4.3 billion)
    using u64 = std::uint64_t; // 64-bit unsigned integer (0 to ~18 quintillion)

    // Signed integer type aliases for cleaner, more explicit code
    using s8  = std::int8_t;  // 8-bit signed integer (-128 to 127)
    using s16 = std::int16_t; // 16-bit signed integer (-32,768 to 32,767)
    using s32 = std::int32_t; // 32-bit signed integer (~-2.1 to 2.1 billion)
    using s64 = std::int64_t; // 64-bit signed integer (~-9 to 9 quintillion)

    // Floating-point type aliases for consistency
    using f32 = float;  // 32-bit floating-point number (single precision)
    using f64 = double; // 64-bit floating-point number (double precision)

    // Character and size type aliases
    using char16 = char16_t;    // 16-bit Unicode character type
    using size_t = std::size_t; // Platform-specific size type for array indices and memory sizes
} // namespace Vantor