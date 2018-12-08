//
// Created by rvg77 on 26.10.18.
//
#include "Network.h"

#ifndef ALGO_CONTEST_ABSTRACT_SOLVER_H
#define ALGO_CONTEST_ABSTRACT_SOLVER_H

class Solver {
 public:

  virtual void set_network(const Network *new_network) = 0;

  virtual Network::FlowType find_max_flow() = 0;
};

#endif //ALGO_CONTEST_ABSTRACT_SOLVER_H
