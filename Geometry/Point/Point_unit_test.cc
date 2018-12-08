//
// Created by rvg77 on 22.11.18.
//

#include "Point.h"

#include <iostream>
#include <algorithm>
#include <gtest/gtest.h>

TEST(Constructors, Default) {
  Point<long long> p;
  ASSERT_TRUE(p.x == 0 && p.y == 0 && p.id == 0);
}

TEST(Constructors, AllParameters) {
  Point<long long> p(1, 1, 1);
  ASSERT_TRUE(p.x == 1 && p.y == 1 && p.id == 1);
}

TEST(Comparators, Equal) {
  Point<long long> a, b;
  ASSERT_TRUE(a == b);
  b.id = -1;
  ASSERT_TRUE(a == b);
}

TEST(Comparators, NotEqual) {
  Point<long long> a, b(1, 0, 0);
  ASSERT_TRUE(a != b);
}

TEST(Comparators, Less) {
  Point<long long> a, b(0, 1, 1);
  ASSERT_TRUE(a < b);
}

TEST(Comparators, LessInSort) {
  Point<long long> a[3];
  a[0].y = 2;
  a[1].y = 0;
  a[2].y = 1;
  std::sort(a, a + 3);
  ASSERT_TRUE(a[0].y == 0 && a[1].y == 1 && a[2].y == 2);
}

int main(int argc, char** argv) {

  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
