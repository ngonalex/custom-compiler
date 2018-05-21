#ifndef PARSER_PARSE_RESULT_H_
#define PARSER_PARSE_RESULT_H_

#include "frontend/parser/parser.h"

using namespace cs160::frontend;


namespace cs160 {
namespace frontend {

class ParseResult {
 public:
  bool isSuccess;
  std::string error_message;
  std::unique_ptr<const AstNode> ast_result;
  
  ParseResult(bool isSuccess, std::string error_message, std::unique_ptr<const AstNode> ast_result) {
    this->isSuccess = isSuccess;
    this->error_message = error_message;
    this->ast_result = std::move(ast_result);
  }

  static ParseResult failure(std::string msg) {
    return ParseResult(false, msg, nullptr);
  }
  static ParseResult success(std::unique_ptr<const AstNode> result) {
    return ParseResult(true, "", std::move(result));
  }
};

} // namespace frontend
} // namespace cs160


#endif