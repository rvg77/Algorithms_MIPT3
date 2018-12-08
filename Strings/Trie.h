//
// Created by rvg77 on 11.11.18.
//

#ifndef CONTEST_2_TRIE_H
#define CONTEST_2_TRIE_H

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

#endif //CONTEST_2_TRIE_H
