#ifndef ATOM_PARSER_H_
#define ATOM_PARSER_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/peg_library/parse_result.h"

using namespace cs160::abstract_syntax::frontend;

namespace cs160 {
namespace frontend {

class AtomParser {
 public:
	explicit AtomParser(char char_to_parse) {
		char_to_parse_ = char_to_parse;
	}

	explicit AtomParser() {
		char_to_parse_ = '\0';
	}

// return ParseResult<Assignment>::failure("Missing ':' inside an assignment");
	ParseResult<AstNode> parse(std::string input) {
		if (input.size() == 0) {
			return ParseResult<AstNode>::failure("Failed to parse");
		} else if (input[0] == char_to_parse_ || char_to_parse_ == '\0') {
			return ParseResult<AstNode>::success();
		} else {
			return ParseResult<AstNode>::failure("Failed to parse"); 
		}
	}

 private:
	char char_to_parse_;
};

}  // namespace frontend
}  // namespace cs160

#endif // ATOM_PARSER_H_