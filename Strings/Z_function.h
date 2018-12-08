//
// Created by rvg77 on 03.11.18.
//

#ifndef CONTEST_2_Z_FUNCTION_H
#define CONTEST_2_Z_FUNCTION_H

#include <string>
#include <vector>

class ZFunction {
  std::string s;

 public:
  ZFunction() = default;
  explicit ZFunction(const std::string &str) {
    s = str;
  };

  void set_string(const std::string &new_string);

  auto get_z_function() const;

  auto get_z_function_from_prefix_function(const std::vector<size_t> &pi) const;
};

void ZFunction::set_string(const std::string &new_string) {
  s = new_string;
}

auto ZFunction::get_z_function() const {
  std::vector<size_t> z(s.size(), 0);

  for (size_t i = 1, l = 0, r = 0; i < s.size(); ++i) {
    if (i <= r)
      z[i] = std::min(r - i + 1, z[i - l]);

    while (i + z[i] < s.size() && s[z[i]] == s[i + z[i]])
      ++z[i];

    if (i + z[i] > r)
      l = i, r = i + z[i] - 1;
  }

  z[0] = s.size();

  return z;
}

auto ZFunction::get_z_function_from_prefix_function(const std::vector<size_t> &pi) const {
  std::vector<size_t> z(pi.size());

  for (size_t i = 1; i < pi.size(); ++i)
    if (pi[i])
      z[i - pi[i] + 1] = pi[i];

  z[0] = pi.size();

  if (z[1])
    for (size_t i = 1; i < z[1]; ++i)
      z[i + 1] = z[1] - i;

  size_t tmp;
  for (size_t i = z[1] + 1; i < z.size() - 1; ++i) {
    tmp = i;

    if (z[i] && !z[i + 1]) {
      for (size_t j = 1; j < z[i] && z[i + j] <= z[j]; ++j) {
        z[i + j] = std::min(z[j], z[i] - j);
        tmp = i + j;
      }
    }

    i = tmp;
  }

  return z;
}

#endif //CONTEST_2_Z_FUNCTION_H
