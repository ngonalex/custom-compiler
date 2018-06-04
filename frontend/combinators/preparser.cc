#include "frontend/combinators/preparser.h"
#include <regex>

std::string preparse_Program(std::string program) {
  program = std::regex_replace(program, std::regex("\t"), "");
  program = std::regex_replace(program, std::regex("\\( +"), "(");
  program = std::regex_replace(program, std::regex(" +\\)"), ")");
  program = std::regex_replace(program, std::regex("; +"), ";");
  program = std::regex_replace(program, std::regex("( *)\\+( *)"), "+");
  program = std::regex_replace(program, std::regex("( *)-( *)"), "-");
  program = std::regex_replace(program, std::regex("( *)/( *)"), "/");
  program = std::regex_replace(program, std::regex("( *)\\*( *)"), "*");
  return program;
}