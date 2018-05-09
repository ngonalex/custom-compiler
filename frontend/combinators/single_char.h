#ifndef SINGLE_CHAR_H_
#define SINGLE_CHAR_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/null.h"

class SingleCharParser : NullParser
{
public:
	virtual ParseStatus parse(std::string inputProgram);
};

#endif SINGLE_CHAR_H_
