// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

// IWYU pragma: private
// IWYU pragma: friend ".*src/blockchain/p2p/bitcoin/message/Filteradd.cpp"

#pragma once

#include <memory>

#include "internal/blockchain/p2p/bitcoin/message/Message.hpp"
#include "opentxs/Types.hpp"
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

class Factory;
}  // namespace opentxs

namespace opentxs::blockchain::p2p::bitcoin::message::implementation
{
class Filteradd final : virtual public internal::Filteradd
{
public:
    auto Element() const noexcept -> OTData final { return element_; }

    Filteradd(
        const api::client::Manager& api,
        const blockchain::Type network,
        const Data& element) noexcept;
    Filteradd(
        const api::client::Manager& api,
        std::unique_ptr<Header> header,
        const Data& element) noexcept;

    ~Filteradd() final = default;

private:
    const OTData element_;

    auto payload() const noexcept -> OTData final;

    Filteradd(const Filteradd&) = delete;
    Filteradd(Filteradd&&) = delete;
    auto operator=(const Filteradd&) -> Filteradd& = delete;
    auto operator=(Filteradd &&) -> Filteradd& = delete;
};
}  // namespace opentxs::blockchain::p2p::bitcoin::message::implementation
