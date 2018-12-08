//
// Created by rvg77 on 22.11.18.
//

#include "Position.h"
#include <gtest/gtest.h>

TEST(Position, Convertion) {
  ASSERT_EQ(position_to_string(BORDER), "BORDER");
  ASSERT_EQ(position_to_string(INSIDE), "INSIDE");
  ASSERT_EQ(position_to_string(OUTSIDE), "OUTSIDE");
}

int main(int argc, char** argv) {

  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
