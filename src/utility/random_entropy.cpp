/**
 * Copyright (c) 2018 chratex developers (see AUTHORS)
 *
 * This file is part of chratex
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

#include <utility/random_entropy.hpp>
#include <boost/random/random_device.hpp>
#include <random>

libbitcoin::data_chunk chratex::create_cs_random() {
  auto slice = libbitcoin::data_chunk(32u);
  boost::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distribution(
      0, std::numeric_limits<uint8_t>::max()
      );
  const auto fill = [&distribution, &gen](uint8_t) {
    return static_cast<uint8_t>(distribution(gen));
  };

  std::transform(slice.begin(), slice.end(), slice.begin(), fill);
  return slice;
}
