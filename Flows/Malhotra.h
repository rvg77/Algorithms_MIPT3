//
// Created by rvg77 on 24.10.18.
//
#include "Network.h"
#include <queue>
#include "Abstract_solver.h"
#ifndef ALGO_CONTEST_MALHOTRA_H
#define ALGO_CONTEST_MALHOTRA_H

class MKMSolver : public Solver {
  const Network *network;
  std::vector<size_t> distance;
  std::vector<Network::FlowType> potential_in, potential_out;
  std::vector<Network::FlowType> amount_to_push;

  Network::FlowType update_edge(const Network::Edge &edge, Network::FlowType delta, bool is_reversed) {
    edge.update_edge(delta);

    if (!is_reversed) {
      potential_in[edge.to] -= delta;
      potential_out[edge.from] -= delta;
    } else {
      potential_out[edge.to] -= delta;
      potential_in[edge.from] -= delta;
    }

    amount_to_push[edge.from] -= delta;
    amount_to_push[edge.to] += delta;
    return edge.flow;
  }

  Network::FlowType potential(Network::VertexType vertex) const {
    if (vertex == network->get_source())
      return potential_out[vertex];

    if (vertex == network->get_target())
      return potential_in[vertex];

    return std::min(potential_out[vertex], potential_in[vertex]);
  }

  void reset();

  bool bfs();

  void initialize_potentials();

  void push(Network::VertexType start, Network::FlowType value, bool is_reversed);

  void remove(Network::VertexType vertex, bool is_reversed);

  Network::FlowType find_blocking_flow();

 public:
  const size_t INF = (size_t) 1e9 + 13;

  MKMSolver(Network *network) : network(network) {};

  void set_network(const Network *new_network);

  Network::FlowType find_max_flow();

};

void MKMSolver::set_network(const Network *new_network) {
  network = new_network;
}

bool MKMSolver::bfs() {
  std::queue<Network::VertexType> queue;

  std::fill(distance.begin(), distance.end(), INF);

  distance[network->get_source()] = 0;
  queue.push(network->get_source());

  while (!queue.empty()) {
    auto vertex = queue.front();
    queue.pop();

    for (auto it = network->get_begin_from(vertex); it != network->get_end_from(vertex); ++it) {
      const Network::Edge &edge = *it;

      if (!edge.is_full() && distance[edge.to] == INF) {
        distance[edge.to] = distance[vertex] + 1;
        queue.push(edge.to);
      }
    }
  }

  return distance[network->get_target()] != INF;
}

void MKMSolver::reset() {
  distance.resize(network->get_vertex_number());
  potential_in.resize(network->get_vertex_number());
  potential_out.resize(network->get_vertex_number());
  amount_to_push.resize(network->get_vertex_number());
}

Network::FlowType MKMSolver::find_max_flow() {
  Network::FlowType max_flow = 0;

  reset();

  while (bfs()) {
    initialize_potentials();

    auto delta_flow = find_blocking_flow();
    while (delta_flow) {
      max_flow += delta_flow;
      delta_flow = find_blocking_flow();
    }
  }

  return max_flow;
}

void MKMSolver::initialize_potentials() {
  potential_in.assign(potential_in.size(), 0);
  potential_out.assign(potential_out.size(), 0);

  for (Network::VertexType vertex = 0; vertex < network->get_vertex_number(); ++vertex) {
    for (auto it = network->get_begin_from(vertex); it != network->get_end_from(vertex); ++it) {
      const Network::Edge &edge = *it;

      if (distance[edge.to] != distance[edge.from] + 1)
        continue;

      auto diff = edge.capacity - edge.flow;
      potential_in[edge.to] += diff;
      potential_out[edge.from] += diff;
    }
  }
}

void MKMSolver::push(Network::VertexType start, Network::FlowType value, bool is_reversed) {
  std::queue<Network::VertexType> queue;

  amount_to_push.assign(network->get_vertex_number(), 0);
  amount_to_push[start] += value;
  queue.push(start);

  while (!queue.empty()) {
    auto vertex = queue.front();
    queue.pop();

    if (!amount_to_push[vertex])
      continue;

    auto begin_it = (is_reversed) ? (network->get_begin_to(vertex)) : (network->get_begin_from(vertex));
    auto end_it = (is_reversed) ? (network->get_end_to(vertex)) : (network->get_end_from(vertex));

    for (auto it = begin_it; it != end_it; ++it) {
      const Network::Edge &edge = *it;

      auto direction = ((is_reversed) ? (-1) : (1));
      if (distance[edge.from] + direction != distance[edge.to])
        continue;

      auto can_to_push = std::min(edge.capacity - edge.flow, amount_to_push[vertex]);
      if (!can_to_push)
        continue;

      if (!amount_to_push[edge.to] && edge.to != network->get_source() && edge.to != network->get_target())
        queue.push(edge.to);

      update_edge(edge, can_to_push, is_reversed);
    }
  }
}

Network::FlowType MKMSolver::find_blocking_flow() {

  while (true) {
    Network::VertexType min_potential_vertex = INF;

    for (Network::VertexType vertex = 0; vertex < network->get_vertex_number(); ++vertex) {
      if (distance[vertex] == INF)
        continue;

      if (min_potential_vertex == INF || (potential(vertex) < potential(min_potential_vertex)))
        min_potential_vertex = vertex;
    }

    if (min_potential_vertex == INF)
      return 0;

    if (!potential(min_potential_vertex)) {
      remove(min_potential_vertex, true);
      remove(min_potential_vertex, false);
      distance[min_potential_vertex] = INF;
      continue;
    }

    auto delta = potential(min_potential_vertex);
    push(min_potential_vertex, delta, true);
    push(min_potential_vertex, delta, false);
    return delta;
  }
}

void MKMSolver::remove(Network::VertexType vertex, bool is_reversed) {

  auto begin_it = (is_reversed) ? (network->get_begin_to(vertex)) : (network->get_begin_from(vertex));
  auto end_it = (is_reversed) ? (network->get_end_to(vertex)) : (network->get_end_from(vertex));

  for (auto it = begin_it; it != end_it; ++it) {
    const Network::Edge &edge = *it;

    auto direction = ((is_reversed) ? (-1) : (1));
    if (distance[edge.from] + direction != distance[edge.to])
      continue;

    auto amount_to_reduce = edge.capacity - edge.flow;
    if (is_reversed) {
      potential_in[edge.from] -= amount_to_reduce;
      potential_out[edge.to] -= amount_to_reduce;
    } else {
      potential_out[edge.from] -= amount_to_reduce;
      potential_in[edge.to] -= amount_to_reduce;
    }

  }
}

#endif //ALGO_CONTEST_MALHOTRA_H
