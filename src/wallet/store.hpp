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

#ifndef CHRATEX_WALLET_STORE_HPP
#define CHRATEX_WALLET_STORE_HPP

#include <database/lmdb.hpp>
#include <lib/numbers.hpp>
#include <config.hpp>
#include <string>

namespace chratex {
namespace wallet {

class kdf {
public:
	void phs(
    chratex::raw_key &result,
    std::string const &password,
    chratex::uint256_union const &salt
  );
	std::mutex mutex;
};

class store {
public:
  store(
    bool & init, 
    chratex::wallet::kdf & kdf, 
    chratex::database::transaction & transaction, 
    chratex::account representative, 
    unsigned fanout,
    std::string const &wallet
  );

  store(
    bool & init, 
    chratex::wallet::kdf & kdf,
    chratex::database::transaction & transaction,
    chratex::account representative,
    unsigned fanout,
    std::string const & wallet,
    std::string const & json
  );

  static unsigned constexpr kdf_full_work = 64 * 1024;

	static unsigned constexpr kdf_test_work = 8;

	static unsigned constexpr kdf_work = chratex::network::current == 
    chratex::networks::test_network ? kdf_test_work : kdf_full_work;

private:
};

}
}

#endif
