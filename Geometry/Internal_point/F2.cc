//
// Created by rvg77 on 22.11.18.
//

#include "F2.h"

#include <fstream>
#include <iostream>

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
#ifdef LOCAL
  std::ifstream in("/home/rvg77/My_files/MIPT/АиСД_MIPT/3 semester/contest_2/Geometry/F2/input.txt");
  solve(in, std::cout);
#else
  solve(std::cin, std::cout);
#endif
  return 0;
}