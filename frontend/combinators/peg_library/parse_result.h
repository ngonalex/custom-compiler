#ifndef PARSE_RESULT_H_
#define PARSE_RESULT_H_

#include <string> // std::string

namespace cs160 {
namespace frontend {

template <class T> 
class ParseResult {
 public:
  std::string initial_chars;
  std::string remaining_chars;
  std::string parsed_chars;
  
  bool status;
  std::string error_message;
  std::unique_ptr<const T> ast_result;
  
  static ParseResult failure(std::string msg) {
    return ParseResult(false, msg, nullptr, false);
  }
  static ParseResult success(std::unique_ptr<const T> result) {
    return ParseResult(true, "", std::move(result), false);
  }

 private:
  ParseResult(bool status, std::string error_message, std::unique_ptr<const T> ast_result) {
    this->status = status;
    this->error_message = error_message;
    this->ast_result = std::move(ast_result);
  }
};

} // namespace frontend
} // namespace cs160


#endif // PARSE_RESULT_H_