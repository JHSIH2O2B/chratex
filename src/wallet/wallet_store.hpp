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

#ifndef CHRATEX_WALLET_WALLET_STORE_HPP
#define CHRATEX_WALLET_WALLET_STORE_HPP

#include <database/lmdb.hpp>
#include <lib/numbers.hpp>

namespace chratex {

class wallet_store {
public:
  wallet_store(
    chratex::transaction &transaction, 
    const std::string &wallet
  );

  bool init(MDB_txn *transaction, const std::string &path);

  static chratex::uint256_union const version_special;
  
private:
  chratex::mdb_env &environment;
  MDB_dbi handle;

};

}

#endif
