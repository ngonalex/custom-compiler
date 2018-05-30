#ifndef PARSE_STATUS_H_
#define PARSE_STATUS_H_

#include "abstract_syntax/abstract_syntax.h"
#include "utility/memory.h"

#include <string> // std::string

using namespace cs160::abstract_syntax::frontend;

namespace cs160 {
namespace frontend {

template <class T> 
class ParseStatus { 
 public:
	// Success or failure
  bool status;
	// Start of the current string you parsed
	int character_start;
	// End of the current string you parsed
	int character_end;
	// Remaining characters from program string
  std::string remaining_chars;
	// Parsed characters from program string
  std::string parsed_chars;
	// If there's an error, it belongs here
	std::string error_type;
  // Template AST for the results that need ASTs
  std::unique_ptr<const T> ast_result;

	// check if two ParseStatuses are equal
	// bool operator==(const ParseStatus<T> &b) const;
	// bool operator!=(const ParseStatus<T> &b) const;

	// sucess state where you do NOT need to return an Ast Node
		// // ParseStatus<T>::sucess(int, int, string, string)
	static ParseStatus success(int character_start, int character_end, std::string remaining_chars, std::string parsed_chars) {
		return ParseStatus(true, character_start, character_end, remaining_chars, parsed_chars, "No Error", nullptr);
	}

	// sucess state where you DO need to return an Ast Node
		// ParseStatus<T>::sucess(int, int, string, string, ast)
	static ParseStatus success(int character_start, int character_end, std::string remaining_chars, std::string parsed_chars, std::unique_ptr<const T> ast_result) {
		return ParseStatus(true, character_start, character_end, remaining_chars, parsed_chars, "No Error", ast_result); 
	}

	// failure state : doesn't return an Ast Node ever
		// ParseStatus<T>::failure(int, string, string)
	static ParseStatus failure(int character_start, std::string remaining_chars, std::string error_type) {
		return ParseStatus(false, character_start, -1, remaining_chars, "", error_type, nullptr); 
	}

 private:
 	ParseStatus(bool status, int character_start, int character_end, std::string remaining_chars, std::string parsed_chars, std::string error_type, std::unique_ptr<const T> ast_result) {
		 this->status = status;
		 this->character_start = character_start;
		 this->character_end = character_end;
		 this->remaining_chars = remaining_chars;
		 this->parsed_chars = parsed_chars;
		 this->error_type = error_type;
		 this->ast_result = std::move(ast_result);
	 }
};

} // namespace frontend
} // namespace cs160

#endif // PARSE_STATUS_H_
