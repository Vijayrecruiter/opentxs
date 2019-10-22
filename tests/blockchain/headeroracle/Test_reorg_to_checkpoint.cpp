// Copyright (c) 2010-2019 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "Helpers.hpp"

TEST_F(Test_HeaderOracle, reorg_to_checkpoint)
{
    EXPECT_TRUE(create_blocks(create_7_));
    EXPECT_TRUE(apply_blocks(sequence_7_));
    EXPECT_TRUE(verify_post_state(post_state_2_));
    EXPECT_TRUE(verify_best_chain(best_chain_2_));
    EXPECT_TRUE(verify_siblings(siblings_2_));

    EXPECT_TRUE(header_oracle_.AddCheckpoint(3, get_block_hash(BLOCK_4)));

    const auto [height, hash] = header_oracle_.GetCheckpoint();

    EXPECT_EQ(height, 3);
    EXPECT_EQ(hash, get_block_hash(BLOCK_4));
    EXPECT_TRUE(verify_post_state(post_state_7_));
    EXPECT_TRUE(verify_best_chain(best_chain_7_));
    EXPECT_TRUE(verify_siblings(siblings_7_));
}
