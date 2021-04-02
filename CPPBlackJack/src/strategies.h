#pragma once
#include <string>
#include <vector>

std::vector<std::vector<std::string>> strat_hard = {
    {"0", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11"},
    {"2", "H", "H", "H", "H", "H", "H", "H", "H", "H", "H"},
    {"3", "H", "H", "H", "H", "H", "H", "H", "H", "H", "H"},
    {"4", "H", "H", "H", "H", "H", "H", "H", "H", "H", "H"},
    {"5", "H", "H", "H", "H", "H", "H", "H", "H", "H", "H"},
    {"6", "H", "H", "H", "H", "H", "H", "H", "H", "H", "H"},
    {"7", "H", "H", "H", "H", "H", "H", "H", "H", "H", "H"},
    {"8", "H", "H", "H", "H", "H", "H", "H", "H", "H", "H"},
    {"9", "H", "D", "D", "D", "D", "H", "H", "H", "H", "H"},
    {"10", "D", "D", "D", "D", "D", "D", "D", "D", "H", "H"},
    {"11", "D", "D", "D", "D", "D", "D", "D", "D", "D", "H"},
    {"12", "H", "H", "S", "S", "S", "H", "H", "H", "H", "H"},
    {"13", "S", "S", "S", "S", "S", "H", "H", "H", "H", "H"},
    {"14", "S", "S", "S", "S", "S", "H", "H", "H", "H", "H"},
    {"15", "S", "S", "S", "S", "S", "H", "H", "H", "H", "H"},
    {"16", "S", "S", "S", "S", "S", "H", "H", "H", "H", "H"},
    {"17", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"},
    {"18", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"},
    {"19", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"},
    {"20", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"},
    {"21", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"}};

std::vector<std::vector<std::string>> strat_soft = {
    {"0", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11"},
    {"13", "H", "H", "H", "D", "D", "H", "H", "H", "H", "H"},
    {"14", "H", "H", "H", "D", "D", "H", "H", "H", "H", "H"},
    {"15", "H", "H", "D", "D", "D", "H", "H", "H", "H", "H"},
    {"16", "H", "H", "D", "D", "D", "H", "H", "H", "H", "H"},
    {"17", "H", "D", "D", "D", "D", "H", "H", "H", "H", "H"},
    {"18", "S", "D", "D", "D", "D", "S", "S", "H", "H", "H"},
    {"19", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"},
    {"20", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"},
    {"21", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"}};

std::vector<std::vector<std::string>> strat_split = {
    {"0", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11"},
    {"2", "P", "P", "P", "P", "P", "P", "H", "H", "H", "H"},
    {"3", "P", "P", "P", "P", "P", "P", "H", "H", "H", "H"},
    {"4", "H", "H", "H", "P", "P", "H", "H", "H", "H", "H"},
    {"6", "P", "P", "P", "P", "P", "H", "H", "H", "H", "H"},
    {"7", "P", "P", "P", "P", "P", "P", "H", "H", "H", "H"},
    {"8", "P", "P", "P", "P", "P", "P", "P", "P", "P", "P"},
    {"9", "P", "P", "P", "P", "P", "S", "P", "P", "S", "S"},
    {"11", "P", "P", "P", "P", "P", "P", "P", "P", "P", "P"}};

inline char const get_action(int const player_val, int dealer_val,
                              std::vector<char>* strategy) {
  const auto key =
      (player_val + dealer_val) * (player_val + dealer_val + 1) / 2 +
      dealer_val;
  return (*strategy)[key];
}

inline std::vector<char> vec_to_map(
    std::vector<std::vector<std::string>> vec) {
  std::vector<char> temp(1000);
  for (auto row = 0; row < static_cast<int>(vec.size()); row++) {
    for (auto col = 0; col < static_cast<int>(vec[0].size()); col++) {
      const auto player_val = std::stoi(vec[row][0]);
      const auto dealer_val = std::stoi(vec[0][col]);
      const auto key = (player_val + dealer_val) * (player_val + dealer_val + 1) / 2 +
                       dealer_val;
      temp[key] = vec[row][col][0];
    }
  }
  return temp;
}
