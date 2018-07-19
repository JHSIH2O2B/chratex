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
#ifndef CHRATEX_DATA_IPP
#define CHRATEX_DATA_IPP

namespace chratex {

inline data_chunk to_chunk(uint8_t byte) {
  return data_chunk{ byte };
}

inline data_chunk build_chunk(loaf slices, size_t extra_reserve) {
  size_t size = 0;
  for (const auto slice: slices) {
    size += slice.size();
  }

  data_chunk out;
  out.reserve(size + extra_reserve);
  for (const auto slice: slices) {
    out.insert(out.end(), slice.begin(), slice.end());
  }

  return out;
}

}

#endif
