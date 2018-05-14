#include "frontend/combinators/v1_combinators/add_sub_expr.h"

#define super NullParser

ParseStatus AddSubParser::parse(std::string inputProgram) {
	if (inputProgram.size() == 0) {
		return super::parse(inputProgram);
	}
  ParseStatus result;

  // ae
  MulDivExprParser lhs;
  ParseStatus lhsParseStatus = lhs.parse(inputProgram);
  // Append to result's AST here

  // op
  AddSubOpParser op;
  ParseStatus opParseStatus = op.parse(lhsParseStatus.remainingCharacters);

  // ae
  // Loop through until you're not hitting any more operators
  while (opParseStatus.status) {
    MulDivExprParser rhs;
    ParseStatus rhsParseStatus = rhs.parse(opParseStatus.remainingCharacters);
    // Append to result's AST here
    result = op.parse(rhsParseStatus.remainingCharacters);
  }

  return result;
}
