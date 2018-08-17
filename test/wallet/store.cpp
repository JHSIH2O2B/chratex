#include <gtest/gtest.h>

#include <database/lmdb.hpp>
#include <utility/filesystem.hpp>
#include <wallet/store.hpp>

TEST(wallet, no_key) {
	bool init;
  chratex::database::mdb_env environment(
    init, chratex::filesystem::unique_path()
  );
	ASSERT_FALSE(init);
	chratex::database::transaction transaction(environment, nullptr, true);
	chratex::kdf kdf;
	chratex::wallet_store wallet(init, kdf, transaction, chratex::genesis_account, 1, "0");
	ASSERT_FALSE(init);
	chratex::keypair key1;
	chratex::raw_key prv1;
	ASSERT_TRUE(wallet.fetch(transaction, key1.pub, prv1));
	ASSERT_TRUE(wallet.valid_password(transaction));
}
