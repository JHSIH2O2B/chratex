#include <gtest/gtest.h>
#include <wallet/mnemonic.hpp>
#include <string>

TEST(mnemonic, initialized_with_12_word_string) {
  std::string phrase = "abandon abandon abandon abandon abandon abandon "
    "abandon abandon abandon abandon abandon about";

  const auto mn = chratex::wallet::mnemonic(phrase);

  const auto words = mn.get_words();

  const std::vector<std::string> expected = { 
    "abandon", "abandon", "abandon", "abandon", "abandon", "abandon", 
    "abandon", "abandon", "abandon", "abandon", "abandon", "about"
  };

  ASSERT_EQ(12, words.size()) << "Word list should be 12 length";

  for (int i = 0; i < words.size(); i++) {
    EXPECT_EQ(expected[i], words[i]) << "Word differs from expected at " << i;
  }

}
