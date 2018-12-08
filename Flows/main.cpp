//
// Created by rvg77 on 23.10.18.
//
#include "Network.h"
#include "Push_preflow.h"
#include "Malhotra.h"
#include <map>
#include <cassert>

void solve(std::istream &in, std::ostream &out) {
  size_t n;
  in >> n;
  std::vector<long long> profit(n + 1);
  size_t s = 0, t = n + 1;
  bool is_oriented = true;

  Network network = Network(is_oriented, n + 2, s, t);

  for (size_t i = 1; i <= n; ++i) {
    long long cost;
    in >> cost;
    profit[i] = cost;
    if (cost > 0)
      network.add_edge(s, i, cost);
    else
      network.add_edge(i, t, -cost);
  }

  const long long inf = (long long) 1e9 + 13;

  for (size_t i = 1; i <= n; ++i) {
    size_t m;
    in >> m;
    for (size_t j = 0; j < m; ++j) {
      size_t p;
      in >> p;
      network.add_edge(i, p, inf);
    }
  }

  MKMSolver solver1 = MKMSolver(&network);
  MTFSolver solver2 = MTFSolver(&network);

  auto max_flow1 = solver1.find_max_flow();

  network.reset_flow();

  auto max_flow2 = solver2.find_max_flow();

  long long sum = 0;

  for (size_t i = 1; i <= n; ++i) {
    if (profit[i] > 0)
      sum += profit[i];
  }

  assert(max_flow1 == max_flow2);

  out << sum - max_flow1;
}

int main() {

  solve(std::cin, std::cout);

  return 0;
}