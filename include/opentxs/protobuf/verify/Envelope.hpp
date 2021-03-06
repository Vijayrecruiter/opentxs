// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef OPENTXS_PROTOBUF_ENVELOPE_HPP
#define OPENTXS_PROTOBUF_ENVELOPE_HPP

#include "opentxs/Version.hpp"  // IWYU pragma: associated

#include "opentxs/protobuf/verify/VerifyCredentials.hpp"

namespace opentxs
{
namespace proto
{
class Envelope;
}  // namespace proto
}  // namespace opentxs

namespace opentxs
{
namespace proto
{
OPENTXS_EXPORT bool CheckProto_1(const Envelope& data, const bool silent);
OPENTXS_EXPORT bool CheckProto_2(const Envelope& data, const bool silent);
OPENTXS_EXPORT bool CheckProto_3(const Envelope& data, const bool silent);
OPENTXS_EXPORT bool CheckProto_4(const Envelope& data, const bool silent);
OPENTXS_EXPORT bool CheckProto_5(const Envelope& data, const bool silent);
OPENTXS_EXPORT bool CheckProto_6(const Envelope& data, const bool silent);
OPENTXS_EXPORT bool CheckProto_7(const Envelope& data, const bool silent);
OPENTXS_EXPORT bool CheckProto_8(const Envelope& data, const bool silent);
OPENTXS_EXPORT bool CheckProto_9(const Envelope& data, const bool silent);
OPENTXS_EXPORT bool CheckProto_10(const Envelope& data, const bool silent);
OPENTXS_EXPORT bool CheckProto_11(const Envelope& data, const bool silent);
OPENTXS_EXPORT bool CheckProto_12(const Envelope& data, const bool silent);
OPENTXS_EXPORT bool CheckProto_13(const Envelope& data, const bool silent);
OPENTXS_EXPORT bool CheckProto_14(const Envelope& data, const bool silent);
OPENTXS_EXPORT bool CheckProto_15(const Envelope& data, const bool silent);
OPENTXS_EXPORT bool CheckProto_16(const Envelope& data, const bool silent);
OPENTXS_EXPORT bool CheckProto_17(const Envelope& data, const bool silent);
OPENTXS_EXPORT bool CheckProto_18(const Envelope& data, const bool silent);
OPENTXS_EXPORT bool CheckProto_19(const Envelope& data, const bool silent);
OPENTXS_EXPORT bool CheckProto_20(const Envelope& data, const bool silent);
}  // namespace proto
}  // namespace opentxs
#endif  // OPENTXS_PROTOBUF_ENVELOPE_HPP
