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

#ifndef CHRATEX_DATABASE_LMDB_HPP
#define CHRATEX_DATABASE_LMDB_HPP

#include <boost/filesystem.hpp>
#include <liblmdb/lmdb.h>

namespace chratex {
namespace database {

  class mbd_env {
    public: 
      mbd_env(bool &, boost::filesystem::path const &, int max_dbs = 128);
      ~mbd_env();
      operator MBD_env *()const;
      MDB_env *environment;
  };

  class transaction {
    public:
      transaction(chratex::database::mdb_env &, MDB_txn *, bool);
      ~transaction();
      operator MDB_txn *() const;
      MDB_txn *handle;
      rai::mdb_env &environment;
  };
}
}

#endif
