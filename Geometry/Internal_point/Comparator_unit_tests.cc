//
// Created by rvg77 on 28.11.18.
//

#include "Comparator.h"
#include "../Point/Point.h"
#include "../Line/Line.h"

#include <gtest/gtest.h>

TEST(Comparator, Functor) {
  Line<long long> l(Point<long long>(0, 0), Point<long long>(1, 1));

  ASSERT_TRUE(LinePointCMP<long long>()(l, Point<long long>(3, 3)) == 0);
  ASSERT_TRUE(LinePointCMP<long long>()(l, Point<long long>(1, 0)) < 0);
  ASSERT_TRUE(LinePointCMP<long long>()(l, Point<long long>(0, 2)) > 0);
}

int main(int argc, char** argv) {

  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
