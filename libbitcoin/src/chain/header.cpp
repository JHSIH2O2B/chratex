/**
 * Copyright (c) 2011-2017 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
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
#include <bitcoin/bitcoin/chain/header.hpp>

#include <chrono>
#include <cstddef>
#include <utility>
#include <bitcoin/bitcoin/chain/chain_state.hpp>
#include <bitcoin/bitcoin/chain/compact.hpp>
#include <bitcoin/bitcoin/constants.hpp>
#include <bitcoin/bitcoin/error.hpp>
#include <bitcoin/bitcoin/math/hash.hpp>
#include <bitcoin/bitcoin/settings.hpp>
#include <bitcoin/bitcoin/utility/container_sink.hpp>
#include <bitcoin/bitcoin/utility/container_source.hpp>
#include <bitcoin/bitcoin/utility/istream_reader.hpp>
#include <bitcoin/bitcoin/utility/ostream_writer.hpp>

namespace libbitcoin {
namespace chain {

// Use system clock because we require accurate time of day.
using wall_clock = std::chrono::system_clock;

// Constructors.
//-----------------------------------------------------------------------------

header::header(const settings& settings)
  : header(0, null_hash, null_hash, 0, 0, 0, settings)
{
}

header::header(header&& other)
  : hash_(other.hash_cache()),
    version_(other.version_),
    previous_block_hash_(std::move(other.previous_block_hash_)),
    merkle_(std::move(other.merkle_)),
    timestamp_(other.timestamp_),
    bits_(other.bits_),
    nonce_(other.nonce_),
    metadata(std::move(other.metadata)),
    settings_(other.settings_)
{
}

header::header(const header& other)
  : hash_(other.hash_cache()),
    version_(other.version_),
    previous_block_hash_(other.previous_block_hash_),
    merkle_(other.merkle_),
    timestamp_(other.timestamp_),
    bits_(other.bits_),
    nonce_(other.nonce_),
    metadata(other.metadata),
    settings_(other.settings_)
{
}

header::header(uint32_t version, hash_digest&& previous_block_hash,
    hash_digest&& merkle, uint32_t timestamp, uint32_t bits, uint32_t nonce,
    const settings& settings)
  : version_(version),
    previous_block_hash_(std::move(previous_block_hash)),
    merkle_(std::move(merkle)),
    timestamp_(timestamp),
    bits_(bits),
    nonce_(nonce),
    metadata{},
    settings_(settings)
{
}

header::header(uint32_t version, const hash_digest& previous_block_hash,
    const hash_digest& merkle, uint32_t timestamp, uint32_t bits,
    uint32_t nonce, const settings& settings)
  : version_(version),
    previous_block_hash_(previous_block_hash),
    merkle_(merkle),
    timestamp_(timestamp),
    bits_(bits),
    nonce_(nonce),
    metadata{},
    settings_(settings)
{
}

// Private cache access for copy/move construction.
header::hash_ptr header::hash_cache() const
{
    shared_lock lock(mutex_);
    return hash_;
}

// Operators.
//-----------------------------------------------------------------------------

header& header::operator=(header&& other)
{
    hash_ = other.hash_cache();
    version_ = other.version_;
    previous_block_hash_ = std::move(other.previous_block_hash_);
    merkle_ = std::move(other.merkle_);
    timestamp_ = other.timestamp_;
    bits_ = other.bits_;
    nonce_ = other.nonce_;
    metadata = std::move(other.metadata);
    return *this;
}

header& header::operator=(const header& other)
{
    hash_ = other.hash_cache();
    version_ = other.version_;
    previous_block_hash_ = other.previous_block_hash_;
    merkle_ = other.merkle_;
    timestamp_ = other.timestamp_;
    bits_ = other.bits_;
    nonce_ = other.nonce_;
    metadata = other.metadata;
    return *this;
}

bool header::operator==(const header& other) const
{
    return (version_ == other.version_)
        && (previous_block_hash_ == other.previous_block_hash_)
        && (merkle_ == other.merkle_)
        && (timestamp_ == other.timestamp_)
        && (bits_ == other.bits_)
        && (nonce_ == other.nonce_);
}

bool header::operator!=(const header& other) const
{
    return !(*this == other);
}

// Deserialization.
//-----------------------------------------------------------------------------

// static
header header::factory(const data_chunk& data, const settings& settings,
    bool wire)
{
    header instance(settings);
    instance.from_data(data, wire);
    return instance;
}

// static
header header::factory(std::istream& stream, const settings& settings,
    bool wire)
{
    header instance(settings);
    instance.from_data(stream, wire);
    return instance;
}

// static
header header::factory(reader& source, const settings& settings, bool wire)
{
    header instance(settings);
    instance.from_data(source, wire);
    return instance;
}

// static
header header::factory(reader& source, hash_digest&& hash,
    const settings& settings, bool wire)
{
    header instance(settings);
    instance.from_data(source, std::move(hash), wire);
    return instance;
}

// static
header header::factory(reader& source, const hash_digest& hash,
    const settings& settings, bool wire)
{
    header instance(settings);
    instance.from_data(source, hash, wire);
    return instance;
}

bool header::from_data(const data_chunk& data, bool wire)
{
    data_source istream(data);
    return from_data(istream, wire);
}

bool header::from_data(std::istream& stream, bool wire)
{
    istream_reader source(stream);
    return from_data(source, wire);
}

bool header::from_data(reader& source, bool)
{
    ////reset();

    version_ = source.read_4_bytes_little_endian();
    previous_block_hash_ = source.read_hash();
    merkle_ = source.read_hash();
    timestamp_ = source.read_4_bytes_little_endian();
    bits_ = source.read_4_bytes_little_endian();
    nonce_ = source.read_4_bytes_little_endian();

    if (!source)
        reset();

    return source;
}

bool header::from_data(reader& source, hash_digest&& hash, bool wire)
{
    if (!from_data(source, wire))
        return false;

    hash_ = std::make_shared<hash_digest>(std::move(hash));
    return true;
}

bool header::from_data(reader& source, const hash_digest& hash, bool wire)
{
    if (!from_data(source, wire))
        return false;

    hash_ = std::make_shared<hash_digest>(hash);
    return true;
}

// protected
void header::reset()
{
    version_ = 0;
    previous_block_hash_.fill(0);
    merkle_.fill(0);
    timestamp_ = 0;
    bits_ = 0;
    nonce_ = 0;
    invalidate_cache();
}

bool header::is_valid() const
{
    return (version_ != 0) ||
        (previous_block_hash_ != null_hash) ||
        (merkle_ != null_hash) ||
        (timestamp_ != 0) ||
        (bits_ != 0) ||
        (nonce_ != 0);
}

// Serialization.
//-----------------------------------------------------------------------------

data_chunk header::to_data(bool wire) const
{
    data_chunk data;
    const auto size = serialized_size(wire);
    data.reserve(size);
    data_sink ostream(data);
    to_data(ostream, wire);
    ostream.flush();
    BITCOIN_ASSERT(data.size() == size);
    return data;
}

void header::to_data(std::ostream& stream, bool wire) const
{
    ostream_writer sink(stream);
    to_data(sink, wire);
}

void header::to_data(writer& sink, bool) const
{
    sink.write_4_bytes_little_endian(version_);
    sink.write_hash(previous_block_hash_);
    sink.write_hash(merkle_);
    sink.write_4_bytes_little_endian(timestamp_);
    sink.write_4_bytes_little_endian(bits_);
    sink.write_4_bytes_little_endian(nonce_);
}

// Size.
//-----------------------------------------------------------------------------

// static
size_t header::satoshi_fixed_size()
{
    return sizeof(version_)
        + hash_size
        + hash_size
        + sizeof(timestamp_)
        + sizeof(bits_)
        + sizeof(nonce_);
}

size_t header::serialized_size(bool) const
{
    return satoshi_fixed_size();
}

// Accessors.
//-----------------------------------------------------------------------------

uint32_t header::version() const
{
    return version_;
}

void header::set_version(uint32_t value)
{
    version_ = value;
    invalidate_cache();
}

const hash_digest& header::previous_block_hash() const
{
    return previous_block_hash_;
}

void header::set_previous_block_hash(const hash_digest& value)
{
    previous_block_hash_ = value;
    invalidate_cache();
}

void header::set_previous_block_hash(hash_digest&& value)
{
    previous_block_hash_ = std::move(value);
    invalidate_cache();
}

const hash_digest& header::merkle() const
{
    return merkle_;
}

void header::set_merkle(const hash_digest& value)
{
    merkle_ = value;
    invalidate_cache();
}

void header::set_merkle(hash_digest&& value)
{
    merkle_ = std::move(value);
    invalidate_cache();
}

uint32_t header::timestamp() const
{
    return timestamp_;
}

void header::set_timestamp(uint32_t value)
{
    timestamp_ = value;
    invalidate_cache();
}

uint32_t header::bits() const
{
    return bits_;
}

void header::set_bits(uint32_t value)
{
    bits_ = value;
    invalidate_cache();
}

uint32_t header::nonce() const
{
    return nonce_;
}

void header::set_nonce(uint32_t value)
{
    nonce_ = value;
    invalidate_cache();
}

// Cache.
//-----------------------------------------------------------------------------

// protected
void header::invalidate_cache() const
{
    ///////////////////////////////////////////////////////////////////////////
    // Critical Section
    mutex_.lock_upgrade();

    if (hash_)
    {
        mutex_.unlock_upgrade_and_lock();
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        hash_.reset();
        //---------------------------------------------------------------------
        mutex_.unlock_and_lock_upgrade();
    }

    mutex_.unlock_upgrade();
    ///////////////////////////////////////////////////////////////////////////
}

hash_digest header::hash() const
{
    ///////////////////////////////////////////////////////////////////////////
    // Critical Section
    mutex_.lock_upgrade();

    if (!hash_)
    {
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        mutex_.unlock_upgrade_and_lock();
        hash_ = std::make_shared<hash_digest>(bitcoin_hash(to_data()));
        mutex_.unlock_and_lock_upgrade();
        //---------------------------------------------------------------------
    }

    const auto hash = *hash_;
    mutex_.unlock_upgrade();
    ///////////////////////////////////////////////////////////////////////////

    return hash;
}

// Validation helpers.
//-----------------------------------------------------------------------------

/// BUGBUG: bitcoin 32bit unix time: en.wikipedia.org/wiki/Year_2038_problem
bool header::is_valid_timestamp() const
{
    using namespace std::chrono;
    static const auto two_hours = seconds(settings_.timestamp_future_seconds);
    const auto time = wall_clock::from_time_t(timestamp_);
    const auto future = wall_clock::now() + two_hours;
    return time <= future;
}

bool header::is_valid_proof_of_work(bool retarget) const
{
    const auto bits = compact(bits_);
    const auto work_limit = retarget ? settings_.retarget_proof_of_work_limit :
        settings_.no_retarget_proof_of_work_limit;
    static const uint256_t pow_limit(compact{ work_limit });

    if (bits.is_overflowed())
        return false;

    uint256_t target(bits);

    // Ensure claimed work is within limits.
    if (target < 1 || target > pow_limit)
        return false;

    // Ensure actual work is at least claimed amount (smaller is more work).
    return to_uint256(hash()) <= target;
}

// static
uint256_t header::proof(uint32_t bits)
{
    const auto header_bits = compact(bits);

    if (header_bits.is_overflowed())
        return 0;

    uint256_t target(header_bits);

    //*************************************************************************
    // CONSENSUS: satoshi will throw division by zero in the case where the
    // target is (2^256)-1 as the overflow will result in a zero divisor.
    // While actually achieving this work is improbable, this method operates
    // on user data method and therefore must be guarded.
    //*************************************************************************
    const auto divisor = target + 1;

    // We need to compute 2**256 / (target + 1), but we can't represent 2**256
    // as it's too large for uint256. However as 2**256 is at least as large as
    // target + 1, it is equal to ((2**256 - target - 1) / (target + 1)) + 1, or
    // (~target / (target + 1)) + 1.
    return (divisor == 0) ? 0 : (~target / divisor) + 1;
}

uint256_t header::proof() const
{
    return proof(bits_);
}


// Validation.
//-----------------------------------------------------------------------------

code header::check(bool retarget) const
{
    if (!is_valid_proof_of_work(retarget))
        return error::invalid_proof_of_work;

    else if (!is_valid_timestamp())
        return error::futuristic_timestamp;

    else
        return error::success;
}

code header::accept() const
{
    const auto state = metadata.state;
    return state ? accept(*state) : error::operation_failed;
}

code header::accept(const chain_state& state) const
{
    if (bits_ != state.work_required())
        return error::incorrect_proof_of_work;

    else if (state.is_checkpoint_conflict(hash()))
        return error::checkpoints_failed;

    else if (state.is_under_checkpoint())
        return error::success;

    else if (version_ < state.minimum_block_version())
        return error::invalid_block_version;

    else if (timestamp_ <= state.median_time_past())
        return error::timestamp_too_early;

    else
        return error::success;
}

} // namespace chain
} // namespace libbitcoin