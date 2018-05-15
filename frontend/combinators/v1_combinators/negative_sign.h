#ifndef NEGATIVE_SIGN_PARSER_
#define NEGATIVE_SIGN_PARSER_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {

class NegativeParser : NullParser {
public:
	virtual ParseStatus parse(std::string inputProgram);
};

} // namespace frontend
} // namespace cs160

#endif // NEGATIVE_SIGN_PARSER_
