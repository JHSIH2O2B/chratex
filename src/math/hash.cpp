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

#include "hash.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <errno.h>
#include <new>
#include <stdexcept>
#include <math/external/sha256.h>

namespace chratex {
hash_digest sha256_hash(data_slice data) {
  hash_digest hash;
  SHA256_(data.data(), data.size(), hash.data());
  return hash;
}

data_chunk sha256_hash_chunk(data_slice data) {
  data_chunk hash(hash_size);
  SHA256_(data.data(), data.size(), hash.data());
  return hash;
}

hash_digest sha256_hash(data_slice first, data_slice second) {
  hash_digest hash;
  SHA256CTX context;
  SHA256Init(&context);
  SHA256Update(&context, first.data(), first.size());
  SHA256Update(&context, second.data(), second.size());
  SHA256Final(&context, hash.data());
  return hash;
}
}

