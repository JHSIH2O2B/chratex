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
#include <database/store_iterator.hpp>
#include <lib/numbers.hpp>

#include <wallet/kdf.hpp>
#include <wallet/fan.hpp>
#include <wallet/value.hpp>
#include <wallet/key_type.hpp>

#include <config.hpp>
#include <string>

namespace chratex {

class wallet_store {
public:
  wallet_store(
    bool &init, 
    chratex::kdf &kdf, 
    chratex::database::transaction &transaction, 
    chratex::account representative, 
    unsigned fanout,
    std::string const &wallet
  );

  wallet_store(
    bool & init, 
    chratex::kdf &kdf,
    chratex::database::transaction &transaction,
    chratex::account representative,
    unsigned fanout,
    std::string const &wallet,
    std::string const &json
  );

  void initialize(MDB_txn *transaction, bool &init, std::string const &path);

  chratex::uint256_union check(MDB_txn *transaction);

  bool valid_password(MDB_txn *transaction);

  void wallet_key(chratex::raw_key &, MDB_txn *);

  void seed(chratex::raw_key &, MDB_txn *);

  void seed_set(MDB_txn *, chratex::raw_key const &);

  chratex::key_type key_type(chratex::wallet_value const &);

  void deterministic_key(chratex::raw_key &, MDB_txn *, uint32_t);

  void deterministic_index_set (MDB_txn *, uint32_t);

  void deterministic_clear(MDB_txn *);

  chratex::uint256_union salt(MDB_txn *);

  chratex::wallet_value entry_get_raw(
    MDB_txn *transaction,
    chratex::public_key const &entry
  );

  void entry_put_raw(
    MDB_txn *,
    chratex::public_key const &,
    chratex::wallet_value const &
  );

  bool fetch(MDB_txn *, chratex::public_key const &, chratex::raw_key &);

  bool exists(MDB_txn *, chratex::public_key const &);

	void destroy(MDB_txn *);

  void erase(MDB_txn *, chratex::public_key const &);

  chratex::store_iterator find(MDB_txn *, chratex::uint256_union const &);

	chratex::store_iterator begin(MDB_txn *, chratex::uint256_union const &);

	chratex::store_iterator begin(MDB_txn *);

	chratex::store_iterator end();

  void version_put (MDB_txn *, unsigned);

  static unsigned constexpr kdf_full_work = 64 * 1024;

	static unsigned constexpr kdf_test_work = 8;

	static unsigned constexpr kdf_work = chratex::network::current == 
    chratex::networks::test_network ? kdf_test_work : kdf_full_work;

  chratex::fan password;

  chratex::fan wallet_key_mem;

  static unsigned const version_1;

	static unsigned const version_current;

	static chratex::uint256_union const version_special;
  
	static chratex::uint256_union const wallet_key_special;

	static chratex::uint256_union const salt_special;
  
	static chratex::uint256_union const check_special;
  
	static chratex::uint256_union const representative_special;
  
	static chratex::uint256_union const seed_special;

	static chratex::uint256_union const deterministic_index_special;

	static int const special_count;

  std::recursive_mutex mutex;

private:

  chratex::kdf &kdf;

	database::mdb_env &environment;

	MDB_dbi handle;
};

}

#endif
