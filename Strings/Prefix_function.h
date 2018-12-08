//
// Created by rvg77 on 03.11.18.
//

#ifndef CONTEST_2_PREFIX_FUNCTION_H
#define CONTEST_2_PREFIX_FUNCTION_H

#include <string>
#include <vector>

class PrefixFunction {
  std::string s;

 public:
  PrefixFunction() = default;
  explicit PrefixFunction(const std::string &s) {
    this->s = s;
  };

  void set_string(const std::string &new_string);

  auto get_prefix_function() const;

  auto get_prefix_function_from_z_function(const std::vector<size_t> &z) const;

};

void PrefixFunction::set_string(const std::string &new_string) {
  this->s = new_string;
}

auto PrefixFunction::get_prefix_function() const {
  std::vector<size_t> pi(s.size(), 0);
  for (size_t i = 1; i < s.size(); ++i) {
    auto j = pi[i - 1];

    while (j > 0 && s[j] != s[i])
      j = pi[j - 1];

    pi[i] = ((s[i] == s[j]) ? j + 1 : 0);
  }

  return pi;
}

auto PrefixFunction::get_prefix_function_from_z_function(const std::vector<size_t> &z) const {
  std::vector<size_t> pi(z.size(), 0);

  for (size_t i = 1; i < z.size(); ++i)
    if (z[i])
      for (int j = (int) z[i] - 1; j >= 0 && !(pi[i + j]); --j)
        pi[i + j] = (size_t) j + 1;

  return pi;
}

#endif //CONTEST_2_PREFIX_FUNCTION_H
