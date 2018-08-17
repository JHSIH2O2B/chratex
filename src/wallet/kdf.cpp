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

#include <wallet/kdf.hpp>
#include <wallet/store.hpp>

#include <argon2.h>

void chratex::kdf::phs(
  chratex::raw_key &result,
  std::string const &password,
  chratex::uint256_union const & salt
) {
	std::lock_guard<std::mutex> lock(mutex);
	auto success(
    argon2_hash(
      1, chratex::wallet_store::kdf_work, 1, password.data(), 
      password.size(), salt.bytes.data(), salt.bytes.size(), 
      result.data.bytes.data(), result.data.bytes.size(), NULL, 0,
      Argon2_d, 0x10
    )
  );
	assert(success == 0);
	(void)success;
}

