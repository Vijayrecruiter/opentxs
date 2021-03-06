// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

// IWYU pragma: private
// IWYU pragma: friend ".*src/blockchain/p2p/bitcoin/message/Getcfilters.cpp"

#pragma once

#include <memory>

#include "internal/blockchain/p2p/bitcoin/message/Message.hpp"
#include "opentxs/Types.hpp"
#include "opentxs/blockchain/Blockchain.hpp"
#include "opentxs/core/Data.hpp"

namespace opentxs
{
namespace api
{
namespace client
{
class Manager;
}  // namespace client
}  // namespace api

namespace blockchain
{
namespace p2p
{
namespace bitcoin
{
class Header;
}  // namespace bitcoin
}  // namespace p2p
}  // namespace blockchain
}  // namespace opentxs

namespace opentxs::blockchain::p2p::bitcoin::message::implementation
{
class Getcfilters final : public internal::Getcfilters
{
public:
    using BitcoinFormat = FilterRequest;

    auto Start() const noexcept -> block::Height final { return start_; }
    auto Stop() const noexcept -> const filter::Hash& final { return stop_; }
    auto Type() const noexcept -> filter::Type final { return type_; }

    Getcfilters(
        const api::client::Manager& api,
        const blockchain::Type network,
        const filter::Type type,
        const block::Height start,
        const filter::Hash& stop) noexcept;
    Getcfilters(
        const api::client::Manager& api,
        std::unique_ptr<Header> header,
        const filter::Type type,
        const block::Height start,
        const filter::Hash& stop) noexcept;

    ~Getcfilters() final = default;

private:
    const filter::Type type_;
    const block::Height start_;
    const filter::pHash stop_;

    auto payload() const noexcept -> OTData final;

    Getcfilters(const Getcfilters&) = delete;
    Getcfilters(Getcfilters&&) = delete;
    auto operator=(const Getcfilters&) -> Getcfilters& = delete;
    auto operator=(Getcfilters &&) -> Getcfilters& = delete;
};
}  // namespace opentxs::blockchain::p2p::bitcoin::message::implementation
