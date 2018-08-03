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

#include "mnemonic.hpp"
#include <gtest/gtest.h>

#include <wallet/mnemonic.hpp>
#include <utility/data.hpp>
#include <formats/base_16.hpp>

#include <string>

void check_words(
  const chratex::wallet::mnemonic &mnemonic, 
  const std::vector<std::string> &expected
) {
  auto words = mnemonic.get_words();
  ASSERT_EQ(expected.size(), words.size()) << "Word list should be 12 length";

  for (int i = 0; i < words.size(); i++) {
    EXPECT_EQ(expected[i], words[i]) << "Word differs from expected at " << i;
  }
}

TEST(mnemonic, decode_mnemonic_no_passphrase) {
  for (const auto &vector : mnemonic_no_passphrase) {
    const auto words = vector.mnemonic;
    auto mn = chratex::wallet::mnemonic(words, vector.language);

    ASSERT_TRUE(mn.is_valid());
    const auto seed = mn.to_seed();
    ASSERT_EQ(chratex::encode_base16(seed), vector.seed);
  }
}

/*
TEST(mnemonic, decode_mnemonic_trezor) {
  for (const auto& vector: mnemonic_trezor_vectors) {
    const auto words = split(vector.mnemonic, ",");
    auto mn = chratex::wallet::mnemonic(words, vector.language);

    ASSERT_TRUE(mn.is_valid());

    const auto seed = (words, vector.passphrase);
    ASSERT_EQ(chratex::encode_base16(seed), vector.seed);
  }
}

TEST(mnemonic__decode_mnemonic__bx) {
  for (const auto& vector: mnemonic_bx_to_seed_vectors) {
    const auto words = split(vector.mnemonic, ",");
    auto mn = chratex::wallet::mnemonic(words, vector.language);

    ASSERT_TRUE(validate_mnemonic(words));
    const auto seed = decode_mnemonic(words, vector.passphrase);
    BOOST_REQUIRE_EQUAL(chratex::encode_base16(seed), vector.seed);
  }
}
*/
TEST(mnemonic, initialized_with_12_word_string) {
    
  std::string phrase = "abandon abandon abandon abandon abandon abandon "
    "abandon abandon abandon abandon abandon about";

  const auto mn = chratex::wallet::mnemonic(phrase);

  const std::vector<std::string> expected = { 
    "abandon", "abandon", "abandon", "abandon", "abandon", "abandon", 
    "abandon", "abandon", "abandon", "abandon", "abandon", "about"
  };

  check_words(mn, expected);

  ASSERT_TRUE(mn.is_valid()) << "Phrase should be valid";
}

TEST(mnemonic, initialized_with_entropy) {

  const std::vector<uint8_t> entropy = {
    0x7f, 0x7f, 0x7f, 0x7f, 
    0x7f, 0x7f, 0x7f, 0x7f, 
    0x7f, 0x7f, 0x7f, 0x7f,
    0x7f, 0x7f, 0x7f, 0x7f
  };

  const auto slice = chratex::data_slice(entropy);

  const auto mn = chratex::wallet::mnemonic(slice);

  const std::vector<std::string> expected = {
    "legal", "winner", "thank", "year", 
    "wave", "sausage", "worth", "useful",
    "legal", "winner", "thank", "yellow"
  };

  check_words(mn, expected);

  ASSERT_TRUE(mn.is_valid()) << "Phrase should be valid";
}
