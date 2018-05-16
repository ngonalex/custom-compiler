#include "frontend/combinators/v1_combinators/mul_div_expr.h"

#define super NullParser

using namespace cs160::frontend;

ParseStatus MulDivExprParser::parse(std::string inputProgram) {
	if (inputProgram.size() == 0) {
		return super::parse(inputProgram);
	}
  // ae
  TermExprParser lhs;
  ParseStatus teParseStatus = lhs.parse(inputProgram);
	if (teParseStatus.status) {
		// op
	  MulDivOpParser op;
		ParseStatus mdParseStatus = op.parse(teParseStatus.remainingCharacters);
		ParseStatus result = teParseStatus;
		while (mdParseStatus.status) {
	    TermExprParser rhs;
	    ParseStatus rhsParseStatus = rhs.parse(mdParseStatus.remainingCharacters);
	    result.ast = std::move(make_node((mdParseStatus.parsedCharacters), 
														 std::move(result.ast), 
														 std::move(rhsParseStatus.ast)));
	    mdParseStatus = op.parse(rhsParseStatus.remainingCharacters);
	  }
		return result; // Return Sucess for TermExpr or MultExpr
	}
	return teParseStatus;	// Returning Failure on TermExpr
}

// Creating the AST Node
std::unique_ptr<const AstNode> MulDivExprParser::make_node(std::string op, 
  std::unique_ptr<const AstNode> first_leaf,
  std::unique_ptr<const AstNode> second_leaf) {
  if (op == "*":) {
    return make_unique<MultiplyExpr>(std::move(first_leaf), 
																		 std::move(second_leaf));
  }
  else if (op == "/") {
    return make_unique<DivideExpr>(std::move(first_leaf), 
																   std::move(second_leaf));
  }
  default: {
    return nullptr;
  }
}