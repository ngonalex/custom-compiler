#ifndef ZERO_OR_MORE_COMBINATOR_H_
#define ZERO_OR_MORE_COMBINATOR_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

class ZeroOrMoreCombinator : NullParser
{
public:
	NullParser *parser;

	virtual ParseStatus parse(std::string inputProgram);
};

#endif // ZERO_OR_MORE_COMBINATOR_H_
