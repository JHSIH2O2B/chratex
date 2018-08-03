/**
 * Copyright (c) 2011-2017 libbitcoin developers
 * Copyright (c) 2018 chratex developers (see AUTHORS)
 *
 * This file is part of chratex.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#ifndef CHRATEX_DATA_HPP
#define CHRATEX_DATA_HPP

#include <array>
#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <iterator>
#include <queue>
#include <vector>
#include <utility/array_slice.hpp>

namespace chratex {

template <size_t Size>
using byte_array = std::array<uint8_t, Size>;
template <size_t Size>
using byte_array = std::array<uint8_t, Size>;

template <size_t Size>
struct byte_array_parts
{
    byte_array<Size> left;
    byte_array<Size> right;
};

// Define arbitrary byte storage types.
typedef byte_array<1> one_byte;
typedef array_slice<uint8_t> data_slice;
typedef std::vector<uint8_t> data_chunk;
typedef std::queue<data_chunk> data_queue;
typedef std::vector<data_chunk> data_stack;
typedef std::initializer_list<data_slice> loaf;

/**
 * Concatenate several data slices into a single data_chunk.
 * @param  extra_reserve  Include this many additional bytes when calling
 * `reserve` on the data_chunk (as an optimization).
 */
inline data_chunk build_chunk(loaf slices, size_t extra_reserve=0);

/**
* Create a single byte data chunk with an initial value.
*/
inline data_chunk to_chunk(uint8_t byte);

/**
 * Create a data chunk from an iterable object.
 */
template <typename Source>
data_chunk to_chunk(const Source& bytes);

}

#include "data.ipp"

#endif

