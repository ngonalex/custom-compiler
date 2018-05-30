#ifndef ATOM_PARSER_H_
#define ATOM_PARSER_H_

#include "frontend/combinators/peg_library/parse_status.h"

namespace cs160 {
namespace frontend {

class AtomParser {
 public:
	explicit AtomParser(char char_to_parse) {
		char_to_parse_ = char_to_parse;
	}

	ParseStatus parse() {

	}

 private:
	char char_to_parse_;
};

}  // namespace frontend
}  // namespace cs160

#endif // ATOM_PARSER_H_
