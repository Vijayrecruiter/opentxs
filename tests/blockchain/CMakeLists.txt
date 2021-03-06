# Copyright (c) 2010-2020 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

if(OT_BLOCKCHAIN_EXPORT)
  add_subdirectory(activity)
  add_subdirectory(headeroracle)
  add_subdirectory(startstop)
endif()

add_opentx_test(unittests-opentxs-blockchain-api Test_BlockchainAPI.cpp)

if(OT_BLOCKCHAIN_EXPORT)
  add_opentx_test(unittests-opentxs-blockchain-blockheader Test_BlockHeader.cpp)
  add_opentx_test(unittests-opentxs-blockchain-blocks-bitcoin
                  Test_BitcoinBlocks.cpp)
  add_opentx_test(unittests-opentxs-blockchain-compactsize Test_CompactSize.cpp)
  add_opentx_test(unittests-opentxs-blockchain-filters Test_Filters.cpp)
  add_opentx_test(unittests-opentxs-blockchain-hash Test_NumericHash.cpp)
  add_opentx_test(unittests-opentxs-blockchain-message Test_Message.cpp)
  add_opentx_test(unittests-opentxs-blockchain-script-bitcoin
                  Test_BitcoinScript.cpp)
  add_opentx_test(unittests-opentxs-blockchain-transaction-bitcoin
                  Test_BitcoinTransaction.cpp)
endif()
