#include "resourse_parser.hpp"

#include <fstream>
#include <iostream>

#include "common/split_string.hpp"

ResourseParser::ResourseParser(const std::string& filepath) {
  std::ifstream file(filepath);
  if (!file.is_open()) {
    std::clog << "FATAL: cannot open " << filepath << std::endl;
    return;
  }
  std::string line;
  while (std::getline(file, line)) {
    if (line.empty() || line[0] == '#') {
      continue;
    }
    std::vector<std::string> splitted = common::split_string(line, "=");
    if (splitted.size() != 2) {
      std::clog << "WARN: incorrect line format" << line;
      continue;
    }
    content_.emplace(splitted[0], splitted[1]);
  }
  std::clog << "Parsed from file " << filepath << std::endl;
  for (const auto& pair : content_) {
    std::clog << "\""
              << pair.first
              << "\" -> \""
              << pair.second
              << "\""
              << std::endl;
  }
}

bool ResourseParser::Contains(const std::string& resource) const {
  return content_.find(resource) != content_.cend();
}

std::string ResourseParser::Content(const std::string& resource) const {
  if (!Contains(resource)) {
    return {};
  }
  return content_.at(resource);
}

