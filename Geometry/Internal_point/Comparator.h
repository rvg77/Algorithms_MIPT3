//
// Created by rvg77 on 28.11.18.
//

#ifndef CONTEST_2_COMPARATOR_H
#define CONTEST_2_COMPARATOR_H

#include "../Line/Line.h"
#include "../Point/Point.h"

template<class CoordinateType>
struct CompareLinePoint {
  CoordinateType operator()(const Line<CoordinateType> &l, const Point<CoordinateType> &p) {
    auto sign = l.a * p.x + l.b * p.y + l.c;
    return sign;
  }
};

#endif //CONTEST_2_COMPARATOR_H
