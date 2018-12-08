//
// Created by rvg77 on 22.11.18.
//

#ifndef CONTEST_2_INTERNAL_POINT_H
#define CONTEST_2_INTERNAL_POINT_H

#include "Position.h"
#include "../Point/Point.h"
#include "../Vector/Vector.h"
#include "../Line/Line.h"
#include "Abstract_Treap.h"
#include "Comparator.h"

#include <algorithm>
#include <vector>

template<typename CoordinateType>
class InternalPoints {
 public:
  typedef Treap<Line<CoordinateType>, Point<CoordinateType>, CompareLinePoint<CoordinateType>> TreapType;
  typedef Point<CoordinateType> PointType;
  typedef Vector<CoordinateType> VectorType;
  typedef std::vector<EPositionType> AnsType;
  typedef std::vector<Point<CoordinateType>> PointsVectorType;
 private:
  size_t n, k; // number of points and queries
  PointsVectorType points, queries, unsorted_points;

 private:
  void sort_data();
  size_t real_index(int index) const;
  bool one_segment(size_t a, size_t b) const;
  PointType get_neighbour(size_t pos, bool next) const;
  void count_answers(TreapType *root, size_t &i, size_t &cur_id, AnsType &ans) const;
  void process_vertex(TreapType *&root, size_t i);
  AnsType process_events();
 public:
  InternalPoints() : n(), k(), points(), queries(), unsorted_points() {};
  InternalPoints(const PointsVectorType &points, const PointsVectorType &queries)
      : n(points.size()), k(queries.size()), points(points), queries(queries) {};

  void delete_duplicates();
  AnsType get_points_positions();
};

template<typename CoordinateType>
void InternalPoints<CoordinateType>::sort_data() {
  std::sort(points.begin(), points.end(), compareXY<CoordinateType>);
  std::sort(queries.begin(), queries.end(), compareXY<CoordinateType>);
}

template<typename CoordinateType>
size_t InternalPoints<CoordinateType>::real_index(int index) const {
  auto ans = static_cast<size_t>(index + n);
  while (ans >= n)
    ans -= n;
  return ans;
}

template<typename CoordinateType>
void InternalPoints<CoordinateType>::delete_duplicates() {
  PointsVectorType raw = points;
  points.clear();
  for (size_t i = 0; i < n; ++i) {
    raw[i].id = 0;
    if (i == 0 || !(raw[i - 1] == raw[i])) {
      points.push_back(raw[i]);
      unsorted_points.push_back(raw[i]);
    }
  }
  n = points.size();
  for (size_t i = 0; i < n; ++i)
    points[i].id = unsorted_points[i].id = static_cast<int>(i);
}

template<typename CoordinateType>
bool InternalPoints<CoordinateType>::one_segment(size_t a, size_t b) const {
  return (abs(points[a].id - points[b].id) == 1 || abs(points[a].id - points[b].id) == static_cast<int>(n - 1));
}

template<typename CoordinateType>
Point<CoordinateType> InternalPoints<CoordinateType>::get_neighbour(size_t pos, bool next) const {
  int offset = ((next) ? 1 : -1);
  return unsorted_points[real_index(points[pos].id + offset)];
}

template<typename CoordinateType>
void InternalPoints<CoordinateType>::count_answers(TreapType *root, size_t &i,
                                                   size_t &cur_id,
                                                   InternalPoints<CoordinateType>::AnsType &ans) const {
  while (cur_id < k && compareXY(queries[cur_id], points[i])) {
    ans[queries[cur_id].id] = TreapType::find(root, queries[cur_id], CompareLinePoint<CoordinateType>());
    if (i && InternalPoints<CoordinateType>::one_segment(i, i - 1) && points[i].x == points[i - 1].x) // vertical case
      ans[queries[cur_id].id] = BORDER;
    ++cur_id;
  }
  while (cur_id < k && queries[cur_id] == points[i])
    ans[queries[cur_id++].id] = BORDER;
}

template<typename CoordinateType>
void InternalPoints<CoordinateType>::process_vertex(InternalPoints<CoordinateType>::TreapType *&root, size_t i) {
  TreapType *left = nullptr, *right = nullptr;
  auto cmp = CompareLinePoint<CoordinateType>();
  TreapType::split(root, points[i], left, right, cmp);
  root = left;

  PointType neighbours[2];
  neighbours[0] = get_neighbour(i, false);
  neighbours[1] = get_neighbour(i, true);

  VectorType v1(points[i], neighbours[0]);
  VectorType v2(points[i], neighbours[1]);
  if (cross_product(v1, v2) < 0)
    std::swap(neighbours[0], neighbours[1]);

  for (auto &neighbour : neighbours) {
    if (neighbour.x > points[i].x) {
      Line<CoordinateType> l(points[i], neighbour);
      root = TreapType::merge(root, new TreapType(l));
    }
  }
  root = TreapType::merge(root, right);
}

template<typename CoordinateType>
typename InternalPoints<CoordinateType>::AnsType InternalPoints<CoordinateType>::process_events() {
  AnsType ans(k);
  TreapType *root = nullptr;
  size_t cur_id = 0;
  for (size_t i = 0; i < n; ++i) {
    InternalPoints<CoordinateType>::count_answers(root, i, cur_id, ans);
    InternalPoints<CoordinateType>::process_vertex(root, i);
  }
  delete root;
  return ans;
}

template<typename CoordinateType>
typename InternalPoints<CoordinateType>::AnsType InternalPoints<CoordinateType>::get_points_positions() {
  delete_duplicates();
  sort_data();
  auto ans = process_events();
  return ans;
}

#endif //CONTEST_2_INTERNAL_POINT_H
