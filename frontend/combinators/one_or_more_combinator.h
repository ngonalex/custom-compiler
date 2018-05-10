#ifndef ONE_OR_MORE_COMBINATOR_H_
#define ONE_OR_MORE_COMBINATOR_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/null.h"

class OneOrMoreCombinator : NullParser
{
	NullParser *parser;

	virtual ParseStatus parse(std::string inputProgram);
};

#endif ONE_OR_MORE_COMBINATOR_H_
