#include "frontend/combinators/v1_combinators/num_parser.h"

#define super NullParser

ParseStatus AddSubParser::parse(std::string inputProgram) {
	if (inputProgram.size() == 0) {
		return super::parse(inputProgram);
	}
  MulDivExpr lhs;
  lhs.parse(inputProgram);
  
  SingleOperatorParser op;
  op.parse(lhs.result.remainingCharacters);
  
  // If it cannot parse an operator, just return the result from MulDiv
  if (op.result.status == false) {
    // return the AST result from MulDiv
  }
  
  firstResult =
  ParseStatus result = 
  NumParser leftNum;
  NumParser rightNum;
  
  OneOrMoreCombinator oneOrMore;
	
	oneOrMore.parser = reinterpret_cast<NullParser *>(&digitParser);
	
  return (oneOrMore.parse(inputProgram));
}
