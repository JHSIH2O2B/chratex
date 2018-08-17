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

#include <liblmdb/lmdb.h>
#include <database/lmdb.hpp>
#include <boost/polymorphic_cast.hpp>
#include <config.hpp>

namespace chratex {

template <typename T, typename U>
class store_iterator_impl
{
public:
	virtual ~store_iterator_impl () = default;
	virtual chratex::store_iterator_impl<T, U> & operator++ () = 0;
	virtual bool operator== (chratex::store_iterator_impl<T, U> const & other_a) const = 0;
	virtual void next_dup () = 0;
	virtual bool is_end_sentinal () const = 0;
	virtual void fill (std::pair<T, U> &) const = 0;
	chratex::store_iterator_impl<T, U> & operator= (chratex::store_iterator_impl<T, U> const &) = delete;
	bool operator== (chratex::store_iterator_impl<T, U> const * other_a) const
	{
		return (other_a != nullptr && *this == *other_a) || (other_a == nullptr && is_end_sentinal ());
	}
	bool operator!= (chratex::store_iterator_impl<T, U> const & other_a) const
	{
		return !(*this == other_a);
	}
};

template <typename T, typename U>
class mdb_iterator : public store_iterator_impl<T, U>
{
public:
	mdb_iterator (MDB_txn * transaction_a, MDB_dbi db_a, chratex::epoch = chratex::epoch::unspecified);
	mdb_iterator (std::nullptr_t, chratex::epoch = chratex::epoch::unspecified);
	mdb_iterator (MDB_txn * transaction_a, MDB_dbi db_a, MDB_val const & val_a, chratex::epoch = chratex::epoch::unspecified);
	mdb_iterator (chratex::mdb_iterator<T, U> && other_a);
	mdb_iterator (chratex::mdb_iterator<T, U> const &) = delete;
	~mdb_iterator ();
	chratex::store_iterator_impl<T, U> & operator++ () override;
	std::pair<chratex::database::mdb_val, chratex::database::mdb_val> * operator-> ();
	bool operator== (chratex::store_iterator_impl<T, U> const & other_a) const override;
	void next_dup () override;
	bool is_end_sentinal () const override;
	void fill (std::pair<T, U> &) const override;
	void clear ();
	chratex::mdb_iterator<T, U> & operator= (chratex::mdb_iterator<T, U> && other_a);
	chratex::store_iterator_impl<T, U> & operator= (chratex::store_iterator_impl<T, U> const &) = delete;
	MDB_cursor * cursor;
	std::pair<chratex::database::mdb_val, chratex::database::mdb_val> current;
};

template <typename T, typename U>
class store_iterator
{
public:
	store_iterator (std::nullptr_t)
	{
	}
	store_iterator (std::unique_ptr<chratex::store_iterator_impl<T, U>> impl_a) :
	impl (std::move (impl_a))
	{
		impl->fill (current);
	}
	store_iterator (chratex::store_iterator<T, U> && other_a) :
	current (std::move (other_a.current)),
	impl (std::move (other_a.impl))
	{
	}
	chratex::store_iterator<T, U> & operator++ ()
	{
		++*impl;
		impl->fill (current);
		return *this;
	}
	chratex::store_iterator<T, U> & operator= (chratex::store_iterator<T, U> && other_a)
	{
		impl = std::move (other_a.impl);
		current = std::move (other_a.current);
		return *this;
	}
	chratex::store_iterator<T, U> & operator= (chratex::store_iterator<T, U> const &) = delete;
	std::pair<T, U> * operator-> ()
	{
		return &current;
	}
	bool operator== (chratex::store_iterator<T, U> const & other_a) const
	{
		return (impl == nullptr && other_a.impl == nullptr) || (impl != nullptr && *impl == other_a.impl.get ()) || (other_a.impl != nullptr && *other_a.impl == impl.get ());
	}
	bool operator!= (chratex::store_iterator<T, U> const & other_a) const
	{
		return !(*this == other_a);
	}

private:
	std::pair<T, U> current;
	std::unique_ptr<chratex::store_iterator_impl<T, U>> impl;
};

}

#endif
