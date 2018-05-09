#ifndef SINGLE_OP_H_
#define SINGLE_OP_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/tokenizer/token.h"
#include "frontend/combinators/null.h"

class SingleOperatorParser : NullParser
{
	virtual ParseStatus parse(std::string inputProgram);
};

#endif SINGLE_OP_H_