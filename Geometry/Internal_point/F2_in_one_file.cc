//
// Created by rvg77 on 23.11.18.
//

#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

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

template<class CoordinateType>
struct Point {
  CoordinateType x, y;
  int id;

  Point() : x(), y(), id() {};
  Point(const CoordinateType &x, const CoordinateType &y) : x(x), y(y), id() {};
  Point(CoordinateType x, CoordinateType y, int id) : x(x), y(y), id(id) {};
};

template<class CoordinateType>
bool operator==(const Point<CoordinateType> &a, const Point<CoordinateType> &b) {
  return a.x == b.x && a.y == b.y;
}

template<class CoordinateType>
bool operator!=(const Point<CoordinateType> &a, const Point<CoordinateType> &b) {
  return !(a == b);
}

template<class CoordinateType>
bool compareXY(const Point<CoordinateType> &a, const Point<CoordinateType> &b) {
  return a.x < b.x || (a.x == b.x && a.y < b.y);
}

template<typename CoordinateType>
struct Vector {
  CoordinateType x, y;
  Vector() : x(), y() {};
  Vector(const CoordinateType &x, const CoordinateType &y) : x(x), y(y) {};
  ~Vector() = default;
  Vector(const Point<CoordinateType> &start, const Point<CoordinateType> &finish);
};

template<typename CoordinateType>
Vector<CoordinateType>::Vector(const Point<CoordinateType> &start, const Point<CoordinateType> &finish) {
  x = finish.x - start.x;
  y = finish.y - start.y;
}

template<typename CoordinateType>
CoordinateType cross_product(const Vector<CoordinateType> &a, const Vector<CoordinateType> &b) {
  return a.x * b.y - a.y * b.x;
}

template<class CoordinateType>
struct Line {
  CoordinateType a, b, c;

  Line() : a(), b(), c() {};
  Line(const Point<CoordinateType> &p1, const Point<CoordinateType> &p2);
};

template<class CoordinateType>
Line<CoordinateType>::Line(const Point<CoordinateType> &p1, const Point<CoordinateType> &p2) {
  a = p1.y - p2.y;
  b = p2.x - p1.x;
  c = -a * p1.x - b * p1.y;
}

struct Random {
  static const int MAX = static_cast<int>(1e9);
  static std::random_device rd;
  static std::mt19937 mt;
  static std::uniform_int_distribution<int> dist;
  static int get_rand() {
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
void Treap<ObjectType, KeyType, Compare>::split(Treap *head,
                                                const KeyType &key,
                                                Treap *&left,
                                                Treap *&right,
                                                Compare &cmp) {
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

template<class CoordinateType>
struct CompareLinePoint {
  CoordinateType operator()(const Line<CoordinateType> &l, const Point<CoordinateType> &p) {
    auto sign = l.a * p.x + l.b * p.y + l.c;
    return sign;
  }
};

template<typename CoordinateType>
struct Data {
  typedef std::vector<Point<CoordinateType>> VectorPointType;
  VectorPointType points, queries;
};

template<typename CoordinateType>
class InternalPoints {
 public:
  typedef Treap<Line<CoordinateType>, Point<CoordinateType>, CompareLinePoint<CoordinateType>> TreapType;
  typedef Point<CoordinateType> PointType;
  typedef Vector<CoordinateType> VectorType;
  typedef std::vector<EPositionType> AnsType;
  typedef std::vector<Point<CoordinateType>> PointsVectorType;
 private:
  size_t n, k; // number of points and queries
  PointsVectorType points, queries, unsorted_points;

 private:
  void sort_data();
  size_t real_index(int index) const;
  bool one_segment(size_t a, size_t b) const;
  PointType get_neighbour(size_t pos, bool next) const;
  void count_answers(TreapType *root, size_t &i, size_t &cur_id, AnsType &ans) const;
  void process_vertex(TreapType *&root, size_t i);
  AnsType process_events();
 public:
  InternalPoints() : n(), k(), points(), queries(), unsorted_points() {};
  InternalPoints(const Data<CoordinateType> & data)
      : n(data.points.size()), k(data.queries.size()), points(data.points), queries(data.queries) {};

  void delete_duplicates();
  AnsType get_points_positions();
};

template<typename CoordinateType>
void InternalPoints<CoordinateType>::sort_data() {
  std::sort(points.begin(), points.end(), compareXY<CoordinateType>);
  std::sort(queries.begin(), queries.end(), compareXY<CoordinateType>);
}

template<typename CoordinateType>
size_t InternalPoints<CoordinateType>::real_index(int index) const {
  auto ans = static_cast<size_t>(index + n);
  while (ans >= n)
    ans -= n;
  return ans;
}

template<typename CoordinateType>
void InternalPoints<CoordinateType>::delete_duplicates() {
  PointsVectorType raw = points;
  points.clear();
  for (size_t i = 0; i < n; ++i) {
    raw[i].id = 0;
    if (i == 0 || raw[i - 1] != raw[i]) {
      points.push_back(raw[i]);
      unsorted_points.push_back(raw[i]);
    }
  }
  n = points.size();
  for (size_t i = 0; i < n; ++i)
    points[i].id = unsorted_points[i].id = static_cast<int>(i);
}

template<typename CoordinateType>
bool InternalPoints<CoordinateType>::one_segment(size_t a, size_t b) const {
  return (abs(points[a].id - points[b].id) == 1 || abs(points[a].id - points[b].id) == static_cast<int>(n - 1));
}

template<typename CoordinateType>
Point<CoordinateType> InternalPoints<CoordinateType>::get_neighbour(size_t pos, bool next) const {
  int offset = ((next) ? 1 : -1);
  return unsorted_points[real_index(points[pos].id + offset)];
}

template<typename CoordinateType>
void InternalPoints<CoordinateType>::count_answers(TreapType *root, size_t &i,
                                                   size_t &cur_id,
                                                   InternalPoints<CoordinateType>::AnsType &ans) const {
  while (cur_id < k && compareXY(queries[cur_id], points[i])) {
    ans[queries[cur_id].id] = TreapType::find(root, queries[cur_id], CompareLinePoint<CoordinateType>());
    if (i && InternalPoints<CoordinateType>::one_segment(i, i - 1) && points[i].x == points[i - 1].x) // vertical case
      ans[queries[cur_id].id] = BORDER;
    ++cur_id;
  }
  while (cur_id < k && queries[cur_id] == points[i])
    ans[queries[cur_id++].id] = BORDER;
}

template<typename CoordinateType>
void InternalPoints<CoordinateType>::process_vertex(InternalPoints<CoordinateType>::TreapType *&root, size_t i) {
  TreapType *left = nullptr, *right = nullptr;
  auto cmp = CompareLinePoint<CoordinateType>();
  TreapType::split(root, points[i], left, right, cmp);
  root = left;

  PointType neighbours[2];
  neighbours[0] = get_neighbour(i, false);
  neighbours[1] = get_neighbour(i, true);

  VectorType v1(points[i], neighbours[0]);
  VectorType v2(points[i], neighbours[1]);
  if (cross_product(v1, v2) < 0)
    std::swap(neighbours[0], neighbours[1]);

  for (auto &neighbour : neighbours) {
    if (neighbour.x > points[i].x) {
      Line<CoordinateType> l(points[i], neighbour);
      root = TreapType::merge(root, new TreapType(l));
    }
  }
  root = TreapType::merge(root, right);
}

template<typename CoordinateType>
typename InternalPoints<CoordinateType>::AnsType InternalPoints<CoordinateType>::process_events() {
  AnsType ans(k);
  TreapType *root = nullptr;
  size_t cur_id = 0;
  for (size_t i = 0; i < n; ++i) {
    InternalPoints<CoordinateType>::count_answers(root, i, cur_id, ans);
    InternalPoints<CoordinateType>::process_vertex(root, i);
  }
  delete root;
  return ans;
}

template<typename CoordinateType>
typename InternalPoints<CoordinateType>::AnsType InternalPoints<CoordinateType>::get_points_positions() {
  delete_duplicates();
  sort_data();
  auto ans = process_events();
  return ans;
}

template<typename CoordinateType>
std::vector<Data<long long>> input(std::istream &in) {
  size_t test_number;
  in >> test_number;
  std::vector<Data<long long>> data(test_number);
  for (size_t test = 0; test < test_number; ++test) {
    size_t n, k;
    in >> n;
    data[test].points.resize(n);
    for (size_t i = 0; i < n; ++i)
      in >> data[test].points[i].x >> data[test].points[i].y;

    in >> k;
    data[test].queries.resize(k);
    for (size_t i = 0; i < k; ++i) {
      in >> data[test].queries[i].x >> data[test].queries[i].y;
      data[test].queries[i].id = static_cast<int>(i);
    }
  }
  return data;
}

void output(std::ostream &out, std::vector<InternalPoints<long long>::AnsType> &answers) {
  for (auto &vec: answers) {
    for (auto &answer : vec)
      out << position_to_string(answer) << '\n';
    out << '\n';
  }
}

std::vector<InternalPoints<long long>::AnsType> get_answers(std::vector<Data<long long>> &data) {
  auto test_number = data.size();

  std::vector<InternalPoints<long long>::AnsType> ans(test_number);
  for (size_t test = 0; test < test_number; ++test) {
    InternalPoints<long long> solver(data[test]);
    ans[test] = solver.get_points_positions();
  }

  return ans;
}

void solve(std::istream &in, std::ostream &out) {
  auto data = input<long long>(in);

  auto ans = get_answers(data);

  output(out, ans);
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
#ifdef LOCAL
  std::ifstream in("/home/rvg77/My_files/MIPT/АиСД_MIPT/3 semester/contest_2/Geometry/F2/input.txt");
  solve(in, std::cout);
#else
  solve(std::cin, std::cout);
#endif
  return 0;
}