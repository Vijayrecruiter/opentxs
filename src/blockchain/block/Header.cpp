// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "0_stdafx.hpp"                 // IWYU pragma: associated
#include "1_Internal.hpp"               // IWYU pragma: associated
#include "blockchain/block/Header.hpp"  // IWYU pragma: associated

#include <cstdint>
#include <memory>
#include <stdexcept>
#include <utility>

#include "internal/blockchain/Blockchain.hpp"
#include "internal/blockchain/block/Block.hpp"
#include "internal/blockchain/block/bitcoin/Bitcoin.hpp"
#include "opentxs/Pimpl.hpp"
#include "opentxs/blockchain/NumericHash.hpp"
#include "opentxs/blockchain/Work.hpp"
#include "opentxs/blockchain/block/Header.hpp"
#include "opentxs/core/Data.hpp"
#include "opentxs/core/Log.hpp"
#include "opentxs/core/LogSource.hpp"
#include "opentxs/protobuf/BlockchainBlockHeader.pb.h"
#include "opentxs/protobuf/BlockchainBlockLocalData.pb.h"

// #define OT_METHOD "opentxs::blockchain::block::Header::"

namespace opentxs::factory
{
auto GenesisBlockHeader(
    const api::client::Manager& api,
    const blockchain::Type type) noexcept
    -> std::unique_ptr<blockchain::block::Header>
{
    using ReturnType = blockchain::block::implementation::Header;

    switch (type) {
        case blockchain::Type::Bitcoin:
        case blockchain::Type::BitcoinCash:
        case blockchain::Type::Bitcoin_testnet3:
        case blockchain::Type::BitcoinCash_testnet3: {
            return factory::BitcoinBlockHeader(
                api, type, ReturnType::genesis_blocks_.at(type)->Bytes());
        }
        default: {
            LogOutput("opentxs::factory::")(__FUNCTION__)(
                ": Unsupported type (")(static_cast<std::uint32_t>(type))(")")
                .Flush();

            return nullptr;
        }
    }
}
}  // namespace opentxs::factory

namespace opentxs::blockchain::block
{
auto BlankHash() noexcept -> pHash
{
    return Data::Factory(
        "0x0000000000000000000000000000000000000000000000000000000000000000",
        Data::Mode::Hex);
}
}  // namespace opentxs::blockchain::block

namespace opentxs::blockchain::block::implementation
{
const Header::GenesisBlockMap Header::genesis_blocks_{
    {Type::Bitcoin,
     Data::Factory(
         "0x010000000000000000000000000000000000000000000000000000000000000000"
         "0000003ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e"
         "4a29ab5f49ffff001d1dac2b7c",
         Data::Mode::Hex)},
    {Type::BitcoinCash,
     Data::Factory(
         "0x010000000000000000000000000000000000000000000000000000000000000000"
         "0000003ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e"
         "4a29ab5f49ffff001d1dac2b7c",
         Data::Mode::Hex)},
    {Type::Bitcoin_testnet3,
     Data::Factory(
         "0x0100000000000000000000000000000000000000000000000000000000000000000"
         "000003ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4a"
         "dae5494dffff001d1aa4ae18",
         Data::Mode::Hex)},
    {Type::BitcoinCash_testnet3,
     Data::Factory(
         "0x0100000000000000000000000000000000000000000000000000000000000000000"
         "000003ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4a"
         "dae5494dffff001d1aa4ae18",
         Data::Mode::Hex)},
};

Header::Header(
    const api::client::Manager& api,
    const VersionNumber version,
    const blockchain::Type type,
    const block::Hash& hash,
    const block::Hash& parentHash,
    const block::Height height,
    const Status status,
    const Status inheritStatus,
    const blockchain::Work& work,
    const blockchain::Work& inheritWork) noexcept
    : api_(api)
    , hash_(hash)
    , parent_hash_(parentHash)
    , version_(version)
    , type_(type)
    , work_(work)
    , height_(height)
    , status_(status)
    , inherit_status_(inheritStatus)
    , inherit_work_(inheritWork)
{
}

Header::Header(
    const api::client::Manager& api,
    const block::Hash& hash,
    const block::Hash& parentHash,
    const SerializedType& serialized) noexcept
    : Header(
          api,
          serialized.version(),
          static_cast<blockchain::Type>(serialized.type()),
          hash,
          parentHash,
          serialized.local().height(),
          static_cast<Status>(serialized.local().status()),
          static_cast<Status>(serialized.local().inherit_status()),
          OTWork{factory::Work(serialized.local().work())},
          OTWork{factory::Work(serialized.local().inherit_work())})
{
}

Header::Header(
    const api::client::Manager& api,
    const blockchain::Type type,
    const block::Hash& hash,
    const block::Hash& parentHash,
    const block::Height height,
    const blockchain::Work& work) noexcept
    : Header(
          api,
          default_version_,
          type,
          hash,
          parentHash,
          height,
          (0 == height) ? Status::Checkpoint : Status::Normal,
          Status::Normal,
          work,
          minimum_work())
{
}

Header::Header(const Header& rhs) noexcept
    : Header(
          rhs.api_,
          rhs.default_version_,
          rhs.type_,
          rhs.hash_,
          rhs.parent_hash_,
          rhs.height_,
          rhs.status_,
          rhs.inherit_status_,
          rhs.work_,
          rhs.inherit_work_)
{
}

auto Header::EffectiveState() const noexcept -> Header::Status
{
    if (Status::CheckpointBanned == inherit_status_) { return inherit_status_; }

    if (Status::Disconnected == inherit_status_) { return inherit_status_; }

    if (Status::Checkpoint == status_) { return Status::Normal; }

    return status_;
}

void Header::CompareToCheckpoint(const block::Position& checkpoint) noexcept
{
    const auto& [height, hash] = checkpoint;

    if (height == height_) {
        if (hash == hash_) {
            status_ = Status::Checkpoint;
        } else {
            status_ = Status::CheckpointBanned;
        }
    } else {
        status_ = Header::Status::Normal;
    }
}

auto Header::Hash() const noexcept -> const block::Hash& { return hash_; }

auto Header::Height() const noexcept -> block::Height { return height_; }

auto Header::InheritedState() const noexcept -> Header::Status
{
    return inherit_status_;
}

void Header::InheritHeight(const block::Header& parent)
{
    if (parent.Hash() != parent_hash_) {
        throw std::runtime_error("Invalid parent");
    }

    height_ = parent.Height() + 1;
}

void Header::InheritState(const block::Header& parent)
{
    if (parent.Hash() != parent_hash_) {
        throw std::runtime_error("Invalid parent");
    }

    inherit_status_ = parent.EffectiveState();
}

void Header::InheritWork(const blockchain::Work& work) noexcept
{
    inherit_work_ = work;
}

auto Header::IsDisconnected() const noexcept -> bool
{
    return Status::Disconnected == EffectiveState();
}

auto Header::IsBlacklisted() const noexcept -> bool
{
    return Status::CheckpointBanned == EffectiveState();
}

auto Header::LocalState() const noexcept -> Header::Status { return status_; }

auto Header::minimum_work() -> OTWork
{
    const auto maxTarget =
        OTNumericHash{factory::NumericHashNBits(NumericHash::MaxTarget)};

    return OTWork{factory::Work(maxTarget)};
}

auto Header::NumericHash() const noexcept -> OTNumericHash
{
    return OTNumericHash{factory::NumericHash(hash_)};
}

auto Header::ParentHash() const noexcept -> const block::Hash&
{
    return parent_hash_;
}

auto Header::Position() const noexcept -> block::Position
{
    return {height_, hash_};
}

void Header::RemoveBlacklistState() noexcept
{
    status_ = Status::Normal;
    inherit_status_ = Status::Normal;
}

void Header::RemoveCheckpointState() noexcept { status_ = Status::Normal; }

auto Header::Serialize() const noexcept -> Header::SerializedType
{
    proto::BlockchainBlockHeader output{};
    output.set_version(version_);
    output.set_type(static_cast<std::uint32_t>(type_));
    auto& local = *output.mutable_local();
    local.set_version(local_data_version_);
    local.set_height(height_);
    local.set_status(static_cast<std::uint32_t>(status_));
    local.set_inherit_status(static_cast<std::uint32_t>(inherit_status_));
    local.set_work(work_->asHex());
    local.set_inherit_work(inherit_work_->asHex());

    return output;
}

void Header::SetDisconnectedState() noexcept
{
    status_ = Status::Disconnected;
    inherit_status_ = Status::Error;
}

auto Header::Valid() const noexcept -> bool { return NumericHash() < Target(); }

auto Header::Work() const noexcept -> OTWork
{
    if (parent_hash_->IsNull()) {
        return work_;
    } else {
        return work_ + inherit_work_;
    }
}
}  // namespace opentxs::blockchain::block::implementation
