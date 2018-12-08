//
// Created by rvg77 on 24.10.18.
//
#include <vector>
#include <list>
#include "Network.h"
#include "Abstract_solver.h"
#ifndef ALGO_CONTEST_PUSH_PREFLOW_H
#define ALGO_CONTEST_PUSH_PREFLOW_H

class MTFSolver : public Solver {
  typedef size_t HeightType;

  const Network *network;
  std::vector<HeightType> height;
  std::vector<Network::FlowType> excess;
  std::vector<Network::EdgeIterator> current_edge;
  std::list<Network::VertexType> L;

  void initialize_preflow();

  void push(const Network::Edge &edge);

  void relabel(Network::VertexType vertex);

  void discharge(Network::VertexType vertex);

  Network::FlowType update_edge(const Network::Edge &edge, Network::FlowType delta) {
    edge.update_edge(delta);
    excess[edge.from] -= delta;
    excess[edge.to] += delta;
    return edge.flow;
  }

 public:
  MTFSolver(Network *network) : network(network) {};

  void set_network(const Network *new_network);

  Network::FlowType find_max_flow();

};

void MTFSolver::set_network(const Network *new_network) {
  network = new_network;
  height.clear();
  excess.clear();
  L.clear();
}

void MTFSolver::initialize_preflow() {
  height.assign(network->get_vertex_number(), 0);
  height[network->get_source()] = network->get_vertex_number();

  excess.assign(network->get_vertex_number(), 0);
  for (auto it = network->get_begin_from(network->get_source()); it != network->get_end_from(network->get_source());
       ++it) {
    const Network::Edge &edge = *it;
    update_edge(edge, edge.capacity);
  }

  for (Network::VertexType vertex = 0; vertex < network->get_vertex_number(); ++vertex) {
    current_edge.push_back(network->get_begin_from(vertex));
  }

  L.clear();
  for (Network::VertexType vertex = 0; vertex < network->get_vertex_number(); ++vertex) {
    if (vertex != network->get_source() && vertex != network->get_target())
      L.push_back(vertex);
  }
}

void MTFSolver::push(const Network::Edge &edge) {
  Network::FlowType delta = std::min(excess[edge.from], edge.capacity - edge.flow);
  update_edge(edge, delta);
}

void MTFSolver::relabel(Network::VertexType vertex) {
  auto min_height = height[vertex];

  for (auto it = network->get_begin_from(vertex); it != network->get_end_from(vertex); ++it) {
    const Network::Edge &edge = *it;

    if (edge.is_full())
      continue;

    min_height = std::min(min_height, height[edge.to]);
  }

  height[vertex] = min_height + 1;
}

void MTFSolver::discharge(Network::VertexType vertex) {
  while (excess[vertex] > 0) {
    if (current_edge[vertex] == network->get_end_from(vertex)) {
      relabel(vertex);
      current_edge[vertex] = network->get_begin_from(vertex);
      continue;
    }

    const Network::Edge &edge = *current_edge[vertex];

    if (!edge.is_full() && height[edge.from] == height[edge.to] + 1)
      push(edge);
    else
      ++current_edge[vertex];
  }
}

Network::FlowType MTFSolver::find_max_flow() {
  initialize_preflow();

  auto it = L.begin();

  while (it != L.end()) {
    auto vertex = *it;
    auto previous_height = height[vertex];

    discharge(vertex);

    if (previous_height < height[vertex]) {
      L.erase(it);
      L.push_front(vertex);
      it = L.begin();
    } else
      ++it;
  }

  return -excess[network->get_source()];
}

#endif //ALGO_CONTEST_PUSH_PREFLOW_H
