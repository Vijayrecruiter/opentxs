// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <string>

#include "internal/api/Api.hpp"
#include "opentxs/Bytes.hpp"
#include "opentxs/Proto.hpp"
#include "opentxs/Types.hpp"
#include "opentxs/core/Data.hpp"
#include "opentxs/core/Secret.hpp"
#include "opentxs/crypto/key/Asymmetric.hpp"
#include "opentxs/crypto/library/AsymmetricProvider.hpp"
#include "opentxs/protobuf/Enums.pb.h"
#include "opentxs/protobuf/Signature.pb.h"

namespace opentxs
{
namespace api
{
namespace internal
{
struct Core;
}  // namespace internal
}  // namespace api

namespace crypto
{
namespace key
{
class Symmetric;
}  // namespace key
}  // namespace crypto

namespace identity
{
class Authority;
}  // namespace identity

namespace proto
{
class AsymmetricKey;
class Ciphertext;
class HDPath;
}  // namespace proto

class Identifier;
class NymParameters;
class OTSignatureMetadata;
class PasswordPrompt;
}  // namespace opentxs

namespace opentxs::crypto::key::implementation
{
class Asymmetric : virtual public key::Asymmetric
{
public:
    auto CalculateHash(
        const proto::HashType hashType,
        const PasswordPrompt& password) const noexcept -> OTData final;
    auto CalculateTag(
        const identity::Authority& nym,
        const proto::AsymmetricKeyType type,
        const PasswordPrompt& reason,
        std::uint32_t& tag,
        Secret& password) const noexcept -> bool final;
    auto CalculateTag(
        const key::Asymmetric& dhKey,
        const Identifier& credential,
        const PasswordPrompt& reason,
        std::uint32_t& tag) const noexcept -> bool final;
    auto CalculateSessionPassword(
        const key::Asymmetric& dhKey,
        const PasswordPrompt& reason,
        Secret& password) const noexcept -> bool final;
    auto CalculateID(Identifier& theOutput) const noexcept -> bool final;
    auto engine() const noexcept -> const crypto::AsymmetricProvider& final
    {
        return provider_;
    }
    auto GetMetadata() const noexcept -> const OTSignatureMetadata* final
    {
        return m_pMetadata;
    }
    auto hasCapability(const NymCapability& capability) const noexcept
        -> bool override;
    auto HasPrivate() const noexcept -> bool final { return has_private_; }
    auto HasPublic() const noexcept -> bool final { return has_public_; }
    auto keyType() const noexcept -> proto::AsymmetricKeyType final
    {
        return type_;
    }
    auto NewSignature(
        const Identifier& credentialID,
        const proto::SignatureRole role,
        const proto::HashType hash) const -> proto::Signature;
    auto Params() const noexcept -> ReadView override { return {}; }
    auto Path() const noexcept -> const std::string override;
    auto Path(proto::HDPath& output) const noexcept -> bool override;
    auto PrivateKey(const PasswordPrompt& reason) const noexcept
        -> ReadView final;
    auto PublicKey() const noexcept -> ReadView final { return key_->Bytes(); }
    auto Role() const noexcept -> proto::KeyRole final { return role_; }
    auto Serialize() const noexcept
        -> std::shared_ptr<proto::AsymmetricKey> override;
    auto SigHashType() const noexcept -> proto::HashType override
    {
        return proto::HASHTYPE_BLAKE2B256;
    }
    auto Sign(
        const GetPreimage input,
        const proto::SignatureRole role,
        proto::Signature& signature,
        const Identifier& credential,
        const PasswordPrompt& reason,
        proto::KeyRole key,
        const proto::HashType hash) const noexcept -> bool final;
    auto TransportKey(
        Data& publicKey,
        Secret& privateKey,
        const PasswordPrompt& reason) const noexcept -> bool override;
    auto Verify(const Data& plaintext, const proto::Signature& sig)
        const noexcept -> bool final;
    auto Version() const noexcept -> VersionNumber final { return version_; }

    operator bool() const noexcept override;
    auto operator==(const proto::AsymmetricKey&) const noexcept -> bool final;

    ~Asymmetric() override;

protected:
    friend OTAsymmetricKey;

    using EncryptedKey = std::unique_ptr<proto::Ciphertext>;
    using EncryptedExtractor = std::function<EncryptedKey(Data&, Secret&)>;

    const api::internal::Core& api_;
    const crypto::AsymmetricProvider& provider_;
    const VersionNumber version_;
    const proto::AsymmetricKeyType type_;
    const proto::KeyRole role_;
    bool has_public_;
    bool has_private_;
    OTSignatureMetadata* m_pMetadata;
    const OTData key_;
    mutable OTSecret plaintext_key_;
    std::unique_ptr<proto::Ciphertext> encrypted_key_;

    static auto create_key(
        const api::internal::Core& api,
        const crypto::AsymmetricProvider& provider,
        const NymParameters& options,
        const proto::KeyRole role,
        const AllocateOutput publicKey,
        const AllocateOutput privateKey,
        const Secret& prv,
        const AllocateOutput params,
        const PasswordPrompt& reason) noexcept(false)
        -> std::unique_ptr<proto::Ciphertext>;
    static auto encrypt_key(
        key::Symmetric& sessionKey,
        const PasswordPrompt& reason,
        const bool attach,
        const ReadView plaintext) noexcept
        -> std::unique_ptr<proto::Ciphertext>;
    static auto encrypt_key(
        const api::internal::Core& api,
        const PasswordPrompt& reason,
        const ReadView plaintext,
        proto::Ciphertext& ciphertext) noexcept -> bool;
    static auto encrypt_key(
        key::Symmetric& sessionKey,
        const PasswordPrompt& reason,
        const bool attach,
        const ReadView plaintext,
        proto::Ciphertext& ciphertext) noexcept -> bool;
    static auto generate_key(
        const crypto::AsymmetricProvider& provider,
        const NymParameters& options,
        const proto::KeyRole role,
        const AllocateOutput publicKey,
        const AllocateOutput privateKey,
        const AllocateOutput params) noexcept(false) -> void;

    auto get_password(
        const key::Asymmetric& target,
        const PasswordPrompt& reason,
        Secret& password) const noexcept -> bool;
    auto get_tag(
        const key::Asymmetric& target,
        const Identifier& credential,
        const PasswordPrompt& reason,
        std::uint32_t& tag) const noexcept -> bool;

    virtual void erase_private_data();

    Asymmetric(
        const api::internal::Core& api,
        const crypto::AsymmetricProvider& engine,
        const proto::AsymmetricKeyType keyType,
        const proto::KeyRole role,
        const bool hasPublic,
        const bool hasPrivate,
        const VersionNumber version,
        OTData&& pubkey,
        EncryptedExtractor) noexcept(false);
    Asymmetric(
        const api::internal::Core& api,
        const crypto::AsymmetricProvider& engine,
        const proto::AsymmetricKeyType keyType,
        const proto::KeyRole role,
        const VersionNumber version,
        EncryptedExtractor) noexcept(false);
    Asymmetric(
        const api::internal::Core& api,
        const crypto::AsymmetricProvider& engine,
        const proto::AsymmetricKey& serializedKey,
        EncryptedExtractor) noexcept(false);
    Asymmetric(const Asymmetric& rhs) noexcept;

private:
    static const std::map<proto::SignatureRole, VersionNumber> sig_version_;

    auto SerializeKeyToData(const proto::AsymmetricKey& rhs) const -> OTData;

    Asymmetric() = delete;
    Asymmetric(Asymmetric&&) = delete;
    auto operator=(const Asymmetric&) -> Asymmetric& = delete;
    auto operator=(Asymmetric &&) -> Asymmetric& = delete;
};
}  // namespace opentxs::crypto::key::implementation
