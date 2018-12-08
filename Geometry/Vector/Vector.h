//
// Created by rvg77 on 23.11.18.
//

#ifndef CONTEST_2_VECTOR_H
#define CONTEST_2_VECTOR_H

#include "../Point/Point.h"

template<typename CoordinateType>
struct Vector {
  CoordinateType x, y;
  Vector() : x(), y() {};
  Vector(const CoordinateType &x, const CoordinateType &y) : x(x), y(y) {};
  ~Vector() = default;
  Vector(const Point<CoordinateType> &start, const Point<CoordinateType> &finish);
};

template <typename CoordinateType>
Vector<CoordinateType>::Vector(const Point<CoordinateType> &start, const Point<CoordinateType> &finish) {
  x = finish.x - start.x;
  y = finish.y - start.y;
}

template <typename CoordinateType>
CoordinateType cross_product(const Vector<CoordinateType> &a, const Vector<CoordinateType> &b) {
  return a.x * b.y - a.y * b.x;
}

#endif //CONTEST_2_VECTOR_H
