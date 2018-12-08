//
// Created by rvg77 on 28.11.18.
//

#ifndef CONTEST_2_ABSTRACT_TREAP_H
#define CONTEST_2_ABSTRACT_TREAP_H

#include "Position.h"

#include <random>
#include <ctime>

struct Random {
  static const int MAX = static_cast<int>(1e9);
  static std::random_device rd;
  static std::mt19937 mt;
  static std::uniform_int_distribution<int> dist;
  static auto get_rand() {
    return dist(mt);
  }
};
std::mt19937 Random::mt = std::mt19937(time(0));
std::uniform_int_distribution<int> Random::dist = std::uniform_int_distribution<int>(0, Random::MAX);

template
    <class ObjectType, class KeyType, class Compare>
class Treap {
  Treap *left_, *right_;
  ObjectType object_;
  size_t size_;
  int prior_;
  void update_size() {
    size_ = 1;
    if (left_ != nullptr)
      size_ += left_->size_;
    if (right_ != nullptr)
      size_ += right_->size_;
  }

 public:
  Treap() : left_(nullptr), right_(nullptr), object_(), size_(0), prior_(Random::get_rand()) {};
  explicit Treap(const ObjectType &object)
      : left_(nullptr), right_(nullptr), object_(object), size_(1), prior_(Random::get_rand()) {};

  ~Treap();

  static Treap *merge(Treap *left, Treap *right);

  static void split(Treap *head, const KeyType &key, Treap *&left, Treap *&right, Compare &cmp);

  static EPositionType find(Treap *head, const KeyType &key, const Compare &cmp);
};

template<class ObjectType, class KeyType, class Compare>
Treap<ObjectType, KeyType, Compare>::~Treap() {
  delete left_;
  delete right_;
}

template<class ObjectType, class KeyType, class Compare>
Treap<ObjectType, KeyType, Compare> *Treap<ObjectType, KeyType, Compare>::merge(Treap *left, Treap *right) {
  if (left == nullptr)
    return right;
  if (right == nullptr)
    return left;

  if (left->prior_ < right->prior_) {
    left->right_ = merge(left->right_, right);
    left->update_size();
    return left;
  } else {
    right->left_ = merge(left, right->left_);
    right->update_size();
    return right;
  }
}

template<class ObjectType, class KeyType, class Compare>
void Treap<ObjectType, KeyType, Compare>::split(Treap *head, const KeyType &key, Treap *&left, Treap *&right, Compare &cmp) {
  if (head == nullptr) {
    left = right = nullptr;
    return;
  }

  auto comparison_result = cmp(head->object_, key);
  if (comparison_result == 0) {
    Treap *t1, *t2;
    split(head->left_, key, left, t1, cmp);
    split(head->right_, key, t2, right, cmp);
  } else if (comparison_result < 0) {
    split(head->left_, key, left, head->left_, cmp);
    head->update_size();
    right = head;
  } else {
    split(head->right_, key, head->right_, right, cmp);
    head->update_size();
    left = head;
  }
}

template<class ObjectType, class KeyType, class Compare>
EPositionType Treap<ObjectType, KeyType, Compare>::find(Treap *head, const KeyType &key, const Compare &cmp) {
  if (head == nullptr)
    return OUTSIDE;

  auto comparison_result = Compare()(head->object_, key);
  if (comparison_result == 0)
    return BORDER;
  if (comparison_result < 0) {
    return find(head->left_, key, cmp);
  } else {
    size_t leftSize = 0;
    if (head->left_ != nullptr)
      leftSize = head->left_->size_ & 1; // mod 2
    EPositionType type = find(head->right_, key, cmp);
    if (type == BORDER)
      return BORDER;
    if (leftSize ^ 1) // leftSize is even
      return (type == INSIDE) ? OUTSIDE : INSIDE;
    return type;
  }
}

#endif //CONTEST_2_ABSTRACT_TREAP_H
