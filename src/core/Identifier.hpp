// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

// IWYU pragma: private
// IWYU pragma: friend ".*src/core/Identifier.cpp"

#pragma once

#include <iosfwd>
#include <string>

#include "core/Data.hpp"
#include "opentxs/Bytes.hpp"
#include "opentxs/Proto.hpp"
#include "opentxs/Types.hpp"
#include "opentxs/core/Data.hpp"
#include "opentxs/core/Identifier.hpp"
#include "opentxs/core/identifier/Nym.hpp"
#include "opentxs/core/identifier/Server.hpp"
#include "opentxs/core/identifier/UnitDefinition.hpp"
#include "opentxs/protobuf/ContactEnums.pb.h"
#include "opentxs/protobuf/Enums.pb.h"

namespace opentxs
{
namespace identity
{
class Nym;
}  // namespace identity

namespace proto
{
class HDPath;
}  // namespace proto

class Contract;
class String;
}  // namespace opentxs

namespace opentxs::implementation
{
class Identifier final : virtual public opentxs::Identifier,
                         virtual public opentxs::identifier::Nym,
                         virtual public opentxs::identifier::Server,
                         virtual public opentxs::identifier::UnitDefinition,
                         public Data
{
public:
    using ot_super = Data;

    using ot_super::operator==;
    auto operator==(const opentxs::Identifier& rhs) const noexcept -> bool final
    {
        return Data::operator==(rhs);
    }
    using ot_super::operator!=;
    auto operator!=(const opentxs::Identifier& rhs) const noexcept -> bool final
    {
        return Data::operator!=(rhs);
    }
    using ot_super::operator>;
    auto operator>(const opentxs::Identifier& rhs) const noexcept -> bool final
    {
        return Data::operator>(rhs);
    }
    using ot_super::operator<;
    auto operator<(const opentxs::Identifier& rhs) const noexcept -> bool final
    {
        return Data::operator<(rhs);
    }
    using ot_super::operator<=;
    auto operator<=(const opentxs::Identifier& rhs) const noexcept -> bool final
    {
        return Data::operator<=(rhs);
    }
    using ot_super::operator>=;
    auto operator>=(const opentxs::Identifier& rhs) const noexcept -> bool final
    {
        return Data::operator>=(rhs);
    }

    void GetString(String& theStr) const final;
    auto str() const -> std::string final;
    auto Type() const -> const ID& final { return type_; }

    auto CalculateDigest(const ReadView bytes, const ID type = DefaultType)
        -> bool final;
    void SetString(const std::string& encoded) final;
    void SetString(const String& encoded) final;
    using ot_super::swap;
    void swap(opentxs::Identifier& rhs) final;

    ~Identifier() final = default;

private:
    friend opentxs::Identifier;
    friend opentxs::identifier::Nym;
    friend opentxs::identifier::Server;
    friend opentxs::identifier::UnitDefinition;

    static const ID DefaultType{ID::blake2b};
    static const std::size_t MinimumSize{10};

    ID type_{DefaultType};

    auto clone() const -> Identifier* final;

    static auto contract_contents_to_identifier(const Contract& in)
        -> Identifier*;
    static auto IDToHashType(const ID type) -> proto::HashType;
    static auto path_to_data(
        const proto::ContactItemType type,
        const proto::HDPath& path) -> OTData;

    explicit Identifier(const std::string& rhs);
    explicit Identifier(const String& rhs);
    explicit Identifier(const identity::Nym& nym);
    explicit Identifier(const Contract& contract);
    explicit Identifier(const Vector& data, const ID type);
    Identifier(const proto::ContactItemType type, const proto::HDPath& path);
    Identifier();
    Identifier(const Identifier& rhs) = delete;
    Identifier(Identifier&& rhs) = delete;
    auto operator=(const Identifier& rhs) -> Identifier& = delete;
    auto operator=(Identifier&& rhs) -> Identifier& = delete;
};
}  // namespace opentxs::implementation
