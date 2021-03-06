// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

// IWYU pragma: private
// IWYU pragma: friend ".*src/blockchain/p2p/bitcoin/message/Headers.cpp"

#pragma once

#include <iosfwd>
#include <memory>
#include <vector>

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
}  // namespace opentxs

namespace opentxs::blockchain::p2p::bitcoin::message::implementation
{
class Headers final : virtual public internal::Headers
{
public:
    auto at(const std::size_t position) const noexcept(false)
        -> const value_type& final
    {
        return *payload_.at(position);
    }
    auto begin() const noexcept -> const_iterator final
    {
        return const_iterator(this, 0);
    }
    auto end() const noexcept -> const_iterator final
    {
        return const_iterator(this, payload_.size());
    }
    auto size() const noexcept -> std::size_t final { return payload_.size(); }

    Headers(
        const api::client::Manager& api,
        const blockchain::Type network,
        std::vector<std::unique_ptr<value_type>>&& headers) noexcept;
    Headers(
        const api::client::Manager& api,
        std::unique_ptr<Header> header,
        std::vector<std::unique_ptr<value_type>>&& headers) noexcept;

    ~Headers() final = default;

private:
    const std::vector<std::unique_ptr<value_type>> payload_;

    auto payload() const noexcept -> OTData final;

    Headers(const Headers&) = delete;
    Headers(Headers&&) = delete;
    auto operator=(const Headers&) -> Headers& = delete;
    auto operator=(Headers &&) -> Headers& = delete;
};
}  // namespace opentxs::blockchain::p2p::bitcoin::message::implementation
