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

#ifndef CHRATEX_WALLET_VALUE_HPP
#define CHRATEX_WALLET_VALUE_HPP

namespace chratex {

class wallet_value {
public:
	wallet_value() = default;
	wallet_value(database::mdb_val const &);
	wallet_value(chratex::uint256_union const &, uint64_t);
	database::mdb_val val() const;
	chratex::private_key key;
	uint64_t work;
};

}
#endif
