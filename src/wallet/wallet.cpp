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

#include <wallet/wallet.hpp>
#include <node/node.hpp>

chratex::wallet::wallet(
  bool &init, 
  chratex::database::transaction &transaction,
  chratex::node &node,
  std::string const &wallet
) : 
  lock_observer([](bool, bool) {}),
  store (
    init, node.wallets.kdf, transaction, 
    node.config.random_representative(), 
    node.config.password_fanout, wallet
  ),
  node(node) {
}

chratex::wallet::wallet(
  bool &init, 
  chratex::database::transaction &transaction,
  chratex::node &node,
  std::string const &wallet,
  std::string const &json
) : 
  lock_observer([](bool, bool) {}),
  store(
    init, node.wallets.kdf, transaction, 
    node.config.random_representative(), 
    node.config.password_fanout, wallet, json
  ),
  node(node) {
}


