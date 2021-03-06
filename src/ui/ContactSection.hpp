// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

// IWYU pragma: private
// IWYU pragma: friend ".*src/ui/ContactSection.cpp"

#pragma once

#include <map>
#include <set>
#include <string>
#include <utility>

#include "1_Internal.hpp"
#include "internal/ui/UI.hpp"
#include "opentxs/Pimpl.hpp"
#include "opentxs/Proto.hpp"
#include "opentxs/SharedPimpl.hpp"
#include "opentxs/Version.hpp"
#include "opentxs/core/Identifier.hpp"
#include "opentxs/protobuf/ContactEnums.pb.h"
#include "opentxs/protobuf/verify/VerifyContacts.hpp"
#include "ui/Combined.hpp"
#include "ui/List.hpp"
#include "ui/RowType.hpp"

namespace opentxs
{
namespace api
{
namespace client
{
namespace internal
{
struct Manager;
}  // namespace internal
}  // namespace client
}  // namespace api

namespace network
{
namespace zeromq
{
namespace socket
{
class Publish;
}  // namespace socket
}  // namespace zeromq
}  // namespace network

namespace ui
{
class ContactSection;
}  // namespace ui

class ContactSection;
}  // namespace opentxs

namespace opentxs::ui::implementation
{
using ContactSectionList = List<
    ContactSectionExternalInterface,
    ContactSectionInternalInterface,
    ContactSectionRowID,
    ContactSectionRowInterface,
    ContactSectionRowInternal,
    ContactSectionRowBlank,
    ContactSectionSortKey,
    ContactSectionPrimaryID>;
using ContactSectionRow =
    RowType<ContactRowInternal, ContactInternalInterface, ContactRowID>;

class ContactSection final
    : public Combined<ContactSectionList, ContactSectionRow, ContactSortKey>
{
public:
    auto ContactID() const noexcept -> std::string final
    {
        return primary_id_->str();
    }
#if OT_QT
    int FindRow(const ContactSectionRowID& id, const ContactSectionSortKey& key)
        const noexcept final
    {
        return find_row(id, key);
    }
#endif
    auto Name(const std::string& lang) const noexcept -> std::string final
    {
        return proto::TranslateSectionName(row_id_, lang);
    }
    auto Type() const noexcept -> proto::ContactSectionName final
    {
        return row_id_;
    }

    void reindex(
        const implementation::ContactSortKey& key,
        implementation::CustomData& custom) noexcept final;

    ContactSection(
        const ContactInternalInterface& parent,
        const api::client::internal::Manager& api,
        const network::zeromq::socket::Publish& publisher,
        const ContactRowID& rowID,
        const ContactSortKey& key,
        CustomData& custom
#if OT_QT
        ,
        const bool qt
#endif
        ) noexcept;
    ~ContactSection() = default;

private:
    static const std::
        map<proto::ContactSectionName, std::set<proto::ContactItemType>>
            allowed_types_;
    static const std::
        map<proto::ContactSectionName, std::map<proto::ContactItemType, int>>
            sort_keys_;

    static auto sort_key(const ContactSectionRowID type) noexcept -> int;
    static auto check_type(const ContactSectionRowID type) noexcept -> bool;

    auto construct_row(
        const ContactSectionRowID& id,
        const ContactSectionSortKey& index,
        CustomData& custom) const noexcept -> void* final;

    auto last(const ContactSectionRowID& id) const noexcept -> bool final
    {
        return ContactSectionList::last(id);
    }
    auto process_section(const opentxs::ContactSection& section) noexcept
        -> std::set<ContactSectionRowID>;
    void startup(const opentxs::ContactSection section) noexcept;

    ContactSection() = delete;
    ContactSection(const ContactSection&) = delete;
    ContactSection(ContactSection&&) = delete;
    auto operator=(const ContactSection&) -> ContactSection& = delete;
    auto operator=(ContactSection &&) -> ContactSection& = delete;
};
}  // namespace opentxs::ui::implementation

template class opentxs::SharedPimpl<opentxs::ui::ContactSection>;
