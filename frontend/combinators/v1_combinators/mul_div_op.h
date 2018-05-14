#ifndef MUL_DIV_OP_H_
#define MUL_DIV_OP_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

class MulDivOpParser : NullParser
{
public:
	virtual ParseStatus parse(std::string inputProgram);
};

#endif // MUL_DIV_OP_H_
