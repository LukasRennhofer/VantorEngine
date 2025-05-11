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
 *  Date: 2025-05-11
 *
 *  File: vathMatrix.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include <initializer_list>
#include <assert.h>

#include "vathVector.hpp"

namespace vath
{
    /*
      Matrix numbering by vath conventions:
      |  0  1  2  3 |
      |  4  5  6  7 |
      |  8  9 10 11 |
      | 12 13 14 15 |
      Column-major layout in memory:
      [ 0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15 ]
      Matrix numbering if we access column-major memory sequentially in the
      array: |  0  4  8 12 | |  1  5  9 13 | |  2  6 10 14 | |  3  7 11 15 |

      There is no need for matrix template specialization.

    */
    template <std::size_t m, std::size_t n, typename T> struct matrix
    {
            union
            {
                    T e[n][m];
                    struct
                    {
                            vector<m, T> col[n];
                    };
            };
            // --------------------------------------------------------------------------------------------
            // consturctor0: default initializes matrix to identity matrix
            matrix()
            {
                for (std::size_t col = 0; col < n; ++col)
                {
                    for (std::size_t row = 0; row < m; ++row)
                    {
                        e[col][row] = (col == row) ? T(1.0f) : T(0.0f);
                    }
                }
            }
            // --------------------------------------------------------------------------------------------
            // constructor1: initialize matrix with initializer list
            matrix(const std::initializer_list<T> args)
            {
                assert(args.size() <= m * n);
                std::size_t cols = 0, rows = 0;

                for (auto &it : args)
                {
                    e[cols][rows++] = it;
                    if (rows >= m)
                    {
                        ++cols;
                        rows = 0;
                    }
                }
            }
            // --------------------------------------------------------------------------------------------
            vector<m, T> &operator[](const std::size_t colIndex)
            {
                assert(colIndex >= 0 && colIndex < n);
                return col[colIndex];
            }
    };

    typedef matrix<2, 2, float>  mat2;
    typedef matrix<3, 3, float>  mat3;
    typedef matrix<4, 4, float>  mat4;
    typedef matrix<2, 2, double> dmat2;
    typedef matrix<3, 3, double> dmat3;
    typedef matrix<4, 4, double> dmat4;

    template <std::size_t m, std::size_t n, typename T> matrix<m, n, T> operator+(matrix<m, n, T> &lhs, matrix<m, n, T> &rhs)
    {
        matrix<m, n, T> result;
        for (std::size_t col = 0; col < n; ++col)
        {
            for (std::size_t row = 0; row < m; ++row)
            {
                result[col][row] = lhs[col][row] + rhs[col][row];
            }
        }
        return result;
    }
    // subtraction
    // --------------------------------------------------------------------------------------------
    template <std::size_t m, std::size_t n, typename T> matrix<m, n, T> operator-(matrix<m, n, T> &lhs, matrix<m, n, T> &rhs)
    {
        matrix<m, n, T> result;
        for (std::size_t col = 0; col < n; ++col)
        {
            for (std::size_t row = 0; row < m; ++row)
            {
                result[col][row] = lhs[col][row] - rhs[col][row];
            }
        }
        return result;
    }
    // multiplication
    // --------------------------------------------------------------------------------------------
    template <std::size_t m, std::size_t n, std::size_t o, typename T> matrix<m, o, T> operator*(matrix<m, n, T> &lhs, matrix<n, o, T> &rhs)
    {
        matrix<m, o, T> result;
        for (std::size_t col = 0; col < o; ++col)
        {
            for (std::size_t row = 0; row < m; ++row)
            {
                T value = {};
                for (std::size_t j = 0; j < n; ++j)
                {
                    value += lhs[j][row] * rhs[col][j];
                }
                result[col][row] = value;
            }
        }
        return result;
    }
    // multiplication with reference matrix (store directly inside provided
    // matrix)
    // --------------------------------------------------------------------------------------------
    template <std::size_t m, std::size_t n, std::size_t o, typename T>
    matrix<m, o, T> &mul(matrix<m, n, T> &result, const matrix<m, n, T> &lhs, const matrix<n, o, T> &rhs)
    {
        for (std::size_t col = 0; col < o; ++col)
        {
            for (std::size_t row = 0; row < m; ++row)
            {
                T value = {};
                for (std::size_t j = 0; j < n; ++j)
                {
                    value += lhs[j][row] * rhs[col][j];
                }
                result[col][row] = value;
            }
        }
        return result;
    }
    // matrix * vector multiplication
    // --------------------------------------------------------------------------------------------
    template <std::size_t m, std::size_t n, typename T> vector<m, T> operator*(matrix<m, n, T> &lhs, vector<n, T> &rhs)
    {
        vector<m, T> result;
        for (std::size_t row = 0; row < m; ++row)
        {
            T value = {};
            for (std::size_t j = 0; j < n; ++j) // j equals col in vath notation (i = row)
            {
                value += lhs[j][row] * rhs[j];
            }
            result[row] = value;
        }
        return result;
    }
} // namespace vath