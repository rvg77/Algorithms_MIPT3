//
// Created by rvg77 on 16.11.18.
//

#ifndef CONTEST_2_AHO_CORASICK_H
#define CONTEST_2_AHO_CORASICK_H

#include <set>
#include <string>
#include <unordered_map>
#include <vector>
#include <map>
#include <iostream>

class Trie {
 public:
  struct Node {
    size_t id;
    size_t number_of_ends;
    size_t number_of_ends_in_subtree;

    std::map<char, Node *> next;

    Node() : id(1), number_of_ends(0), number_of_ends_in_subtree(0) {
      next = std::map<char, Node *>();
    };
    virtual ~Node() {
      for (auto &item : next) {
        if (item.second != nullptr)
          delete item.second;
      }
    }
  };
 protected:
  size_t size_;
  Node *root_;
 public:
  size_t size();
  Node *get_root() const;

  Trie();
  virtual ~Trie();

  Trie(const Trie &) = delete;
  Trie(Trie &&) = delete;
  Trie &operator=(const Trie &) = delete;
  Trie &operator=(Trie &&) = delete;

  void insert(const std::string &s);

  size_t find(const std::string &s);

  std::string find_kth(size_t k);
};

size_t Trie::size() {
  return size_;
}

Trie::Node* Trie::get_root() const {
  return root_;
}

Trie::~Trie() {
  delete root_;
}

Trie::Trie() : size_(0) {
  root_ = new Node();
}

void Trie::insert(const std::string &s) {
  auto cur = root_;
  for (auto c : s) {
    ++cur->number_of_ends_in_subtree;
    if (cur->next.find(c) != cur->next.end())
      cur = cur->next[c];
    else {
      auto pointer = new Node();
      cur->next[c] = pointer;
      cur = pointer;
    }
  }
  if (!cur->number_of_ends)
    ++size_;
  ++(cur->number_of_ends);
  ++(cur->number_of_ends_in_subtree);
}

size_t Trie::find(const std::string &s) { // returns the quantity of string s in trie
  auto cur = root_;
  for (auto c : s) {
    if (cur->next.find(c) != cur->next.end())
      cur = cur->next[c];
    else
      return 0;
  }

  return cur->number_of_ends;
}

std::string Trie::find_kth(size_t k) {
  if (size() < k)
    throw "Trie does not contain k strings";

  auto cur = root_;
  std::string s;
  while (true) {
    //std::cout << k << ' ' << cur->number_of_ends << '\n';
    if (k <= cur->number_of_ends)
      return s;
    k -= cur->number_of_ends;
    for (auto it = cur->next.begin(); it != cur->next.end(); ++it) {
      size_t tmp = it->second->number_of_ends_in_subtree;
      char delta = it->first;
      if (k <= tmp) {
        s += std::string(1, delta);
        //std::cout << s << '\n';
        cur = it->second;
        break;
      }
      k -= tmp;
    }
  }
}

class AhoCorasick {
 public:
  struct Node {
    Node *parent;
    Node *suf_link;
    Node *compressed_link;
    char parent_char;
    size_t is_leaf;
    size_t id;
    std::vector<size_t> sample_ids;
    std::unordered_map<char, Node *> son;
    std::unordered_map<char, Node *> go;

    Node() :
    parent(nullptr), suf_link(nullptr), compressed_link(nullptr), parent_char((char)0), is_leaf(0), id(0) {
      sample_ids = std::vector<size_t>();
      son = std::unordered_map<char, Node *>();
      go = std::unordered_map<char, Node *>();
    };

    ~Node() {
      for (auto &item : son)
        delete item.second;
    };
  };
  size_t size_;
  Node *root_;

  Node *get_go(Node *vertex, char c);
  Node *get_suf_link(Node *vertex);
  Node *get_compressed_link(Node *vertex);

  AhoCorasick();
  virtual ~AhoCorasick();

  AhoCorasick(const AhoCorasick &) = delete;
  AhoCorasick(AhoCorasick &&) = delete;
  AhoCorasick &operator=(const AhoCorasick &) = delete;
  AhoCorasick &operator=(AhoCorasick &&) = delete;

  void add_sample(const std::string &str, size_t id);
};

AhoCorasick::~AhoCorasick() {
  delete root_;
}

AhoCorasick::AhoCorasick() {
  root_ = new Node();
  size_ = 1;
}

AhoCorasick::Node *AhoCorasick::get_suf_link(AhoCorasick::Node *vertex) {
  if (vertex->suf_link == nullptr) {
    if (vertex == root_ || vertex->parent == root_)
      vertex->suf_link = root_;
    else
      vertex->suf_link = get_go(get_suf_link(vertex->parent), vertex->parent_char);
  }
  return vertex->suf_link;
}

AhoCorasick::Node *AhoCorasick::get_go(AhoCorasick::Node *vertex, char c) {
  if (vertex->go.find(c) == vertex->go.end()) {
    if (vertex->son.find(c) != vertex->son.end())
      vertex->go[c] = vertex->son[c];
    else {
      if (vertex == root_)
        vertex->go[c] = root_;
      else
        vertex->go[c] = get_go(get_suf_link(vertex), c);
    }
  }
  return vertex->go[c];
}

AhoCorasick::Node *AhoCorasick::get_compressed_link(AhoCorasick::Node *vertex) {
  if (vertex->compressed_link == nullptr) {
    auto s_l = get_suf_link(vertex);
    if (s_l->is_leaf)
      vertex->compressed_link = s_l;
    else {
      if (s_l == root_)
        vertex->compressed_link = root_;
      else
        vertex->compressed_link = get_compressed_link(s_l);
    }
  }
  return vertex->compressed_link;
}

void AhoCorasick::add_sample(const std::string &str, size_t id) {
  auto cur = root_;
  for (const auto &c : str) {
    if (cur->son.find(c) == cur->son.end()) {
      auto pointer = new Node();
      ++size_;
      pointer->id = size_ - 1;
      pointer->parent = cur;
      pointer->parent_char = c;
      cur->son[c] = pointer;
    }
    cur = cur->son[c];
  }

  ++cur->is_leaf;
  cur->sample_ids.push_back(id);
}

#endif //CONTEST_2_AHO_CORASICK_H
