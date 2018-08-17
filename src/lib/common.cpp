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

#include <lib/common.hpp>
#include <config.hpp>

#include <ed25519-donna/ed25519.h>

// Genesis keys for network variants
namespace
{
char const * test_private_key_data = "34F0A37AAD20F4A260F0A5B3CB3D7FB50673212263E58A380BC10474BB039CE4";
char const * test_public_key_data = "B0311EA55708D6A53C75CDBF88300259C6D018522FE3D4D0A242E431F9E8B6D0"; // chrx_3e3j5tkog48pnny9dmfzj1r16pg8t1e76dz5tmac6iq689wyjfpiij4txtdo
char const * beta_public_key_data = "A59A47CC4F593E75AE9AD653FDA9358E2F7898D9ACC8C60E80D0495CE20FBA9F"; // chrx_3betaz86ypbygpqbookmzpnmd5jhh4efmd8arr9a3n4bdmj1zgnzad7xpmfp
char const * live_public_key_data = "E89208DD038FBB269987689621D52292AE9C35941A7484756ECCED92A65093BA"; // chrx_3t6k35gi95xu6tergt6p69ck76ogmitsa8mnijtpxm9fkcm736xtoncuohr3
char const * test_genesis_data = R"%%%({
	"type": "open",
	"source": "B0311EA55708D6A53C75CDBF88300259C6D018522FE3D4D0A242E431F9E8B6D0",
	"representative": "chrx_3e3j5tkog48pnny9dmfzj1r16pg8t1e76dz5tmac6iq689wyjfpiij4txtdo",
	"account": "chrx_3e3j5tkog48pnny9dmfzj1r16pg8t1e76dz5tmac6iq689wyjfpiij4txtdo",
	"work": "9680625b39d3363d",
	"signature": "ECDA914373A2F0CA1296475BAEE40500A7F0A7AD72A5A80C81D7FAB7F6C802B2CC7DB50F5DD0FB25B2EF11761FA7344A158DD5A700B21BD47DE5BD0F63153A02"
})%%%";

char const * beta_genesis_data = R"%%%({
        "type": "open",
        "source": "A59A47CC4F593E75AE9AD653FDA9358E2F7898D9ACC8C60E80D0495CE20FBA9F",
        "representative": "chrx_3betaz86ypbygpqbookmzpnmd5jhh4efmd8arr9a3n4bdmj1zgnzad7xpmfp",
        "account": "chrx_3betaz86ypbygpqbookmzpnmd5jhh4efmd8arr9a3n4bdmj1zgnzad7xpmfp",
        "work": "000000000f0aaeeb",
        "signature": "A726490E3325E4FA59C1C900D5B6EEBB15FE13D99F49D475B93F0AACC5635929A0614CF3892764A04D1C6732A0D716FFEB254D4154C6F544D11E6630F201450B"
})%%%";

char const * live_genesis_data = R"%%%({
	"type": "open",
	"source": "E89208DD038FBB269987689621D52292AE9C35941A7484756ECCED92A65093BA",
	"representative": "chrx_3t6k35gi95xu6tergt6p69ck76ogmitsa8mnijtpxm9fkcm736xtoncuohr3",
	"account": "chrx_3t6k35gi95xu6tergt6p69ck76ogmitsa8mnijtpxm9fkcm736xtoncuohr3",
	"work": "62f05417dd3fb691",
	"signature": "9F0C933C8ADE004D808EA1985FA746A7E95BA2A38F867640F53EC8F180BDFE9E2C1268DEAD7C2664F356E37ABA362BC58E46DBA03E523A7B5A19E4B6EB12BB02"
})%%%";

class ledger_constants
{
public:
	ledger_constants () :
	zero_key ("0"),
	test_genesis_key(test_private_key_data),
	test_account(test_public_key_data),
	beta_account(beta_public_key_data),
	live_account(live_public_key_data),
	test_genesis(test_genesis_data),
	beta_genesis(beta_genesis_data),
	live_genesis(live_genesis_data),
	genesis_account (chratex::network::current == chratex::networks::test_network ? test_account : chratex::network::current == chratex::networks::beta_network ? beta_account : live_account),
	genesis_block (chratex::network::current == chratex::networks::test_network ? test_genesis : chratex::network::current == chratex::networks::beta_network ? beta_genesis : live_genesis),
	genesis_amount (std::numeric_limits<chratex::uint128_t>::max ()),
	burn_account (0)
	{
		CryptoPP::AutoSeededRandomPool random_pool;
		// Randomly generating these mean no two nodes will ever have the same sentinel values which protects against some insecure algorithms
		random_pool.GenerateBlock (not_a_block.bytes.data (), not_a_block.bytes.size ());
		random_pool.GenerateBlock (not_an_account.bytes.data (), not_an_account.bytes.size ());
	}
	chratex::keypair zero_key;
	chratex::keypair test_genesis_key;
	chratex::account test_account;
	chratex::account beta_account;
	chratex::account live_account;
	std::string test_genesis;
	std::string beta_genesis;
	std::string live_genesis;
	chratex::account genesis_account;
	std::string genesis_block;
	chratex::uint128_t genesis_amount;
	chratex::block_hash not_a_block;
	chratex::account not_an_account;
	chratex::account burn_account;
};
ledger_constants globals;
}

chratex::keypair const & chratex::zero_key (globals.zero_key);
chratex::keypair const & chratex::test_genesis_key (globals.test_genesis_key);
chratex::account const & chratex::test_account (globals.test_account);
chratex::account const & chratex::beta_account (globals.beta_account);
chratex::account const & chratex::live_account (globals.live_account);
std::string const & chratex::test_genesis (globals.test_genesis);
std::string const & chratex::beta_genesis (globals.beta_genesis);
std::string const & chratex::live_genesis (globals.live_genesis);

chratex::account const & chratex::genesis_account (globals.genesis_account);
std::string const & chratex::genesis_block (globals.genesis_block);
chratex::uint128_t const & chratex::genesis_amount (globals.genesis_amount);
chratex::block_hash const & chratex::not_a_block (globals.not_a_block);
chratex::block_hash const & chratex::not_an_account (globals.not_an_account);
chratex::account const & chratex::burn_account (globals.burn_account);

// Create a new random keypair
chratex::keypair::keypair () {
	random_pool.GenerateBlock (prv.data.bytes.data (), prv.data.bytes.size ());
	ed25519_publickey (prv.data.bytes.data (), pub.bytes.data ());
}

// Create a keypair given a private key
chratex::keypair::keypair (chratex::raw_key && prv_a) :
prv (std::move (prv_a)) {
	ed25519_publickey (prv.data.bytes.data (), pub.bytes.data ());
}

// Create a keypair given a hex string of the private key
chratex::keypair::keypair (std::string const & prv_a) {
	auto error (prv.data.decode_hex (prv_a));
	assert (!error);
	ed25519_publickey (prv.data.bytes.data (), pub.bytes.data ());
}
