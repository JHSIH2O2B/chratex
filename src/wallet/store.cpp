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

#include <wallet/store.hpp>
#include <wallet/value.hpp>
#include <wallet/key_type.hpp>

#include <argon2.h>

#include <boost/filesystem.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

unsigned const chratex::wallet_store::version_1(1);
unsigned const chratex::wallet_store::version_current(version_1);

// Wallet version number
chratex::uint256_union const chratex::wallet_store::version_special(0);
// Random number used to salt private key encryption
chratex::uint256_union const chratex::wallet_store::salt_special(1);
// Key used to encrypt wallet keys, encrypted itself by the user password
chratex::uint256_union const chratex::wallet_store::wallet_key_special(2);
// Check value used to see if password is valid
chratex::uint256_union const chratex::wallet_store::check_special(3);
// Representative account to be used if we open a new account
chratex::uint256_union const chratex::wallet_store::representative_special(4);
// Wallet seed for deterministic key generation
chratex::uint256_union const chratex::wallet_store::seed_special(5);
// Current key index for deterministic keys
chratex::uint256_union const 
  chratex::wallet_store::deterministic_index_special(6);
int const chratex::wallet_store::special_count(7);

chratex::wallet_store::wallet_store(
  bool &init, 
  chratex::kdf &kdf, 
  database::transaction &transaction, 
  chratex::account representative, 
  unsigned fanout,
  std::string const &wallet
) :
  password(0, fanout),
  wallet_key_mem(0, fanout),
  kdf(kdf),
  environment(transaction.get_environment()) {
    
  init = false;
	initialize(transaction, init, wallet);
	if (!init) {
		int version_status;
		MDB_val version_value;
		version_status = mdb_get(
      transaction, handle, database::mdb_val(version_special), &version_value
    );

		if (version_status == MDB_NOTFOUND) {
			version_put(transaction, version_current);
			chratex::uint256_union salt;
			random_pool.GenerateBlock(salt.bytes.data(), salt.bytes.size());
			entry_put_raw(
        transaction, chratex::wallet_store::salt_special,
        chratex::wallet_value(salt, 0)
      );
			// Wallet key is a fixed random key that encrypts all entries
			chratex::raw_key wallet_key;
			random_pool.GenerateBlock(
        wallet_key.data.bytes.data(),
        sizeof(wallet_key.data.bytes)
      );
			chratex::raw_key password_l;
			password_l.data.clear();
			password.value_set(password_l);
			chratex::raw_key zero;
			zero.data.clear();

			// Wallet key is encrypted by the user's password
			chratex::uint256_union encrypted;
			encrypted.encrypt(wallet_key, zero, salt.owords[0]);
			entry_put_raw(
        transaction, chratex::wallet_store::wallet_key_special, 
        chratex::wallet_value(encrypted, 0)
      );
			chratex::raw_key wallet_key_enc;
			wallet_key_enc.data = encrypted;
			wallet_key_mem.value_set(wallet_key_enc);
			chratex::uint256_union check;
			check.encrypt(zero, wallet_key, salt.owords[0]);
			entry_put_raw(
        transaction, chratex::wallet_store::check_special,
        chratex::wallet_value(check, 0)
      );
			entry_put_raw(
        transaction, chratex::wallet_store::representative_special,
        chratex::wallet_value(representative, 0)
      );
			chratex::raw_key seed;
			random_pool.GenerateBlock(
        seed.data.bytes.data(), seed.data.bytes.size()
      );
			seed_set(transaction, seed);
			entry_put_raw(
        transaction, chratex::wallet_store::deterministic_index_special,
        chratex::wallet_value(chratex::uint256_union(0), 0)
      );
		}
	}
	chratex::raw_key key;
	key.data = entry_get_raw(
    transaction, chratex::wallet_store::wallet_key_special
  ).key;
	wallet_key_mem.value_set(key);
}

chratex::wallet_store::wallet_store(
  bool &init, 
  chratex::kdf &kdf, 
  database::transaction &transaction, 
  chratex::account representative, 
  unsigned fanout,
  std::string const &wallet,
  std::string const & json
) : 
  password(0, fanout),
  wallet_key_mem(0, fanout),
  kdf(kdf),
  environment(transaction.get_environment()) {

    init = false;

  initialize(transaction, init, wallet);

  if (!init) {
    MDB_val junk;
    assert(
      mdb_get(
        transaction, 
        handle, 
        database::mdb_val(version_special), &junk
      ) == MDB_NOTFOUND
    );

    boost::property_tree::ptree wallet_l;
    std::stringstream istream (json);
    try {
      boost::property_tree::read_json (istream, wallet_l);
    } catch (...) {
      init = true;
    }

		for (auto i(wallet_l.begin()), n(wallet_l.end()); i != n; ++i) {
		  chratex::uint256_union key;
			init = key.decode_hex(i->first);
			if (!init) {
				chratex::uint256_union value;
				init = value.decode_hex(wallet_l.get<std::string>(i->first));
				if (!init) {
					entry_put_raw(transaction, key, chratex::wallet_value(value, 0));
				} else {
					init = true;
				}
			} else {
				init = true;
			}
		}
		init |= mdb_get(
      transaction, handle, chratex::database::mdb_val(version_special), 
      &junk
    ) != 0;
		init |= mdb_get(
      transaction, handle, chratex::database::mdb_val(wallet_key_special),
      &junk
    ) != 0;
		init |= mdb_get(
      transaction, handle, chratex::database::mdb_val(salt_special), &junk
    ) != 0;
		init |= mdb_get(
      transaction, handle, chratex::database::mdb_val(check_special), &junk
    ) != 0;
		init |= mdb_get(
      transaction,
      handle,
      chratex::database::mdb_val(representative_special),
      &junk
    ) != 0;
		chratex::raw_key key;
		key.data.clear();
		password.value_set(key);
		key.data = entry_get_raw(
      transaction, chratex::wallet_store::wallet_key_special
    ).key;
		wallet_key_mem.value_set(key);
  }
}

void chratex::wallet_store::initialize(
  MDB_txn *transaction, bool &init, std::string const &path
) {
	assert(strlen(path.c_str()) == path.size());
	auto error(0);
	error |= mdb_dbi_open(transaction, path.c_str(), MDB_CREATE, &handle);
	init = error != 0;
}

chratex::uint256_union chratex::wallet_store::check(MDB_txn *transaction) {
  chratex::wallet_value value(
    entry_get_raw(transaction, chratex::wallet_store::check_special)
  );
	return value.key;
}

bool chratex::wallet_store::valid_password(MDB_txn *transaction) {
	chratex::raw_key zero;
	zero.data.clear();
	chratex::raw_key wallet_key_l;
	wallet_key(wallet_key_l, transaction);
	chratex::uint256_union check_l;
	check_l.encrypt(zero, wallet_key_l, salt(transaction).owords[0]);
	bool ok = check(transaction) == check_l;
	return ok;
}

void chratex::wallet_store::wallet_key(
  chratex::raw_key &prv, MDB_txn *transaction
) {
  std::lock_guard<std::recursive_mutex> lock(mutex);
	chratex::raw_key wallet;
	wallet_key_mem.value(wallet);
	chratex::raw_key password;
	this->password.value(password);
	prv.decrypt(wallet.data, password, salt(transaction).owords[0]);
}

void chratex::wallet_store::seed(chratex::raw_key &prv, MDB_txn *transaction) {
	chratex::wallet_value value(
    entry_get_raw(transaction, chratex::wallet_store::seed_special)
  );
	chratex::raw_key password;
	wallet_key(password, transaction);
	prv.decrypt(value.key, password, salt(transaction).owords[0]);
}

void chratex::wallet_store::seed_set(
  MDB_txn *transaction, chratex::raw_key const &prv
) {
	chratex::raw_key password;
	wallet_key(password, transaction);
	chratex::uint256_union ciphertext;
	ciphertext.encrypt(
    prv, password, salt(transaction).owords[0]
  );
	entry_put_raw(
    transaction,
    chratex::wallet_store::seed_special,
    chratex::wallet_value(ciphertext, 0)
  );
	deterministic_clear(transaction);
}

chratex::key_type chratex::wallet_store::key_type(
  chratex::wallet_value const &value
) {
	auto number(value.key.number());
	chratex::key_type result;
	auto text(number.convert_to<std::string>());
	if (number > std::numeric_limits<uint64_t>::max()) {
		result = chratex::key_type::adhoc;
	} else {
		if ((number >> 32).convert_to<uint32_t> () == 1) {
			result = chratex::key_type::deterministic;
		} else {
			result = chratex::key_type::unknown;
		}
	}
	return result;
}

void chratex::wallet_store::deterministic_key(
  chratex::raw_key & prv, MDB_txn * transaction, uint32_t index
) {
	assert(valid_password(transaction));
	chratex::raw_key seed_l;
	seed(seed_l, transaction);
	chratex::deterministic_key(seed_l.data, index, prv.data);
}

void chratex::wallet_store::deterministic_index_set(
  MDB_txn *transaction, uint32_t index
) {
	chratex::uint256_union idx(index);
	chratex::wallet_value value(idx, 0);
	entry_put_raw(
    transaction, chratex::wallet_store::deterministic_index_special, value
  );
}

void chratex::wallet_store::deterministic_clear(MDB_txn * transaction) {
	chratex::uint256_union key(0);

	for (auto i(begin(transaction)), n(end()); i != n;) {
		switch (key_type(chratex::wallet_value(i->second))) {
			case chratex::key_type::deterministic: {
				chratex::uint256_union key(i->first);
				erase(transaction, key);
				i = begin(transaction, key);
				break;
			}
			default: {
				++i;
				break;
			}
		}
	}
	deterministic_index_set(transaction, 0);
}

chratex::uint256_union chratex::wallet_store::salt(MDB_txn * transaction) {
	chratex::wallet_value value(
    entry_get_raw(transaction, chratex::wallet_store::salt_special)
  );
	return value.key;
}

void chratex::wallet_store::erase(
  MDB_txn *transaction_a, chratex::public_key const &pub
) {
	auto status(
    mdb_del(transaction_a, handle, chratex::database::mdb_val(pub), nullptr)
  );
	assert(status == 0);
}

chratex::wallet_value chratex::wallet_store::entry_get_raw(
  MDB_txn *transaction_a, chratex::public_key const &pub_a
) {
	chratex::wallet_value result;
  chratex::database::mdb_val value;
	auto status(
    mdb_get(transaction_a, handle, chratex::database::mdb_val(pub_a), value)
  );
	if (status == 0) {
		result = chratex::wallet_value(value);
	} else {
		result.key.clear();
		result.work = 0;
	}
	return result;
}

void chratex::wallet_store::entry_put_raw(
  MDB_txn *transaction,
  chratex::public_key const &pub,
  chratex::wallet_value const &entry
) {
	auto status(
    mdb_put(transaction, handle, database::mdb_val(pub), entry.val(), 0)
  );
	assert(status == 0);
}

bool chratex::wallet_store::fetch(
  MDB_txn *transaction, chratex::public_key const &pub, chratex::raw_key &prv
) {
	auto result = false;
	if (valid_password (transaction)) {
		chratex::wallet_value value (entry_get_raw (transaction, pub));
		if (!value.key.is_zero ()) {
			switch (key_type (value)) {
				case chratex::key_type::deterministic: {
					chratex::raw_key seed_l;
					seed(seed_l, transaction);
					uint32_t index(
            static_cast<uint32_t>(
              value.key.number() &static_cast<uint32_t>(-1)
            )
          );
					deterministic_key(prv, transaction, index);
					break;
				}
				case chratex::key_type::adhoc: {
					// Ad-hoc keys
					chratex::raw_key password_l;
					wallet_key(password_l, transaction);
					prv.decrypt(value.key, password_l, salt (transaction).owords[0]);
					break;
				}
				default: {
					result = true;
					break;
				}
			}
		} else {
			result = true;
		}
	} else {
		result = true;
	}
	if (!result) {
		chratex::public_key compare(chratex::pub_key(prv.data));
		if (!(pub == compare)) {
			result = true;
		}
	}
	return result;
}

bool chratex::wallet_store::exists(
  MDB_txn *transaction, chratex::public_key const &pub
) {
	return !pub.is_zero() && find(transaction, pub) != end();
}

void chratex::wallet_store::destroy(MDB_txn *transaction) {
	auto status(mdb_drop(transaction, handle, 1));
	assert(status == 0);
}

void chratex::wallet_store::version_put(
  MDB_txn *transaction, unsigned version
) {
	chratex::uint256_union entry(version);
	entry_put_raw(
    transaction, chratex::wallet_store::version_special,
    chratex::wallet_value(entry, 0)
  );
}

chratex::store_iterator<chratex::uint256_union, chratex::wallet_value> chratex::wallet_store::begin(MDB_txn *transaction_a) {
  chratex::store_iterator<chratex::uint256_union, chratex::wallet_value> result (std::make_unique<chratex::mdb_iterator<chratex::uint256_union, chratex::wallet_value>> (transaction_a, handle, chratex::database::mdb_val (chratex::uint256_union (special_count))));

  /*
	chratex::store_iterator<
    chratex::uint256_union, chratex::wallet_value
  > result(
    std::make_unique<
      chratex::mdb_iterator<chratex::uint256_union, chratex::wallet_value>
    >(
      transaction, 
      handle,
      chratex::database::mdb_val(
        chratex::uint256_union(special_count)
      )
    )
  );*/
	return result;
}

chratex::store_iterator<
  chratex::uint256_union, chratex::wallet_value
> chratex::wallet_store::begin(
  MDB_txn *transaction_a, chratex::uint256_union const & key
) {
  chratex::store_iterator<chratex::uint256_union, chratex::wallet_value> result (std::make_unique<chratex::mdb_iterator<chratex::uint256_union, chratex::wallet_value>> (transaction_a, handle, chratex::database::mdb_val (key)));
	/*chratex::store_iterator<
    chratex::uint256_union, chratex::wallet_value
  > result(
    std::make_unique<
      chratex::mdb_iterator<chratex::uint256_union, chratex::wallet_value>
    >(
      transaction, handle, database::mdb_val(key)
    )
  );*/
	return result;
}

chratex::store_iterator<chratex::uint256_union, chratex::wallet_value> chratex::wallet_store::find(
  MDB_txn *transaction, chratex::uint256_union const &key
) {
	auto result(begin(transaction, key));
	chratex::store_iterator<chratex::uint256_union, chratex::wallet_value> end(nullptr);
	if (result != end) {
		if (chratex::uint256_union(result->first) == key) {
			return result;
		} else {
			return end;
		}
	} else {
		return end;
	}
	return result;
}

chratex::store_iterator<chratex::uint256_union, chratex::wallet_value> chratex::wallet_store::end () {
	return chratex::store_iterator<chratex::uint256_union, chratex::wallet_value>(nullptr);
}
