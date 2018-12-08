//
// Created by rvg77 on 22.11.18.
//

#ifndef CONTEST_2_F2_H
#define CONTEST_2_F2_H

#include "Position.h"
#include "../Point/Point.h"
#include "Internal_point.h"

#include <fstream>
#include <vector>

template<typename CoordinateType>
struct Data {
  typedef std::vector<Point<CoordinateType>> VectorPointType;
  VectorPointType points, queries;
};

template<typename CoordinateType>
std::vector<Data<long long>> input(std::istream &in) {
  size_t test_number;
  in >> test_number;
  std::vector<Data<long long>> data(test_number);
  for (size_t test = 0; test < test_number; ++test) {
    size_t n, k;
    in >> n;
    data[test].points.resize(n);
    for (size_t i = 0; i < n; ++i)
      in >> data[test].points[i].x >> data[test].points[i].y;

    in >> k;
    data[test].queries.resize(k);
    for (size_t i = 0; i < k; ++i) {
      in >> data[test].queries[i].x >> data[test].queries[i].y;
      data[test].queries[i].id = static_cast<int>(i);
    }
  }
  return data;
}

void output(std::ostream &out, std::vector<InternalPoints<long long>::AnsType> &answers) {
  for (auto & vec: answers) {
    for (auto &answer : vec)
      out << position_to_string(answer) << '\n';
    out << '\n';
  }
}

void solve(std::istream &in, std::ostream &out) {
  auto data = input<long long>(in);
  auto test_number = data.size();

  std::vector<InternalPoints<long long>::AnsType> ans(test_number);
  for (size_t test = 0; test < test_number; ++test) {
    InternalPoints<long long> solver(data[test].points, data[test].queries);
    ans[test] = solver.get_points_positions();
  }

  output(out, ans);
}

#endif //CONTEST_2_F2_H
