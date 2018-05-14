#include "frontend/combinators/v1_combinators/add_sub_expr.h"

#define super NullParser

ParseStatus AddSubParser::parse(std::string inputProgram) {
	if (inputProgram.size() == 0) {
		return super::parse(inputProgram);
	}
  ParseStatus result;
  
  // ae
  MulDivExpr lhs;
  lhs.parse(inputProgram);
  // Append to result's AST here
  
  // op
  AddSubOpParser op;
  op.parse(lhs.result.remainingCharacters);
    
  // ae
  // Loop through until you're not hitting any more operators
  while (op.result.status) {
    MulDivExpr rhs;
    rhs.parse(op.result.remainingChracters);
    // Append to result's AST here
    result = op.parse(rhs.result.remainingCharacters);
  }
  
  return result;
}
