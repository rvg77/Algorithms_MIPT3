//
// Created by rvg77 on 28.11.18.
//

#include "./Line.h"

#include <gtest/gtest.h>
#include <iostream>

TEST(Constructors, Default) {
  Line<long long> l;
  ASSERT_TRUE(l.a == 0 && l.b == 0 && l.c == 0);
}

TEST(Constructors, FromPoints) {
  Point<long long> p1(0, 0), p2(1, 1);
  Line<long long> l(p1, p2);
  //std::cout << l.a << ' ' << l.b << ' ' << l.c << '\n';
  ASSERT_TRUE(l.a == -1 && l.b == 1 && l.c == 0);
}

int main(int argc, char** argv) {

  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}