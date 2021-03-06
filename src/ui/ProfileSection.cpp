// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "0_stdafx.hpp"           // IWYU pragma: associated
#include "1_Internal.hpp"         // IWYU pragma: associated
#include "ui/ProfileSection.hpp"  // IWYU pragma: associated

#include <map>
#include <memory>
#include <set>
#include <stdexcept>
#include <thread>
#include <type_traits>
#include <utility>

#include "internal/ui/UI.hpp"
#include "opentxs/Types.hpp"
#include "opentxs/contact/ContactGroup.hpp"
#include "opentxs/contact/ContactSection.hpp"
#include "opentxs/core/Identifier.hpp"
#include "opentxs/core/Log.hpp"
#include "opentxs/protobuf/ContactEnums.pb.h"
#include "opentxs/protobuf/verify/VerifyContacts.hpp"
#include "opentxs/ui/ProfileSection.hpp"
#include "ui/Combined.hpp"
#include "ui/Widget.hpp"

template class opentxs::SharedPimpl<opentxs::ui::ProfileSection>;

//#define OT_METHOD "opentxs::ui::implementation::ProfileSection::"

namespace opentxs::factory
{
auto ProfileSectionWidget(
    const ui::implementation::ProfileInternalInterface& parent,
    const api::client::internal::Manager& api,
    const network::zeromq::socket::Publish& publisher,
    const ui::implementation::ProfileRowID& rowID,
    const ui::implementation::ProfileSortKey& key,
    ui::implementation::CustomData& custom
#if OT_QT
    ,
    const bool qt
#endif
    ) noexcept -> std::shared_ptr<ui::implementation::ProfileRowInternal>
{
    using ReturnType = ui::implementation::ProfileSection;

    return std::make_shared<ReturnType>(
        parent,
        api,
        publisher,
        rowID,
        key,
        custom
#if OT_QT
        ,
        qt
#endif
    );
}
}  // namespace opentxs::factory

namespace opentxs::ui
{
static const std::
    map<proto::ContactSectionName, std::set<proto::ContactItemType>>
        allowed_types_{
            {proto::CONTACTSECTION_COMMUNICATION,
             {
                 proto::CITEMTYPE_PHONE,
                 proto::CITEMTYPE_EMAIL,
                 proto::CITEMTYPE_SKYPE,
                 proto::CITEMTYPE_WIRE,
                 proto::CITEMTYPE_QQ,
                 proto::CITEMTYPE_BITMESSAGE,
                 proto::CITEMTYPE_WHATSAPP,
                 proto::CITEMTYPE_TELEGRAM,
                 proto::CITEMTYPE_KIK,
                 proto::CITEMTYPE_BBM,
                 proto::CITEMTYPE_WECHAT,
                 proto::CITEMTYPE_KAKAOTALK,
             }},
            {proto::CONTACTSECTION_PROFILE,
             {
                 proto::CITEMTYPE_FACEBOOK,  proto::CITEMTYPE_GOOGLE,
                 proto::CITEMTYPE_LINKEDIN,  proto::CITEMTYPE_VK,
                 proto::CITEMTYPE_ABOUTME,   proto::CITEMTYPE_ONENAME,
                 proto::CITEMTYPE_TWITTER,   proto::CITEMTYPE_MEDIUM,
                 proto::CITEMTYPE_TUMBLR,    proto::CITEMTYPE_YAHOO,
                 proto::CITEMTYPE_MYSPACE,   proto::CITEMTYPE_MEETUP,
                 proto::CITEMTYPE_REDDIT,    proto::CITEMTYPE_HACKERNEWS,
                 proto::CITEMTYPE_WIKIPEDIA, proto::CITEMTYPE_ANGELLIST,
                 proto::CITEMTYPE_GITHUB,    proto::CITEMTYPE_BITBUCKET,
                 proto::CITEMTYPE_YOUTUBE,   proto::CITEMTYPE_VIMEO,
                 proto::CITEMTYPE_TWITCH,    proto::CITEMTYPE_SNAPCHAT,
             }},
        };

static const std::
    map<proto::ContactSectionName, std::map<proto::ContactItemType, int>>
        sort_keys_{
            {proto::CONTACTSECTION_COMMUNICATION,
             {
                 {proto::CITEMTYPE_PHONE, 0},
                 {proto::CITEMTYPE_EMAIL, 1},
                 {proto::CITEMTYPE_SKYPE, 2},
                 {proto::CITEMTYPE_TELEGRAM, 3},
                 {proto::CITEMTYPE_WIRE, 4},
                 {proto::CITEMTYPE_WECHAT, 5},
                 {proto::CITEMTYPE_QQ, 6},
                 {proto::CITEMTYPE_KIK, 7},
                 {proto::CITEMTYPE_KAKAOTALK, 8},
                 {proto::CITEMTYPE_BBM, 9},
                 {proto::CITEMTYPE_WHATSAPP, 10},
                 {proto::CITEMTYPE_BITMESSAGE, 11},
             }},
            {proto::CONTACTSECTION_PROFILE,
             {
                 {proto::CITEMTYPE_FACEBOOK, 0},
                 {proto::CITEMTYPE_TWITTER, 1},
                 {proto::CITEMTYPE_REDDIT, 2},
                 {proto::CITEMTYPE_GOOGLE, 3},
                 {proto::CITEMTYPE_SNAPCHAT, 4},
                 {proto::CITEMTYPE_YOUTUBE, 5},
                 {proto::CITEMTYPE_TWITCH, 6},
                 {proto::CITEMTYPE_GITHUB, 7},
                 {proto::CITEMTYPE_LINKEDIN, 8},
                 {proto::CITEMTYPE_MEDIUM, 9},
                 {proto::CITEMTYPE_TUMBLR, 10},
                 {proto::CITEMTYPE_YAHOO, 11},
                 {proto::CITEMTYPE_MYSPACE, 12},
                 {proto::CITEMTYPE_VK, 13},
                 {proto::CITEMTYPE_MEETUP, 14},
                 {proto::CITEMTYPE_VIMEO, 15},
                 {proto::CITEMTYPE_ANGELLIST, 16},
                 {proto::CITEMTYPE_ONENAME, 17},
                 {proto::CITEMTYPE_ABOUTME, 18},
                 {proto::CITEMTYPE_BITBUCKET, 19},
                 {proto::CITEMTYPE_WIKIPEDIA, 20},
                 {proto::CITEMTYPE_HACKERNEWS, 21},
             }},
        };

auto ProfileSection::AllowedItems(
    const proto::ContactSectionName section,
    const std::string& lang) noexcept -> ProfileSection::ItemTypeList
{
    ItemTypeList output{};

    try {
        for (const auto& type : allowed_types_.at(section)) {
            output.emplace_back(type, proto::TranslateItemType(type, lang));
        }
    } catch (const std::out_of_range&) {
    }

    return output;
}
}  // namespace opentxs::ui

namespace opentxs::ui::implementation
{
ProfileSection::ProfileSection(
    const ProfileInternalInterface& parent,
    const api::client::internal::Manager& api,
    const network::zeromq::socket::Publish& publisher,
    const ProfileRowID& rowID,
    const ProfileSortKey& key,
    CustomData& custom
#if OT_QT
    ,
    const bool qt
#endif
    ) noexcept
    : Combined(
          api,
          publisher,
          parent.NymID(),
          parent.WidgetID(),
          parent,
          rowID,
          key
#if OT_QT
          ,
          qt
#endif
      )
{
    init();
    startup_.reset(new std::thread(
        &ProfileSection::startup,
        this,
        extract_custom<opentxs::ContactSection>(custom)));

    OT_ASSERT(startup_)
}

auto ProfileSection::AddClaim(
    const proto::ContactItemType type,
    const std::string& value,
    const bool primary,
    const bool active) const noexcept -> bool
{
    return parent_.AddClaim(row_id_, type, value, primary, active);
}

auto ProfileSection::check_type(const ProfileSectionRowID type) noexcept -> bool
{
    try {
        return 1 == allowed_types_.at(type.first).count(type.second);
    } catch (const std::out_of_range&) {
    }

    return false;
}

auto ProfileSection::construct_row(
    const ProfileSectionRowID& id,
    const ProfileSectionSortKey& index,
    CustomData& custom) const noexcept -> void*
{
    names_.emplace(id, index);
    const auto [it, added] = items_[index].emplace(
        id,
        factory::ProfileSubsectionWidget(
            *this,
            api_,
            publisher_,
            id,
            index,
            custom
#if OT_QT
            ,
            enable_qt_
#endif
            ));

    return it->second.get();
}

auto ProfileSection::Delete(const int type, const std::string& claimID)
    const noexcept -> bool
{
    Lock lock(lock_);
    const ProfileSectionRowID key{
        row_id_, static_cast<proto::ContactItemType>(type)};
    auto& group = find_by_id(lock, key);

    if (false == group.Valid()) { return false; }

    return group.Delete(claimID);
}

auto ProfileSection::Items(const std::string& lang) const noexcept
    -> ProfileSection::ItemTypeList
{
    return AllowedItems(row_id_, lang);
}

auto ProfileSection::Name(const std::string& lang) const noexcept -> std::string
{
    return proto::TranslateSectionName(row_id_, lang);
}

auto ProfileSection::process_section(
    const opentxs::ContactSection& section) noexcept
    -> std::set<ProfileSectionRowID>
{
    OT_ASSERT(row_id_ == section.Type())

    std::set<ProfileSectionRowID> active{};

    for (const auto& [type, group] : section) {
        OT_ASSERT(group)

        const ProfileSectionRowID key{row_id_, type};

        if (check_type(key)) {
            CustomData custom{new opentxs::ContactGroup(*group)};
            add_item(key, sort_key(key), custom);
            active.emplace(key);
        }
    }

    return active;
}

void ProfileSection::reindex(const ProfileSortKey&, CustomData& custom) noexcept
{
    delete_inactive(
        process_section(extract_custom<opentxs::ContactSection>(custom)));
}

auto ProfileSection::SetActive(
    const int type,
    const std::string& claimID,
    const bool active) const noexcept -> bool
{
    Lock lock(lock_);
    const ProfileSectionRowID key{
        row_id_, static_cast<proto::ContactItemType>(type)};
    auto& group = find_by_id(lock, key);

    if (false == group.Valid()) { return false; }

    return group.SetActive(claimID, active);
}

auto ProfileSection::SetPrimary(
    const int type,
    const std::string& claimID,
    const bool primary) const noexcept -> bool
{
    Lock lock(lock_);
    const ProfileSectionRowID key{
        row_id_, static_cast<proto::ContactItemType>(type)};
    auto& group = find_by_id(lock, key);

    if (false == group.Valid()) { return false; }

    return group.SetPrimary(claimID, primary);
}

auto ProfileSection::SetValue(
    const int type,
    const std::string& claimID,
    const std::string& value) const noexcept -> bool
{
    Lock lock(lock_);
    const ProfileSectionRowID key{
        row_id_, static_cast<proto::ContactItemType>(type)};
    auto& group = find_by_id(lock, key);

    if (false == group.Valid()) { return false; }

    return group.SetValue(claimID, value);
}

auto ProfileSection::sort_key(const ProfileSectionRowID type) noexcept -> int
{
    return sort_keys_.at(type.first).at(type.second);
}

void ProfileSection::startup(const opentxs::ContactSection section) noexcept
{
    process_section(section);
    finish_startup();
}
}  // namespace opentxs::ui::implementation
