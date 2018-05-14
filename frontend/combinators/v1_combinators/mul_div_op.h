#ifndef MUL_DIV_OP_H_
#define MUL_DIV_OP_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"
#include "frontend/combinators/basic_combinators/parsestatus.h"
#include "frontend/combinators/v1_combinators/term_expr.h"
class MulDivOpParser : NullParser
{
public:
	virtual ParseStatus parse(std::string inputProgram);
};

#endif // MUL_DIV_OP_H_
