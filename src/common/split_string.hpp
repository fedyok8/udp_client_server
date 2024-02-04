#ifndef SRC_COMMON_SPLIT_STRING_HPP__
#define SRC_COMMON_SPLIT_STRING_HPP__

#include <vector>
#include <string>

namespace common {

std::vector<std::string> split_string(const std::string& str,
                                      const std::string& delimeter);

} // namespace common

#endif // SRC_COMMON_SPLIT_STRING_HPP__
