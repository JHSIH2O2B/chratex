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

using namespace chratex::database

mdb_env::mdb_env(
  bool &error_a, boost::filesystem::path const & path_a, int max_dbs
) {
  boost::system::error_code error;
  if (path_a.has_parent_path()) {

    boost::filesystem::create_directories (path_a.parent_path (), error);

    if (!error) {
      auto status1 = mdb_env_create(&environment);
      assert(status1 == 0);
      auto status2 = mdb_env_set_maxdbs(environment, max_dbs);
      assert(status2 == 0);
      auto status3 = mdb_env_set_mapsize(environment, 1ULL * 1024 * 1024 * 1024 * 128); // 128 Gigabyte
      assert(status3 == 0);
      // It seems if there's ever more threads than mdb_env_set_maxreaders has read slots available, we get failures on transaction creation unless MDB_NOTLS is specified
      // This can happen if something like 256 io_threads are specified in the node config
      auto status4(mdb_env_open(environment, path_a.string ().c_str (), MDB_NOSUBDIR | MDB_NOTLS, 00600));
      error_a = status4 != 0;
    }

  } else {
    error_a = true;
    environment = nullptr;
  }
}

mdb_env::~mdb_env(

)
