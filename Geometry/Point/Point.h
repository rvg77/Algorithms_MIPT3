//
// Created by rvg77 on 22.11.18.
//

#ifndef CONTEST_2_POINTS_H
#define CONTEST_2_POINTS_H

template<class CoordinateType>
struct Point {
  CoordinateType x, y;
  int id;

  Point() : x(), y(), id() {};
  Point(const CoordinateType &x, const CoordinateType &y) : x(x), y(y), id() {};
  Point(CoordinateType x, CoordinateType y, int id) : x(x), y(y), id(id) {};
};

template<class CoordinateType>
bool operator == (const Point<CoordinateType> &a, const Point<CoordinateType> &b) {
  return a.x == b.x && a.y == b.y;
}

template<class CoordinateType>
bool operator != (const Point<CoordinateType> &a, const Point<CoordinateType> &b) {
  return !(a == b);
}

template<class CoordinateType>
bool compareXY(const Point<CoordinateType> &a, const Point<CoordinateType> &b) {
  return a.x < b.x || (a.x == b.x && a.y < b.y);
}

#endif //CONTEST_2_POINTS_H
