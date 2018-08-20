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

#ifndef CHRATEX_INTERFACE_H
#define CHRATEX_INTERFACE_H

#if __cplusplus
extern "C" {
#endif

typedef unsigned char *chrx_uint128;// 16byte array for public and private keys
typedef unsigned char *chrx_uint256; // 32byte array for public and private keys
typedef unsigned char *chrx_uint512; // 64byte array for signatures
typedef void *chrx_transaction;

// Convert amount bytes 'source' to a 39 byte not-null-terminated decimal string 'destination'
void chrx_uint128_to_dec(const chrx_uint128 source, char *destination);
// Convert public/private key bytes 'source' to a 64 byte not-null-terminated hex string 'destination'
void chrx_uint256_to_string(const chrx_uint256 source, char *destination);
// Convert public key bytes 'source' to a 65 byte non-null-terminated account string 'destination'
void chrx_uint256_to_address(chrx_uint256 source, char *destination);
// Convert public/private key bytes 'source' to a 128 byte not-null-terminated hex string 'destination'
void chrx_uint512_to_string(const chrx_uint512 source, char *destination);

// Convert 39 byte decimal string 'source' to a byte array 'destination'
// Return 0 on success, nonzero on error
int chrx_uint128_from_dec(const char *source, chrx_uint128 destination);
// Convert 64 byte hex string 'source' to a byte array 'destination'
// Return 0 on success, nonzero on error
int chrx_uint256_from_string(const char *source, chrx_uint256 destination);
// Convert 128 byte hex string 'source' to a byte array 'destination'
// Return 0 on success, nonzero on error
int chrx_uint512_from_string(const char *source, chrx_uint512 destination);

// Check if the null-terminated string 'account' is a valid chrx account number
// Return 0 on correct, nonzero on invalid
int chrx_valid_address(const char *account);

// Create a new random number in to 'destination'
void chrx_generate_random(chrx_uint256 destination);
// Retrieve the deterministic private key for 'seed' at 'index'
void chrx_seed_key(const chrx_uint256 seed, int index, chrx_uint256);
// Derive the public key 'pub' from 'key'
void chrx_key_account(chrx_uint256 key, chrx_uint256 pub);

// Sign 'transaction' using 'private_key' and write to 'signature'
char *chrx_sign_transaction(
  const char *transaction, const chrx_uint256 private_key
);
// Generate work for 'transaction'
char *chrx_work_transaction(const char *transaction);

#if __cplusplus
} // extern "C"
#endif

#endif // XRB_INTERFACE_H
