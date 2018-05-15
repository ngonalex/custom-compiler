#include "frontend/combinators/v1_combinators/add_sub_expr.h"

#define super NullParser

ParseStatus AddSubExprParser::parse(std::string inputProgram) {
	if (inputProgram.size() == 0) {
		return super::parse(inputProgram);
	}
  // ae
  MulDivExprParser lhs;
  ParseStatus mdParseStatus = lhs.parse(inputProgram);
	if (mdParseStatus.status) {
		// op
	  AddSubOpParser op;
		ParseStatus asParseStatus = op.parse(mdParseStatus.remainingCharacters);
		ParseStatus result = mdParseStatus;
		while (asParseStatus.status) {
	    TermExpr rhs;
	    ParseStatus rhsParseStatus = rhs.parse(asParseStatus.remainingCharacters);
	    result.ast = std::move(make_node((asParseStatus.parsedChracters), 
														 std::move(result.ast), 
														 std::move(rhsParseStatus.ast)));
	    asParseStatus = op.parse(rhsParseStatus.remainingCharacters);
	  }
		return result;	// Returning Sucess on MulDivExpr or AddSubExpr
	}
	return lhsParseStatus;	// Returning Failure on MulDivExpr
}

// Creating the AST Node
std::unique_ptr<const AstNode> AddSubExprParser::make_node(std::string op, 
  std::unique_ptr<const AstNode> first_leaf,
  std::unique_ptr<const AstNode> second_leaf) {
  switch(op) {
    case "+": {
      return make_unique<AddExpr>(std::move(first_leaf), 
																			 std::move(second_leaf));
    }
    case "-": {
      return make_unique<SubtractExpr>(std::move(first_leaf), 
																	   std::move(second_leaf));
    }
    default: {
      return nullptr;
    }
  }
}