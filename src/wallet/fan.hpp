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

#pragma once

#ifndef CHRATEX_WALLET_FAN_HPP
#define CHRATEX_WALLET_FAN_HPP

#include <lib/numbers.hpp>

namespace chratex {

class fan {
public:
	fan(chratex::uint256_union const &, size_t);
	void value(chratex::raw_key &);
	void value_set(chratex::raw_key const &);
	std::vector<std::unique_ptr<chratex::uint256_union>> values;

private:
	std::mutex mutex;
	void value_get(chratex::raw_key &);
};

}
#endif
