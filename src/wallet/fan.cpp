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

#include <wallet/fan.hpp>

chratex::fan::fan(chratex::uint256_union const & key, size_t count_a) {
	std::unique_ptr<chratex::uint256_union> first(new chratex::uint256_union(key));
	for (auto i (1); i < count_a; ++i) {
		std::unique_ptr<chratex::uint256_union> entry(new chratex::uint256_union);
		random_pool.GenerateBlock(entry->bytes.data(), entry->bytes.size());
		*first ^= *entry;
		values.push_back(std::move(entry));
	}
	values.push_back(std::move(first));
}

void chratex::fan::value(chratex::raw_key & prv_a) {
	std::lock_guard<std::mutex> lock (mutex);
	value_get (prv_a);
}

void chratex::fan::value_get(chratex::raw_key & prv_a) {
	assert(!mutex.try_lock());
	prv_a.data.clear();
	for (auto & i : values) {
		prv_a.data ^= *i;
	}
}

void chratex::fan::value_set(chratex::raw_key const & value_a) {
	std::lock_guard<std::mutex> lock(mutex);
	chratex::raw_key value_l;
	value_get(value_l);
	*(values[0]) ^= value_l.data;
	*(values[0]) ^= value_a.data;
}


