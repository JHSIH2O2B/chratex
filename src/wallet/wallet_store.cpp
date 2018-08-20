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

#include <wallet/wallet_store.hpp>
#include <iostream>

// Wallet version number
chratex::uint256_union const chratex::wallet_store::version_special(0);

chratex::wallet_store::wallet_store(
  chratex::transaction &transaction, 
  const std::string &wallet
) : environment(transaction.get_environment()) {
  if (init(transaction, wallet)) {
    int version_status;
    MDB_val version_value;
    version_status = mdb_get(
      transaction, handle, chratex::mdb_val(version_special), &version_value
    );

    if (version_status == MDB_NOTFOUND) {
      std::cout << "Could not find the database" << std::endl;
    }
  }
}

bool chratex::wallet_store::init(
  MDB_txn *transaction, 
  const std::string &path
) {
  assert(strlen(path.c_str()) == path.size());
  auto error(0);
  error |= mdb_dbi_open(transaction, path.c_str(), MDB_CREATE, &handle);
  return error == 0;
}
  
