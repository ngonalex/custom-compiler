#ifndef NEGATIVE_SIGN_PARSER_
#define NEGATIVE_SIGN_PARSER_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

class NegativeParser : NullParser {
public:
	virtual ParseStatus parse(std::string inputProgram);
};

#endif // NEGATIVE_SIGN_PARSER_
