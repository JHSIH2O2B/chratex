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

#include <wallet/value.hpp>

chratex::wallet_value::wallet_value(chratex::mdb_val const &val) {
	assert(val.size () == sizeof (*this));

	std::copy(
    reinterpret_cast<uint8_t const *>(val.data()), 
    reinterpret_cast<uint8_t const *>(val.data()) + sizeof(key), 
    key.chars.begin()
  );
	std::copy(
    reinterpret_cast<uint8_t const *>(val.data()) + sizeof(key),
    reinterpret_cast<uint8_t const *>(val.data()) + sizeof(key) + sizeof(work),
    reinterpret_cast<char *> (&work)
  );
}

chratex::wallet_value::wallet_value(
  chratex::uint256_union const & key,
  uint64_t work
) : key (key_a), work (work_a) {
}

chratex::mdb_val chratex::wallet_value::val() const {
	static_assert(
    sizeof(*this) == sizeof(key) + sizeof(work), "Class not packed"
  );
	return chratex::mdb_val(
    sizeof(*this), const_cast<chratex::wallet_value *>(this)
  );
}
