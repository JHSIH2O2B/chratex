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

#include "mnemonic.hpp"
#include <sstream>
#include <string>
#include <stdexcept>
#include <assert.h>

#include <math/hash.hpp>

namespace chratex {
namespace wallet {

// BIP-39 private constants.
static constexpr size_t bits_per_word = 11;
static constexpr size_t entropy_bit_divisor = 32;
static constexpr size_t hmac_iterations = 2048;
static const auto passphrase_prefix = "mnemonic";

inline uint8_t bip39_shift(size_t bit) {
  return (1 << (byte_bits - (bit % byte_bits) - 1));
}

mnemonic::mnemonic(
  const std::string &phrase,
  const dictionary &lexicon
) : lexicon(lexicon) {

  std::istringstream f(phrase);
  std::string s;
  while (getline(f, s, ' ')) {
    words.push_back(s);
  }
}

mnemonic::mnemonic(
    const std::vector<std::string> &words,
    const dictionary &lexicon
) : words(words), lexicon(lexicon) {}

mnemonic::mnemonic(
  data_slice entropy,
  const dictionary &lexicon
) : lexicon(lexicon) {

  if ((entropy.size() % mnemonic_seed_multiple) != 0) {
    throw std::runtime_error("Incorrect entropy size");
  }

  const size_t entropy_bits = (entropy.size() * byte_bits);
  const size_t check_bits = (entropy_bits / entropy_bit_divisor);
  const size_t total_bits = (entropy_bits + check_bits);
  const size_t word_count = (total_bits / bits_per_word);

  if ((total_bits % bits_per_word) != 0) {
    throw std::runtime_error("Incorrect bits in entropy");
  }

  if ((word_count % mnemonic_word_multiple) != 0) {
    throw std::runtime_error("Incorrect word count");
  }

  const auto data = build_chunk({ entropy, sha256_hash(entropy) });

  size_t bit = 0;

  for (size_t word = 0; word < word_count; word++) {
    size_t position = 0;
    for (size_t loop = 0; loop < bits_per_word; loop++) {
      bit = (word * bits_per_word + loop);
      position <<= 1;

      const auto byte = bit / byte_bits;

      if ((data[byte] & bip39_shift(bit)) > 0) {
        position++;
      }
    }

    if (dictionary_size < position) {
      throw std::runtime_error("Incorrect position");
    }
    words.push_back(lexicon[position]);
  }


  assert(words.size() == ((bit + 1) / bits_per_word));
}

std::vector<std::string> mnemonic::get_words() const {
  return words;
}

}
}

