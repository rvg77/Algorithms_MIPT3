//
// Created by rvg77 on 22.11.18.
//

#include <bits/stdc++.h>

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

struct Point {
  long long x, y;
  int id;
  Point() = default;
  Point(long long x, long long y, int id) : x(x), y(y), id(id) {}
};

bool operator==(const Point &a, const Point &b) {
  return a.x == b.x && a.y == b.y;
}

bool operator!=(const Point &a, const Point &b) {
  return !(a == b);
}

bool compareXY(const Point &a, const Point &b) {
  if (a.x != b.x)
    return a.x < b.x;
  return a.y < b.y;
}

long long cross_product(Point a, Point b) {
  return a.x * b.y - a.y * b.x;
}

class Treap {
  Treap *left, *right;
  long long a, b, c; // line
  int size, prior;

  void update_size() {
    size = 1;
    if (left != nullptr)
      size += left->size;
    if (right != nullptr)
      size += right->size;
  }

 public:
  Treap() : left(nullptr), right(nullptr), a(0), b(0), c(0), size(0), prior(rand()) {};
  Treap(long long a, long long b, long long c)
      : left(nullptr), right(nullptr), a(a), b(b), c(c), size(1), prior(rand()) {};

  static Treap *merge(Treap *left, Treap *right) {
    if (left == nullptr)
      return right;
    if (right == nullptr)
      return left;

    if (left->prior < right->prior) {
      left->right = merge(left->right, right);
      left->update_size();
      return left;
    } else {
      right->left = merge(left, right->left);
      right->update_size();
      return right;
    }
  }

  static void split(Treap *head, const Point &p, Treap *&left, Treap *&right) {
    if (head == nullptr) {
      left = right = nullptr;
      return;
    }

    long long sign = head->a * p.x + head->b * p.y + head->c;
    if (sign == 0) {
      Treap *t1, *t2;
      split(head->left, p, left, t1);
      split(head->right, p, t2, right);
    } else if (sign < 0) {
      split(head->left, p, left, head->left);
      head->update_size();
      right = head;
    } else {
      split(head->right, p, head->right, right);
      head->update_size();
      left = head;
    }
  }

  static EPositionType find(Treap *head, const Point &p) {
    if (head == nullptr)
      return OUTSIDE;

    long long sign = head->a * p.x + head->b * p.y + head->c;
    if (sign == 0)
      return BORDER;
    if (sign < 0) {
      return find(head->left, p);
    } else {
      int leftSize = 0;
      if (head->left != nullptr)
        leftSize = head->left->size & 1; // mod 2
      EPositionType type = find(head->right, p);
      if (type == BORDER)
        return BORDER;
      if (leftSize ^ 1) // leftSize is even
        return (type == INSIDE) ? OUTSIDE : INSIDE;
      return type;
    }
  }
};

struct Data {
  size_t n, k; // number of points and queries
  std::vector<Point> points, queries, not_sorted_points;

  Data() = default;
  explicit Data(std::istream &in) {
    n = k = 0;
    in >> n;
    points.resize(n);
    for (size_t i = 0; i < n; ++i) {
      in >> points[i].x >> points[i].y;
    }
    in >> k;
    queries.resize(k);
    for (size_t i = 0; i < k; ++i) {
      in >> queries[i].x >> queries[i].y;
      queries[i].id = (int) i;
    }
  }

};

class Solver {
 public:
  typedef std::vector<EPositionType> AnsType;
 private:
  Data data;
  AnsType ans;

  void delete_dublicates() {
    std::vector<Point> raw = data.points;
    data.points.clear();
    for (size_t i = 0; i < data.n; ++i) {
      raw[i].id = 0;
      if (i == 0 || !(raw[i - 1] == raw[i])) {
        data.points.push_back(raw[i]);
        data.not_sorted_points.push_back(raw[i]);
      }
    }
    data.n = data.points.size();
    for (size_t i = 0; i < data.n; ++i)
      data.points[i].id = data.not_sorted_points[i].id = (int) i;
  }

 public:
  explicit Solver(Data &&data) : data(data) {};


  AnsType solve() {
    delete_dublicates();
    ans.resize(data.k);
    std::sort(data.points.begin(), data.points.end(), compareXY);
    std::sort(data.queries.begin(), data.queries.end(), compareXY);

    Treap *head = nullptr;
    int cur_id = 0;
    for (size_t i = 0; i < data.n; ++i) {
      while (cur_id < data.k && compareXY(data.queries[cur_id], data.points[i])) {
        ans[data.queries[cur_id].id] = Treap::find(head, data.queries[cur_id]);
        if (i &&
            (abs(data.points[i].id - data.points[i - 1].id) == 1
                || abs(data.points[i].id - data.points[i - 1].id) == data.n - 1) &&
            data.points[i].x == data.points[i - 1].x)
          ans[data.queries[cur_id].id] = BORDER;
        ++cur_id;
      }
      while (cur_id < data.k && data.queries[cur_id] == data.points[i])
        ans[data.queries[cur_id++].id] = BORDER;

      Treap *left = nullptr, *right = nullptr;
      Treap::split(head, data.points[i], left, right);
      head = left;
      Point neighbour[2];
      neighbour[0] = data.not_sorted_points[(data.points[i].id - 1 + data.n) % data.n];
      neighbour[1] = data.not_sorted_points[(data.points[i].id + 1) % data.n];
      Point v1(neighbour[0].x - data.points[i].x, neighbour[0].y - data.points[i].y, -1);
      Point v2(neighbour[1].x - data.points[i].x, neighbour[1].y - data.points[i].y, -1);
      if (cross_product(v1, v2) < 0)
        std::swap(neighbour[0], neighbour[1]);
      for (int m = 0; m < 2; ++m) {
        if (neighbour[m].x > data.points[i].x) {
          long long a = data.points[i].y - neighbour[m].y;
          long long b = neighbour[m].x - data.points[i].x;
          long long c = -a * data.points[i].x - b * data.points[i].y;
          head = Treap::merge(head, new Treap(a, b, c));
        }
      }
      head = Treap::merge(head, right);
    }
    return ans;
  }
};

void out(std::ostream &out, const std::vector<EPositionType> &ans) {
  for (auto &ans_i : ans)
    out << position_to_string(ans_i) << "\n";
}

int main() {
#ifdef LOCAL
  freopen("xxx.in", "r", stdin);
  freopen("xxx.out", "w", stdout);
#endif
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  srand(time(nullptr));
  int t;
  std::cin >> t;
  for (int tt = 0; tt < t; ++tt) {
    Solver solver(Data(std::cin));
    std::vector<EPositionType> ans = solver.solve();
    out(std::cout, ans);
  }
  return 0;
}

