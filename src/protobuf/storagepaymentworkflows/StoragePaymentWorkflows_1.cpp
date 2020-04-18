// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "opentxs/Proto.hpp"  // IWYU pragma: associated

#include "opentxs/protobuf/Basic.hpp"
#include "opentxs/protobuf/verify/StoragePaymentWorkflows.hpp"
#include "opentxs/protobuf/verify/VerifyStorage.hpp"
#include "protobuf/Check.hpp"

#define PROTO_NAME "payment workflow storage index"

namespace opentxs::proto
{
bool CheckProto_1(const StoragePaymentWorkflows& input, const bool silent)
{
    CHECK_SUBOBJECTS(workflow, StoragePaymentWorkflowsAllowedStorageItemHash())
    CHECK_SUBOBJECTS(
        items, StoragePaymentWorkflowsAllowedStorageWorkflowIndex())
    CHECK_SUBOBJECTS(
        accounts, StoragePaymentWorkflowsAllowedStorageWorkflowIndex())
    CHECK_SUBOBJECTS(
        units, StoragePaymentWorkflowsAllowedStorageWorkflowIndex())
    CHECK_IDENTIFIERS(archived)
    CHECK_SUBOBJECTS(
        types, StoragePaymentWorkflowsAllowedStoragePaymentWorkflowType())

    if (input.workflow_size() != input.types_size()) {
        FAIL_4(
            "Wrong number of index objects. Workflows: ",
            input.workflow_size(),
            " Index objects: ",
            input.types_size())
    }

    return true;
}

bool CheckProto_2(const StoragePaymentWorkflows& input, const bool silent)
{
    return CheckProto_1(input, silent);
}

bool CheckProto_3(const StoragePaymentWorkflows& input, const bool silent)
{
    return CheckProto_1(input, silent);
}

bool CheckProto_4(const StoragePaymentWorkflows& input, const bool silent)
{
    UNDEFINED_VERSION(4)
}

bool CheckProto_5(const StoragePaymentWorkflows& input, const bool silent)
{
    UNDEFINED_VERSION(5)
}

bool CheckProto_6(const StoragePaymentWorkflows& input, const bool silent)
{
    UNDEFINED_VERSION(6)
}

bool CheckProto_7(const StoragePaymentWorkflows& input, const bool silent)
{
    UNDEFINED_VERSION(7)
}

bool CheckProto_8(const StoragePaymentWorkflows& input, const bool silent)
{
    UNDEFINED_VERSION(8)
}

bool CheckProto_9(const StoragePaymentWorkflows& input, const bool silent)
{
    UNDEFINED_VERSION(9)
}

bool CheckProto_10(const StoragePaymentWorkflows& input, const bool silent)
{
    UNDEFINED_VERSION(10)
}

bool CheckProto_11(const StoragePaymentWorkflows& input, const bool silent)
{
    UNDEFINED_VERSION(11)
}

bool CheckProto_12(const StoragePaymentWorkflows& input, const bool silent)
{
    UNDEFINED_VERSION(12)
}

bool CheckProto_13(const StoragePaymentWorkflows& input, const bool silent)
{
    UNDEFINED_VERSION(13)
}

bool CheckProto_14(const StoragePaymentWorkflows& input, const bool silent)
{
    UNDEFINED_VERSION(14)
}

bool CheckProto_15(const StoragePaymentWorkflows& input, const bool silent)
{
    UNDEFINED_VERSION(15)
}

bool CheckProto_16(const StoragePaymentWorkflows& input, const bool silent)
{
    UNDEFINED_VERSION(16)
}

bool CheckProto_17(const StoragePaymentWorkflows& input, const bool silent)
{
    UNDEFINED_VERSION(17)
}

bool CheckProto_18(const StoragePaymentWorkflows& input, const bool silent)
{
    UNDEFINED_VERSION(18)
}

bool CheckProto_19(const StoragePaymentWorkflows& input, const bool silent)
{
    UNDEFINED_VERSION(19)
}

bool CheckProto_20(const StoragePaymentWorkflows& input, const bool silent)
{
    UNDEFINED_VERSION(20)
}
}  // namespace opentxs::proto
