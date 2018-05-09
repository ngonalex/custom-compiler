#ifndef SINGLE_CHAR_H_
#define SINGLE_CHAR_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/tokenizer/token.h"
#include "frontend/combinators/null.h"

class AndCombinator : NullParser
{
	NullParser firstParser;
	NullParser secondParser;

	virtual ParseStatus parse(std::string inputProgram);
};

#endif SINGLE_CHAR_H_