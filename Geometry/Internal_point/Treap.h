//
// Created by rvg77 on 22.11.18.
//

#ifndef CONTEST_2_TREAP_H
#define CONTEST_2_TREAP_H

#include "Position.h"
#include "../Point/Point.h"

template<typename CoordinateType>
class Treap {
  Treap *left_, *right_;
  CoordinateType a_, b_, c_; // line
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
  Treap() : left_(nullptr), right_(nullptr), a_(), b_(), c_(), size_(0), prior_(rand()) {};
  Treap(CoordinateType a, CoordinateType b, CoordinateType c)
      : left_(nullptr), right_(nullptr), a_(a), b_(b), c_(c), size_(1), prior_(rand()) {};

  ~Treap();

  static Treap *merge(Treap *left, Treap *right);

  static void split(Treap *head, const Point<CoordinateType> &p, Treap *&left, Treap *&right);

  static EPositionType find(Treap *head, const Point<CoordinateType> &p);
};

template <typename CoordinateType>
Treap<CoordinateType>::~Treap() {
    delete left_;
    delete right_;
}
template <typename CoordinateType>
Treap<CoordinateType>* Treap<CoordinateType>::merge(Treap *left, Treap *right) {
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

template <typename CoordinateType>
void Treap<CoordinateType>::split(Treap *head, const Point<CoordinateType> &p, Treap *&left, Treap<CoordinateType> *&right) {
  if (head == nullptr) {
    left = right = nullptr;
    return;
  }

  CoordinateType sign = head->a_ * p.x + head->b_ * p.y + head->c_;
  if (sign == 0) {
    Treap *t1, *t2;
    split(head->left_, p, left, t1);
    split(head->right_, p, t2, right);
  } else if (sign < 0) {
    split(head->left_, p, left, head->left_);
    head->update_size();
    right = head;
  } else {
    split(head->right_, p, head->right_, right);
    head->update_size();
    left = head;
  }
}

template <typename CoordinateType>
EPositionType Treap<CoordinateType>::find(Treap *head, const Point<CoordinateType> &p) {
  if (head == nullptr)
    return OUTSIDE;

  CoordinateType sign = head->a_ * p.x + head->b_ * p.y + head->c_;
  if (sign == 0)
    return BORDER;
  if (sign < 0) {
    return find(head->left_, p);
  } else {
    size_t leftSize = 0;
    if (head->left_ != nullptr)
      leftSize = head->left_->size_ & 1; // mod 2
    EPositionType type = find(head->right_, p);
    if (type == BORDER)
      return BORDER;
    if (leftSize ^ 1) // leftSize is even
      return (type == INSIDE) ? OUTSIDE : INSIDE;
    return type;
  }
}

#endif //CONTEST_2_TREAP_H
