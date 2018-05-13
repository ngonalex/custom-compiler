#ifndef SINGLE_OP_H_
#define SINGLE_OP_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

class SingleOperatorParser : NullParser
{
public:
	virtual ParseStatus parse(std::string inputProgram);
};

#endif // SINGLE_OP_H_
