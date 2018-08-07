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

#ifndef CHRATEX_WALLET_MNEMONIC_HPP
#define CHRATEX_WALLET_MNEMONIC_HPP

#include <string>

#include <wallet/dictionary.hpp>
#include <utility/data.hpp>
#include <math/hash.hpp>
#include <boost/locale.hpp>

namespace chratex {
namespace wallet {
  static constexpr size_t mnemonic_word_multiple = 3;
  static constexpr size_t mnemonic_seed_multiple = 4;

  class mnemonic {
    public:

      /**
       * Create a mnemonic from a phrase stored in a string.
       */
      mnemonic(
        const std::string &phrase,
        const dictionary &lexicon=language::en
      );

      /**
       * Create a mnemonic from a vector of words acting as each individual
       * word in a phrase.
       */
      mnemonic(
        const std::vector<std::string> &words,
        const dictionary &lexicon=language::en
      );

      /**
       * Create a mnemonic from given entropy
       */
      mnemonic(
        data_slice entropy,
        const dictionary &lexicon=language::en
      );

      /**
       * Create a mnemonic and generate the entropy interally
       */
      mnemonic(
        const dictionary &lexicon=language::en
      );

      /**
       * Checks that the mnemonic is valid in at least one of the languages.
       */
      bool is_valid(const dictionary_list &lexicons = language::all) const;

      /**
       * Checks that the mnemonic is valid in the language provided.
       */
      bool is_valid(const dictionary &lexicon) const;

      /**
       * Returns a vector of the words that compose this mnemonic phrase.
       */
      std::vector<std::string> get_words() const;

      /**
       * Returns the seed associated with this mnemonic.
       */
      long_hash to_seed() const;

    private:

      std::vector<std::string> words;

      dictionary lexicon;
  };
}
}

#endif
