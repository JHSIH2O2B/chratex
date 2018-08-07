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
#include <utility/string.hpp>
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

TEST(mnemonic, create_mnemonic_trezor) {
  for (const auto &vector : mnemonic_trezor_vectors) {
    chratex::data_chunk entropy;
    chratex::decode_base16(entropy, vector.entropy);
    const auto mnemonic = chratex::wallet::mnemonic(entropy);
    const auto words = mnemonic.get_words();

    EXPECT_TRUE(words.size() > 0);
    EXPECT_EQ(vector.mnemonic, chratex::join(words));
    EXPECT_TRUE(mnemonic.is_valid());
  }
}

TEST(mnemonic, validate_mnemonic__invalid) {
  for (const auto& vector: invalid_mnemonic_tests) {
    const auto mnemonic = chratex::wallet::mnemonic(vector);
    EXPECT_FALSE(mnemonic.is_valid());
  }
}

TEST(mnemonic, create_mnemonic__tiny) {
  const chratex::data_chunk entropy(4, 0xa9);
  const auto mnemonic = chratex::wallet::mnemonic(entropy);
  const auto words = mnemonic.get_words();

  ASSERT_EQ(words.size(), 3u);
  ASSERT_TRUE(mnemonic.is_valid());
}

TEST(mnemonic, create_mnemonic__giant) {
  const chratex::data_chunk entropy(1024, 0xa9);
  const auto mnemonic = chratex::wallet::mnemonic(entropy);
  const auto words = mnemonic.get_words();

  ASSERT_EQ(words.size(), 768u);
  ASSERT_TRUE(mnemonic.is_valid());
}

TEST(mnemonic, dictionary__en_es__no_intersection) {
  const auto& english = chratex::wallet::language::en;
  const auto& spanish = chratex::wallet::language::es;
  for (const auto es: spanish) {
    std::string test(es);
    const auto it = std::find(english.begin(), english.end(), test);
    EXPECT_EQ(it, std::end(english));
  }
}

TEST(mnemonic, dictionary__en_it__no_intersection) {
  const auto& english = chratex::wallet::language::en;
  const auto& italian = chratex::wallet::language::it;
  for (const auto it: italian) {
    std::string test(it);
    const auto iter = std::find(english.begin(), english.end(), test);
    EXPECT_EQ(iter, std::end(english));
  }
}

TEST(mnemonic, dictionary__fr_es__no_intersection) {
  const auto& french = chratex::wallet::language::fr;
  const auto& spanish = chratex::wallet::language::es;
  for (const auto es: spanish) {
    std::string test(es);
    const auto it = std::find(french.begin(), french.end(), test);
    EXPECT_EQ(it, std::end(french));
  }
}

TEST(mnemonic, dictionary__it_es__no_intersection) {
  const auto& italian = chratex::wallet::language::it;
  const auto& spanish = chratex::wallet::language::es;
  for (const auto es: spanish) {
    std::string test(es);
    const auto it = std::find(italian.begin(), italian.end(), test);
    EXPECT_EQ(it, std::end(italian));
  }
}

TEST(mnemonic, dictionary__fr_it__no_intersection) {
  const auto& french = chratex::wallet::language::fr;
  const auto& italian = chratex::wallet::language::it;
  for (const auto it: italian) {
    std::string test(it);
    const auto iter = std::find(french.begin(), french.end(), test);
    EXPECT_EQ(iter, std::end(french));
  }
}

TEST(mnemonic, dictionary__cs_ru__no_intersection) {
  const auto& czech = chratex::wallet::language::cs;
  const auto& russian = chratex::wallet::language::ru;
  for (const auto ru: russian) {
    std::string test(ru);
    const auto iter = std::find(czech.begin(), czech.end(), test);
    EXPECT_EQ(iter, std::end(czech));
  }
}

TEST(mnemonic, dictionary__cs_uk__no_intersection) {
  const auto& czech = chratex::wallet::language::cs;
  const auto& ukranian = chratex::wallet::language::uk;

  for (const auto uk: ukranian) {
    std::string test(uk);
    const auto iter = std::find(czech.begin(), czech.end(), test);
    EXPECT_EQ(std::end(czech), iter);
  }
}

TEST(mnemonic, dictionary__zh_Hans_Hant__intersection) {
  const auto& simplified = chratex::wallet::language::zh_Hans;
  const auto& traditional = chratex::wallet::language::zh_Hant;
  size_t intersection = 0;
  for (const auto hant: traditional) {
    std::string test(hant);
    const auto it = std::find(simplified.begin(), simplified.end(), test);
    if (it != std::end(simplified)) {
      intersection++;
    }
  }

  ASSERT_EQ(1275u, intersection);
}
