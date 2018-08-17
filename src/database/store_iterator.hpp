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
	virtual ~store_iterator_impl () = default;
	virtual chratex::store_iterator_impl & operator++ () = 0;
	virtual std::pair<database::mdb_val, database::mdb_val> * operator-> () = 0;
	virtual bool operator==(chratex::store_iterator_impl const &other) const = 0;
	virtual void next_dup() = 0;
	virtual bool is_end_sentinal() const = 0;
	chratex::store_iterator_impl &operator=(
    chratex::store_iterator_impl const &
  ) = delete;
	bool operator==(chratex::store_iterator_impl const * other_a) const;
	bool operator!=(chratex::store_iterator_impl const & other_a) const;
};

class mdb_iterator : public store_iterator_impl
{
public:
	mdb_iterator (MDB_txn * transaction_a, MDB_dbi db_a, chratex::epoch = chratex::epoch::unspecified);
	mdb_iterator (std::nullptr_t, chratex::epoch = chratex::epoch::unspecified);
	mdb_iterator (MDB_txn * transaction_a, MDB_dbi db_a, MDB_val const & val_a, chratex::epoch = chratex::epoch::unspecified);
	mdb_iterator (chratex::mdb_iterator && other_a);
	mdb_iterator (chratex::mdb_iterator const &) = delete;
	~mdb_iterator ();
	chratex::store_iterator_impl & operator++ () override;
	std::pair<database::mdb_val, database::mdb_val> * operator-> () override;
	bool operator== (chratex::store_iterator_impl const & other_a) const override;
	void next_dup () override;
	bool is_end_sentinal () const override;
	void clear ();
	chratex::mdb_iterator & operator= (chratex::mdb_iterator && other_a);
	chratex::store_iterator_impl & operator= (chratex::store_iterator_impl const &) = delete;
	MDB_cursor * cursor;
	std::pair<database::mdb_val, database::mdb_val> current;
};
class mdb_merge_iterator;

class store_iterator {
  friend class chratex::mdb_merge_iterator;
public:
	store_iterator(std::nullptr_t);
	store_iterator(std::unique_ptr<chratex::store_iterator_impl>);
	store_iterator(chratex::store_iterator &&);
	chratex::store_iterator &operator++ ();
	chratex::store_iterator &operator= (chratex::store_iterator &&);
	chratex::store_iterator &operator= (chratex::store_iterator const &) = delete;
	std::pair<database::mdb_val, database::mdb_val> * operator-> ();
	bool operator== (chratex::store_iterator const &) const;
	bool operator!= (chratex::store_iterator const &) const;

private:
	std::unique_ptr<chratex::store_iterator_impl> impl;
};

}

#endif
