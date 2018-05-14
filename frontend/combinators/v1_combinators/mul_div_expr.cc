#include "frontend/combinators/v1_combinators/mul_div_expr.h"

#define super NullParser

ParseStatus MulDivExprParser::parse(std::string inputProgram) {
	if (inputProgram.size() == 0){
		return super::parse(inputProgram);
	}
	ParseStatus result;

  // ae
  TermExpr lhs;
  ParseStatus teParseStatus = lhs.parse(inputProgram);
  // Append to result's AST here

  // op
  MulDivOpParser op;
	ParseStatus mdParseStatus = op.parse(teParseStatus.remainingCharacters);

  // ae
  // Loop through until you're not hitting any more operators
  while (mdParseStatus.status) {
    TermExpr rhs;
    ParseStatus rhsParseStatus = rhs.parse(mdParseStatus.remainingCharacters);
    // Append to result's AST here
    result = op.parse(rhsParseStatus.remainingCharacters);
  }

  return result;
}
