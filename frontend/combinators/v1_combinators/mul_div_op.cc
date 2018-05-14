#include "frontend/combinators/v1_combinators/mul_div_op.h"

#define super NullParser

ParseStatus MulDivOpParser::parse(std::string inputProgram) {
	if (inputProgram.size() == 0){
		return super::parse(inputProgram);
	}
	ParseResult result;
  
  // ae
  TermExpr lhs;
  lhs.parse(inputProgram);
  // Append to result's AST here
  
  // op
  MulDivOpParser op;
  op.parse(lhs.result.remainingCharacters);
    
  // ae
  // Loop through until you're not hitting any more operators
  while (op.result.status) {
    TermExpr rhs;
    rhs.parse(op.result.remainingChracters);
    // Append to result's AST here
    result = op.parse(rhs.result.remainingChracters);
  }
  
  return result;
}
