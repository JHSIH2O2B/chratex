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

#ifndef CHRATEX_DATABASE_STORE_ITERATOR_HPP
#define CHRATEX_DATABASE_STORE_ITERATOR_HPP

#include <database/lmdb.hpp>

namespace chratex {

class store_iterator_impl {
public:
	store_iterator_impl(
    MDB_txn *transaction, 
    MDB_dbi db,
    chratex::epoch = chratex::epoch::unspecified
  );
	store_iterator_impl(
    std::nullptr_t,
    chratex::epoch = chratex::epoch::unspecified
  );
	store_iterator_impl(
    MDB_txn * transaction,
    MDB_dbi db,
    MDB_val const &val,
    chratex::epoch = chratex::epoch::unspecified
  );
	store_iterator_impl(
    chratex::store_iterator_impl &&other
  );
	store_iterator_impl(
    chratex::store_iterator_impl const &
  ) = delete;
	~store_iterator_impl();
	chratex::store_iterator_impl & operator++();
	chratex::store_iterator_impl & operator=(
    chratex::store_iterator_impl && other
  );
	chratex::store_iterator_impl & operator=(
    chratex::store_iterator_impl const &
  ) = delete;
	std::pair<database::mdb_val, database::mdb_val> *operator->();
	bool operator==(chratex::store_iterator_impl const * other_a) const;
	bool operator==(chratex::store_iterator_impl const & other_a) const;
	bool operator!=(chratex::store_iterator_impl const & other_a) const;
	void next_dup();
	void clear();
	bool is_end_sentinal() const;
	MDB_cursor *cursor;
	std::pair<database::mdb_val, database::mdb_val> current;
};

class store_iterator {
public:
	store_iterator (std::nullptr_t);
	store_iterator (std::unique_ptr<chratex::store_iterator_impl>);
	store_iterator (chratex::store_iterator &&);
	chratex::store_iterator & operator++ ();
	chratex::store_iterator & operator= (chratex::store_iterator &&);
	chratex::store_iterator & operator= (chratex::store_iterator const &) = delete;
	std::pair<database::mdb_val, database::mdb_val> * operator-> ();
	bool operator== (chratex::store_iterator const &) const;
	bool operator!= (chratex::store_iterator const &) const;

private:
	std::unique_ptr<chratex::store_iterator_impl> impl;
};

}

#endif
