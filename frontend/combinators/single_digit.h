#ifndef SINGLE_DIGIT_H_
#define SINGLE_DIGIT_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/tokenizer/token.h"
#include "frontend/combinators/null.h"

class SingleDigitParser : NullParser
{
	virtual ParseStatus parse(std::string inputProgram);
};

#endif SINGLE_DIGIT_H_