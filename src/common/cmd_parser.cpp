#include "cmd_parser.hpp"

namespace {

bool IsShortArg(const std::string& arg) {
  if (arg.size() < 2) {
    return false;
  }
  return arg[0] == '-';
}

bool IsLongArg(const std::string& arg) {
  if (arg.size() < 3) {
    return false;
  }
  return (arg[0] == '-') && (arg[1] == '-');
}

bool IsArg(const std::string& arg) {
  return IsLongArg(arg) || IsShortArg(arg);
}

}

CmdParser::CmdParser(int argc, char** argv)
  : args_(argc) {
  for (int i = 0; i < args_.size(); ++i) {
    args_[i] = std::string(argv[i]);
  }
}

bool CmdParser::FindFlag(char f_short, std::string f_long) const {
  std::string s = std::string("-") + f_short;
  std::string l = std::string("--") + f_long;
  for (int i = 0; i < args_.size(); ++i) {
    if (args_[i] == s || args_[i] == l) {
      return true;
    }
  }
  return false;
}

std::string CmdParser::FindOption(char f_short, std::string f_long) const {
  std::string s = std::string("-") + f_short;
  std::string l = std::string("--") + f_long;
  for (int i = 0; i < args_.size() - 1; ++i) {
    if ((args_[i] == s || args_[i] == l) && !IsArg(args_[i + 1])) {
      return args_[i + 1];
    }
  }
  return {};
}
