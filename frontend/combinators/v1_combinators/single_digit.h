#ifndef SINGLE_DIGIT_H_
#define SINGLE_DIGIT_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

class SingleDigitParser : NullParser
{
public:
	virtual ParseStatus parse(std::string inputProgram);
};

#endif // SINGLE_DIGIT_H_
