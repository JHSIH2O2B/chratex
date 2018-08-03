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

#include <gtest/gtest.h>
#include <wallet/mnemonic.hpp>
#include <utility/data.hpp>
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
