// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef OPENTXS_IDENTITY_CREDENTIAL_VERIFICATION_HPP
#define OPENTXS_IDENTITY_CREDENTIAL_VERIFICATION_HPP

// IWYU pragma: no_include "opentxs/Proto.hpp"

#include "opentxs/Forward.hpp"  // IWYU pragma: associated

#include "opentxs/Proto.hpp"
#include "opentxs/identity/credential/Base.hpp"

namespace opentxs
{
namespace api
{
namespace internal
{
struct Core;
}  // namespace internal
}  // namespace api

namespace identity
{
namespace credential
{
class Verification : virtual public Base
{
public:
    OPENTXS_EXPORT static proto::Verification SigningForm(
        const proto::Verification& item);
    OPENTXS_EXPORT static std::string VerificationID(
        const api::internal::Core& api,
        const proto::Verification& item);

    OPENTXS_EXPORT ~Verification() override = default;

protected:
    Verification() noexcept {}  // TODO Signable

private:
    Verification(const Verification&) = delete;
    Verification(Verification&&) = delete;
    Verification& operator=(const Verification&) = delete;
    Verification& operator=(Verification&&) = delete;
};
}  // namespace credential
}  // namespace identity
}  // namespace opentxs
#endif
