//
// Created by rvg77 on 23.11.18.
//
//
// Created by rvg77 on 22.11.18.
//

#include "Vector.h"

#include <iostream>
#include <algorithm>
#include <gtest/gtest.h>

TEST(CrossProduct, Simple) {
    Vector<long long> v1(1, 0), v2(0, 1);
    ASSERT_EQ(cross_product(v1, v2), 1);
    v1.y = 1;
    v2.x = 1;
  ASSERT_EQ(cross_product(v1, v2), 0);
}

TEST(Constructors, ByPoints) {
  Point<long long> a(0, 0), b(1, 1);
  Vector<long long> v(a, b);
  ASSERT_TRUE(v.x == 1 && v.y == 1);
}

int main(int argc, char** argv) {

  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
