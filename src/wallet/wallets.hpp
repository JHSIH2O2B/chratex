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

#ifndef CHRTEX_WALLET_WALLETS_HPP
#define CHRTEX_WALLET_WALLETS_HPP

#include <wallet/kdf.hpp>

namespace chratex {
// The wallets set is all the wallets a node controls.  A node may contain multiple wallets independently encrypted and operated.
class wallets {
public:
	wallets(bool &error, chratex::node &node);
	~wallets();
  chratex::kdf kdf;
};
}

#endif