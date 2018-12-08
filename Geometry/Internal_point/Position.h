//
// Created by rvg77 on 22.11.18.
//

#ifndef CONTEST_2_POSITION_H
#define CONTEST_2_POSITION_H

#include <string>

enum EPositionType {
  OUTSIDE,
  INSIDE,
  BORDER
};

std::string position_to_string(EPositionType type) {
  if (type == BORDER)
    return "BORDER";
  if (type == INSIDE)
    return "INSIDE";
  if (type == OUTSIDE)
    return "OUTSIDE";
  return "";
}

#endif //CONTEST_2_POSITION_H
