//
// Created by rvg77 on 28.11.18.
//

#ifndef CONTEST_2_LINE_H
#define CONTEST_2_LINE_H

#include "../Point/Point.h"

template<class CoordinateType>
struct Line {
  CoordinateType a, b, c;

  Line() : a(), b(), c() {};
  Line(const Point<CoordinateType> &p1, const Point<CoordinateType> &p2);
};

template <class CoordinateType>
Line<CoordinateType>::Line(const Point<CoordinateType> &p1, const Point<CoordinateType> &p2) {
  a = p1.y - p2.y;
  b = p2.x - p1.x;
  c = -a * p1.x - b * p1.y;
}

#endif //CONTEST_2_LINE_H
