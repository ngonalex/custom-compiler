#ifndef AND_COMBINATOR_H_
#define AND_COMBINATOR_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/null.h"

class AndCombinator : NullParser
{
public: 
	NullParser *firstParser;
	NullParser *secondParser;

	virtual ParseStatus parse(std::string inputProgram);
};

#endif AND_COMBINATOR_H_
