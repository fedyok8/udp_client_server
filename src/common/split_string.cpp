#include "split_string.hpp"

std::vector<std::string> common::split_string(const std::string& str,
                                              const std::string& delimeter) {
  std::vector<std::string> splitted;

  std::string::size_type position = 0;
  std::string::size_type previous = 0;

  while ((position = str.find(delimeter, previous)) != std::string::npos) {
    splitted.push_back(str.substr(previous, position - previous));
    previous = position + delimeter.size();
  }

  splitted.push_back(str.substr(previous));
  return splitted;
}
