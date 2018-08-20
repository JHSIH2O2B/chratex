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

#include <database/lmdb.hpp>
#include <iostream>

chratex::mdb_env::mdb_env(
  bool &error_a, boost::filesystem::path const &path_a, int max_dbs
) {
  boost::system::error_code error;
  if (path_a.has_parent_path()) {

    std::cout << path_a << std::endl;

    boost::filesystem::create_directories(path_a.parent_path (), error);

    if (!error) {
      auto status1 = mdb_env_create(&environment);
      assert(status1 == 0);
      auto status2 = mdb_env_set_maxdbs(environment, max_dbs);
      assert(status2 == 0);
      auto status3 = mdb_env_set_mapsize(environment, 1ULL * 1024 * 1024 * 1024 * 128); // 128 Gigabyte
      assert(status3 == 0);
      // It seems if there's ever more threads than mdb_env_set_maxreaders has read slots available, we get failures on transaction creation unless MDB_NOTLS is specified
      // This can happen if something like 256 io_threads are specified in the node config
      auto status4(mdb_env_open(environment, path_a.string().c_str (), MDB_NOSUBDIR | MDB_NOTLS, 00600));
      error_a = status4 != 0;
    }

  } else {
    error_a = true;
    environment = nullptr;
  }
}

chratex::mdb_env::~mdb_env() {
	if (environment != nullptr) {
		mdb_env_close (environment);
	}
}

chratex::mdb_env::operator MDB_env *() const {
	return environment;
}

chratex::mdb_val::mdb_val (
  chratex::epoch epoch_a
) : value ({ 0, nullptr }), epoch (epoch_a) {
}

chratex::mdb_val::mdb_val(
  MDB_val const &value, chratex::epoch epoch_a
) : value(value), epoch(epoch) {
}

chratex::mdb_val::mdb_val(size_t size, void *data) : value ({ size, data }) {
}

chratex::mdb_val::mdb_val(
  chratex::uint128_union const & val
) : mdb_val(sizeof(val), const_cast<chratex::uint128_union *>(&val)) {
}

chratex::mdb_val::mdb_val(
  chratex::uint256_union const &val
) : mdb_val(sizeof(val), const_cast<chratex::uint256_union *>(&val)) {
}

void * chratex::mdb_val::data() const {
	return value.mv_data;
}

size_t chratex::mdb_val::size() const {
	return value.mv_size;
}

chratex::mdb_val::operator MDB_val *() const {
	// Allow passing a temporary to a non-c++ function which
  // doesn't have constness
	return const_cast<MDB_val *>(&value);
};

chratex::mdb_val::operator MDB_val const &() const {
	return value;
}

chratex::transaction::transaction(
  chratex::mdb_env & environment_a, MDB_txn * parent_a, bool write
) : environment(environment_a) {
	auto status(
    mdb_txn_begin(environment_a, parent_a, write ? 0 : MDB_RDONLY, &handle)
  );
	assert (status == 0);
}

chratex::transaction::~transaction () {
	auto status(mdb_txn_commit(handle));
	assert(status == 0);
}

chratex::transaction::operator MDB_txn *() const {
	return handle;
}

chratex::mdb_env &chratex::transaction::get_environment() {
  return environment;
}

chratex::mdb_val::operator chratex::uint128_union() const {
	chratex::uint128_union result;
	assert(size() == sizeof(result));
	std::copy(
    reinterpret_cast<uint8_t const *>(data()),
    reinterpret_cast<uint8_t const *>(data()) + sizeof(result),
    result.bytes.data()
  );
	return result;
}

chratex::mdb_val::operator chratex::uint256_union() const {
	chratex::uint256_union result;
	assert(size() == sizeof(result));
	std::copy(
    reinterpret_cast<uint8_t const *>(data()),
    reinterpret_cast<uint8_t const *>(data()) + sizeof(result),
    result.bytes.data()
  );
	return result;
}

