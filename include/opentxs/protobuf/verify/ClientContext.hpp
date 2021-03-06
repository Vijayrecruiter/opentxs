// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef OPENTXS_PROTOBUF_CLIENTCONTEXT_HPP
#define OPENTXS_PROTOBUF_CLIENTCONTEXT_HPP

#include "opentxs/Version.hpp"  // IWYU pragma: associated

#include "opentxs/protobuf/verify/VerifyConsensus.hpp"

namespace opentxs
{
namespace proto
{
class ClientContext;
}  // namespace proto
}  // namespace opentxs

namespace opentxs
{
namespace proto
{
OPENTXS_EXPORT bool CheckProto_1(
    const ClientContext& context,
    const bool silent);
OPENTXS_EXPORT bool CheckProto_2(const ClientContext&, const bool);
OPENTXS_EXPORT bool CheckProto_3(const ClientContext&, const bool);
OPENTXS_EXPORT bool CheckProto_4(const ClientContext&, const bool);
OPENTXS_EXPORT bool CheckProto_5(const ClientContext&, const bool);
OPENTXS_EXPORT bool CheckProto_6(const ClientContext&, const bool);
OPENTXS_EXPORT bool CheckProto_7(const ClientContext&, const bool);
OPENTXS_EXPORT bool CheckProto_8(const ClientContext&, const bool);
OPENTXS_EXPORT bool CheckProto_9(const ClientContext&, const bool);
OPENTXS_EXPORT bool CheckProto_10(const ClientContext&, const bool);
OPENTXS_EXPORT bool CheckProto_11(const ClientContext&, const bool);
OPENTXS_EXPORT bool CheckProto_12(const ClientContext&, const bool);
OPENTXS_EXPORT bool CheckProto_13(const ClientContext&, const bool);
OPENTXS_EXPORT bool CheckProto_14(const ClientContext&, const bool);
OPENTXS_EXPORT bool CheckProto_15(const ClientContext&, const bool);
OPENTXS_EXPORT bool CheckProto_16(const ClientContext&, const bool);
OPENTXS_EXPORT bool CheckProto_17(const ClientContext&, const bool);
OPENTXS_EXPORT bool CheckProto_18(const ClientContext&, const bool);
OPENTXS_EXPORT bool CheckProto_19(const ClientContext&, const bool);
OPENTXS_EXPORT bool CheckProto_20(const ClientContext&, const bool);
}  // namespace proto
}  // namespace opentxs

#endif  // OPENTXS_PROTOBUF_CLIENTCONTEXT_HPP
