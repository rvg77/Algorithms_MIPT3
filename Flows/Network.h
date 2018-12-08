//
// Created by rvg77 on 23.10.18.
//
#include <iostream>
#include <vector>

#ifndef ALGO_CONTEST_NETWORK_H
#define ALGO_CONTEST_NETWORK_H

class Network {
 public:
  typedef size_t VertexType;
  typedef long long FlowType;

  struct Edge {
    VertexType from, to;
    FlowType capacity;
    mutable FlowType flow;
    Edge *rev;
    Edge *pair;

    Edge() = default;
    Edge(VertexType from, VertexType to, FlowType cap) : from(from), to(to), capacity(cap), flow(0), rev(nullptr), pair(
        nullptr) {};
    Edge(Edge &&edge) noexcept
        : from(edge.from), to(edge.to), capacity(edge.capacity), flow(edge.flow), rev(edge.rev), pair(edge.pair) {
      if (edge.rev == nullptr)
        return;

      this->rev = edge.rev;
      edge.rev = nullptr;
      this->rev->rev = this;

      this->pair = edge.pair;
      edge.pair = nullptr;
      this->pair->pair = this;
    };
    Edge &operator=(const Edge &edge) = delete;
    Edge &operator=(Edge &&edge) = delete;

    bool is_full() const {
      return flow == capacity;
    }

    void update_edge(FlowType delta) const {
      flow += delta;
      rev->flow -= delta;
      pair->flow += delta;
      rev->pair->flow -= delta;
    }
  };

 private:
  bool is_oriented;

  VertexType vertex_number_, source_, target_;

  std::vector<std::vector<Edge>> graph_, reverse_graph_;

 public:
  typedef std::vector<Network::Edge>::const_iterator EdgeIterator;

  Network() = default;

  Network(bool is_oriented, VertexType vertex_number_, VertexType source_, VertexType target_)
      : is_oriented(is_oriented), vertex_number_(vertex_number_), source_(source_), target_(target_) {
    graph_.resize(vertex_number_);
    reverse_graph_.resize(vertex_number_);
  }

  EdgeIterator get_begin_from(VertexType v) const {
    return graph_[v].begin();
  }

  EdgeIterator get_end_from(VertexType v) const {
    return graph_[v].end();
  }

  EdgeIterator get_begin_to(VertexType v) const {
    return reverse_graph_[v].begin();
  }

  EdgeIterator get_end_to(VertexType v) const {
    return reverse_graph_[v].end();
  }

  void add_edge_to_graph(std::vector<std::vector<Edge> > &graph,
                         VertexType from,
                         VertexType to,
                         long long capacity) {

    graph[from].emplace_back(from, to, capacity);
    if (is_oriented)
      graph[to].emplace_back(to, from, 0);
    else
      graph[to].emplace_back(to, from, capacity);

    graph[to].back().rev = &graph[from].back();
    graph[from].back().rev = &graph[to].back();
  }

  void add_edge(VertexType from, VertexType to, long long capacity) {
    add_edge_to_graph(graph_, from, to, capacity);
    add_edge_to_graph(reverse_graph_, to, from, capacity);
    Edge &e1 = graph_[from].back(), &rev_e1 = graph_[to].back();
    Edge &e2 = reverse_graph_[from].back(), &rev_e2 = reverse_graph_[to].back();

    e1.pair = &rev_e2;
    e2.pair = &rev_e1;
    rev_e1.pair = &e2;
    rev_e2.pair = &e1;
  }

  VertexType get_source() const {
    return source_;
  }

  VertexType get_target() const {
    return target_;
  }

  VertexType get_vertex_number() const {
    return vertex_number_;
  }

  void reset_flow() {
    for (auto &vec : graph_)
      for (auto &edge : vec)
        edge.flow = 0;
  }
};

#endif //ALGO_CONTEST_NETWORK_H
