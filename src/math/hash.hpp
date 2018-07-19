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
#ifndef CHRATEX_MATH_HASH_HPP
#define CHRATEX_MATH_HASH_HPP

#include <cstddef>
#include <string>
#include <vector>
#include "../utility/data.hpp"

namespace chratex {

// Common hash container sizes.
static constexpr size_t hash_size = 32;
static constexpr size_t half_hash_size = hash_size / 2;
static constexpr size_t quarter_hash_size = half_hash_size / 2;
static constexpr size_t long_hash_size = 2 * hash_size;
static constexpr size_t short_hash_size = 20;
static constexpr size_t mini_hash_size = 6;

// Common hash containers.
typedef byte_array<hash_size> hash_digest;
typedef byte_array<half_hash_size> half_hash;
typedef byte_array<quarter_hash_size> quarter_hash;
typedef byte_array<long_hash_size> long_hash;
typedef byte_array<short_hash_size> short_hash;
typedef byte_array<mini_hash_size> mini_hash;

// Lists of common hashes.
typedef std::vector<hash_digest> hash_list;
typedef std::vector<half_hash> half_hash_list;
typedef std::vector<quarter_hash> quarter_hash_list;
typedef std::vector<long_hash> long_hash_list;
typedef std::vector<short_hash> short_hash_list;
typedef std::vector<mini_hash> mini_hash_list;

/// Generate a sha256 hash.
hash_digest sha256_hash(data_slice data);
data_chunk sha256_hash_chunk(data_slice data);

/// Generate a sha256 hash.
/// This hash function was used in electrum seed stretching (obsoleted).
hash_digest sha256_hash(data_slice first, data_slice second);
}

#endif

