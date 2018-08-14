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

#ifndef CHRATEX_NODE_NODE_HPP
#define CHRATEX_NODE_NODE_HPP

#include <memory>
#include <node/config.hpp>
#include <wallet/wallets.hpp>

namespace chratex {
class node: public std::enable_shared_from_this<chratex::node> {
  public:
    chratex::node_config config;
    chratex::wallets wallets;
};
}

#endif
