#ifndef SRC_COMMON_CMD_PARSER_HPP__
#define SRC_COMMON_CMD_PARSER_HPP__

#include <string>
#include <vector>

class CmdParser {
 public:
  CmdParser(int argc, char** argv);
  bool FindFlag(char f_short, std::string f_long) const;
  std::string FindOption(char f_short, std::string f_long) const;

 private:
  std::vector<std::string> args_;
};

#endif // SRC_COMMON_CMD_PARSER_HPP__
