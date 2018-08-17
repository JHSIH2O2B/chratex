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

#ifndef CHRATEX_LIB_COMMON_HPP
#define CHRATEX_LIB_COMMON_HPP

#include <lib/numbers.hpp>

namespace chratex {

/**
 * A key pair. The private key is generated from the random pool, or passed in
 * as a hex string. The public key is derived using ed25519.
 */
class keypair {
public:
	keypair();
	keypair(std::string const &);
	keypair(chratex::raw_key &&);
	chratex::public_key pub;
	chratex::raw_key prv;
};

/**
 * Tag for which epoch an entry belongs to
 */
enum class epoch : uint8_t {
	invalid = 0,
	unspecified = 1,
	epoch_0 = 2,
	epoch_1 = 3
};

extern chratex::keypair const &zero_key;
extern chratex::keypair const &test_genesis_key;
extern chratex::account const &test_account;
extern chratex::account const &beta_account;
extern chratex::account const &live_account;
extern std::string const &test_genesis;
extern std::string const &beta_genesis;
extern std::string const &live_genesis;
extern std::string const &genesis_block;
extern chratex::account const &genesis_account;
extern chratex::account const &burn_account;
extern chratex::uint128_t const &genesis_amount;
// A block hash that compares inequal to any real block hash
extern chratex::block_hash const &not_a_block;
// An account number that compares inequal to any real account number
extern chratex::block_hash const &not_an_account;
/*
class genesis {
public:
	explicit genesis();
	chratex::block_hash hash() const;
	std::unique_ptr<chratex::open_block> open;
};
}
*/
}

#endif
