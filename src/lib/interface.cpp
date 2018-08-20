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
#include <lib/interface.h>

#include <xxhash/xxhash.h>

#include <ed25519-donna/ed25519.h>

#include <blake2/blake2.h>

#include <boost/property_tree/json_parser.hpp>

#include <lib/numbers.hpp>

#include <cstring>

extern "C" {
void chrx_uint128_to_dec(chrx_uint128 source, char *destination) {
	auto const & number(*reinterpret_cast<chratex::uint128_union *> (source));
	strncpy(destination, number.to_string_dec().c_str (), 32);
}

void chrx_uint256_to_string(chrx_uint256 source, char *destination) {
	auto const & number(*reinterpret_cast<chratex::uint256_union *>(source));
	strncpy(destination, number.to_string().c_str (), 64);
}

void chrx_uint256_to_address(chrx_uint256 source, char *destination) {
	auto const & number(*reinterpret_cast<chratex::uint256_union *>(source));
	strncpy(destination, number.to_account().c_str(), 65);
}

void chrx_uint512_to_string(chrx_uint512 source, char *destination) {
	auto const &number(*reinterpret_cast<chratex::uint512_union *>(source));
	strncpy(destination, number.to_string().c_str(), 128);
}

int chrx_uint128_from_dec(const char * source, chrx_uint128 destination) {
	auto &number(*reinterpret_cast<chratex::uint128_union *> (destination));
	auto error(number.decode_dec(source));
	return error ? 1 : 0;
}

int chrx_uint256_from_string(const char * source, chrx_uint256 destination) {
	auto &number(*reinterpret_cast<chratex::uint256_union *> (destination));
	auto error(number.decode_hex(source));
	return error ? 1 : 0;
}

int chrx_uint512_from_string(const char * source, chrx_uint512 destination) {
	auto & number (*reinterpret_cast<chratex::uint512_union *> (destination));
	auto error(number.decode_hex(source));
	return error ? 1 : 0;
}

int chrx_valid_address(const char * account_a)
{
	chratex::uint256_union account;
	auto error(account.decode_account(account_a));
	return error ? 1 : 0;
}

void chrx_generate_random(chrx_uint256 seed) {
	auto &number(*reinterpret_cast<chratex::uint256_union *>(seed));
	chratex::random_pool.GenerateBlock(number.bytes.data(), number.bytes.size());
}

void chrx_seed_key(chrx_uint256 seed, int index, chrx_uint256 destination)
{
	auto &seed_l(*reinterpret_cast<chratex::uint256_union *>(seed));
	auto &destination_l(*reinterpret_cast<chratex::uint256_union *>(destination));
	chratex::deterministic_key(seed_l, index, destination_l);
}

void chrx_key_account(const chrx_uint256 key, chrx_uint256 pub) {
	ed25519_publickey(key, pub);
}

#include <ed25519-donna/ed25519-hash-custom.h>
void ed25519_randombytes_unsafe (void * out, size_t outlen) {
	chratex::random_pool.GenerateBlock(reinterpret_cast<uint8_t *>(out), outlen);
}
void ed25519_hash_init(ed25519_hash_context * ctx)
{
	ctx->blake2 = new blake2b_state;
	blake2b_init(reinterpret_cast<blake2b_state *> (ctx->blake2), 64);
}

void ed25519_hash_update(
  ed25519_hash_context * ctx, uint8_t const * in, size_t inlen
) {
	blake2b_update(reinterpret_cast<blake2b_state *> (ctx->blake2), in, inlen);
}

void ed25519_hash_final(ed25519_hash_context * ctx, uint8_t * out) {
	blake2b_final(reinterpret_cast<blake2b_state *> (ctx->blake2), out, 64);
	delete reinterpret_cast<blake2b_state *> (ctx->blake2);
}

void ed25519_hash(uint8_t * out, uint8_t const * in, size_t inlen) {
	ed25519_hash_context ctx;
	ed25519_hash_init(&ctx);
	ed25519_hash_update(&ctx, in, inlen);
	ed25519_hash_final(&ctx, out);
}
}
