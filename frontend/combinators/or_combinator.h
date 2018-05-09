#ifndef OR_COMBINATOR_H_
#define OR_COMBINATOR_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/tokenizer/token.h"
#include "frontend/combinators/null.h"

class OrCombinator : NullParser
{
	NullParser *firstParser;
	NullParser *secondParser;

	virtual ParseStatus parse(std::string inputProgram);
};

#endif OR_COMBINATOR_H_