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

#include <lib/common.hpp>
#include <lib/numbers.hpp>

#include <boost/filesystem.hpp>
#include <liblmdb/lmdb.h>

namespace chratex {
namespace database {

  class mdb_env {
    public: 
      mdb_env(bool &, boost::filesystem::path const &, int max_dbs = 128);
      ~mdb_env();
      operator MDB_env *()const;
    private:
      MDB_env *environment;
  };

  class mdb_val {
    public:

      mdb_val(chratex::epoch = chratex::epoch::unspecified);

      mdb_val(MDB_val const &, chratex::epoch = chratex::epoch::unspecified);

      mdb_val(size_t, void *);

      mdb_val (chratex::uint128_union const &);

      mdb_val (chratex::uint256_union const &);

      void *data() const;

      size_t size() const;

      explicit operator chratex::uint128_union() const;

      explicit operator chratex::uint256_union() const;

      operator MDB_val *() const;

      operator MDB_val const &() const;

      MDB_val value;

      std::shared_ptr<std::vector<uint8_t>> buffer;

      chratex::epoch epoch;
  };

  class transaction {
    public:

      transaction(chratex::database::mdb_env &, MDB_txn *, bool);

      ~transaction();

      operator MDB_txn *() const;

      mdb_env &get_environment();

    private:

      MDB_txn *handle;

      mdb_env &environment;

  };
}
}

#endif
