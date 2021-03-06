// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "0_stdafx.hpp"                // IWYU pragma: associated
#include "1_Internal.hpp"              // IWYU pragma: associated
#include "blockchain/NumericHash.hpp"  // IWYU pragma: associated

#include <boost/endian/buffers.hpp>
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <vector>

#include "internal/blockchain/Blockchain.hpp"
#include "opentxs/Pimpl.hpp"
#include "opentxs/blockchain/Blockchain.hpp"
#include "opentxs/blockchain/NumericHash.hpp"
#include "opentxs/core/Data.hpp"
#include "opentxs/core/Log.hpp"
#include "opentxs/core/LogSource.hpp"
#include "opentxs/iterator/Bidirectional.hpp"

#define OT_METHOD "opentxs::blockchain::implementation::NumericHash::"

namespace be = boost::endian;

namespace opentxs::factory
{
auto NumericHashNBits(const std::int32_t input) -> blockchain::NumericHash*
{
    using ReturnType = blockchain::implementation::NumericHash;
    using ArgumentType = ReturnType::Type;
    using MantissaType = mp::checked_cpp_int;

    const be::big_int32_buf_t big{input};
    const auto nBits = Data::Factory(&big, sizeof(big));
    auto exponent{std::to_integer<std::uint8_t>(nBits->at(0))};
    MantissaType mantissa{};
    auto it = nBits->begin();
    ++it;
    mp::import_bits(mantissa, it, nBits->end(), 8, true);

    if (mantissa < 0) { return new ReturnType(); }

    ArgumentType value{};

    try {
        value = ArgumentType{mantissa << (8 * (exponent - 3))};
    } catch (...) {
        LogOutput("opentxs::factory::")(__FUNCTION__)(
            ": Failed to calculate target")
            .Flush();

        return new ReturnType();
    }

    return new ReturnType(value);
}

auto NumericHash(const blockchain::block::Hash& hash)
    -> blockchain::NumericHash*
{
    using ReturnType = blockchain::implementation::NumericHash;
    ReturnType::Type value{};

    if (hash.empty()) { return new ReturnType(); }

    try {
        // Interpret hash as little endian
        mp::import_bits(value, hash.begin(), hash.end(), 8, false);
    } catch (...) {
        LogOutput("opentxs::factory::")(__FUNCTION__)(": Failed to decode hash")
            .Flush();

        return new ReturnType();
    }

    return new ReturnType(value);
}
}  // namespace opentxs::factory

namespace opentxs
{
auto operator==(
    const OTNumericHash& lhs,
    const blockchain::NumericHash& rhs) noexcept -> bool
{
    return lhs.get() == rhs;
}

auto operator!=(
    const OTNumericHash& lhs,
    const blockchain::NumericHash& rhs) noexcept -> bool
{
    return lhs.get() != rhs;
}

auto operator<(
    const OTNumericHash& lhs,
    const blockchain::NumericHash& rhs) noexcept -> bool
{
    return lhs.get() < rhs;
}

auto operator<=(
    const OTNumericHash& lhs,
    const blockchain::NumericHash& rhs) noexcept -> bool
{
    return lhs.get() <= rhs;
}
}  // namespace opentxs

namespace opentxs::blockchain
{
const std::int32_t NumericHash::MaxTarget{486604799};  // 0x1d00ffff
}  // namespace opentxs::blockchain

namespace opentxs::blockchain::implementation
{
NumericHash::NumericHash(const Type& data) noexcept
    : blockchain::NumericHash()
    , data_(data)
{
}

NumericHash::NumericHash() noexcept
    : NumericHash(Type{})
{
}

NumericHash::NumericHash(const NumericHash& rhs) noexcept
    : NumericHash(rhs.data_)
{
}

auto NumericHash::operator==(const blockchain::NumericHash& rhs) const noexcept
    -> bool
{
    const auto& input = dynamic_cast<const NumericHash&>(rhs);

    return data_ == input.data_;
}

auto NumericHash::operator!=(const blockchain::NumericHash& rhs) const noexcept
    -> bool
{
    const auto& input = dynamic_cast<const NumericHash&>(rhs);

    return data_ != input.data_;
}

auto NumericHash::operator<(const blockchain::NumericHash& rhs) const noexcept
    -> bool
{
    const auto& input = dynamic_cast<const NumericHash&>(rhs);

    return data_ < input.data_;
}

auto NumericHash::operator<=(const blockchain::NumericHash& rhs) const noexcept
    -> bool
{
    const auto& input = dynamic_cast<const NumericHash&>(rhs);

    return data_ <= input.data_;
}

auto NumericHash::asHex(const std::size_t minimumBytes) const noexcept
    -> std::string
{
    std::vector<unsigned char> bytes;

    try {
        // Export as big endian
        mp::export_bits(data_, std::back_inserter(bytes), 8, true);
    } catch (...) {
        LogOutput(OT_METHOD)(__FUNCTION__)(": Failed to encode number").Flush();

        return {};
    }

    while (minimumBytes > bytes.size()) { bytes.insert(bytes.begin(), 0x0); }

    return opentxs::Data::Factory(bytes.data(), bytes.size())->asHex();
}
}  // namespace opentxs::blockchain::implementation
