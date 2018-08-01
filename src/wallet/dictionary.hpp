/**
 * Copyright (c) 2011-2017 libbitcoin developers
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
#ifndef CHRATEX_WALLET_DICTIONARY_HPP
#define CHRATEX_WALLET_DICTIONARY_HPP

#include <array>
#include <vector>
#include <string>

namespace chratex {
namespace wallet {

  static constexpr size_t dictionary_size = 2048;

  typedef std::array<std::string, dictionary_size> dictionary;

  typedef std::vector<const dictionary *> dictionary_list;

  namespace language {
    extern const dictionary en;
    extern const dictionary es;
    extern const dictionary ja;
    extern const dictionary it;
    extern const dictionary fr;
    extern const dictionary cs;
    extern const dictionary ru;
    extern const dictionary uk;
    extern const dictionary zh_Hans;
		extern const dictionary zh_Hant;

    // All built-in languages:
    extern const dictionary_list all;
  }
}
}

#endif
