#include "frontend/combinators/v1_combinators/mul_div_expr.h"
#include "iostream"

#define super NullParser

using namespace cs160::frontend;
using namespace std;

ParseStatus MulDivExprParser::parse(std::string inputProgram) {
	trim(inputProgram);

	if (inputProgram.size() == 0) {
		return super::parse(inputProgram);
	}

  // ae
  TermExprParser lhs;
  ParseStatus result = lhs.parse(inputProgram);
	if (result.status) {
		// op
	  MulDivOpParser op;
		ParseStatus mdParseStatus = op.parse(result.remainingCharacters);
		while (mdParseStatus.status) {
	    TermExprParser rhs;
	    ParseStatus rhsParseStatus = rhs.parse(mdParseStatus.remainingCharacters);
	    result.ast = std::move(make_node((mdParseStatus.parsedCharacters),
	    	unique_cast<const ArithmeticExpr>(std::move(result.ast)),
	    	unique_cast<const ArithmeticExpr>(std::move(rhsParseStatus.ast))));
	    mdParseStatus = op.parse(rhsParseStatus.remainingCharacters);
	  }
		result.remainingCharacters = mdParseStatus.remainingCharacters;
	}
	return result;	// Returning Success/Failure on TermExpr
}

// Creating the AST Node
std::unique_ptr<const ArithmeticExpr> MulDivExprParser::make_node(std::string op,
  std::unique_ptr<const ArithmeticExpr> first_leaf,
  std::unique_ptr<const ArithmeticExpr> second_leaf) {
  if (op == "*") {
      return make_unique<MultiplyExpr>(std::move(first_leaf),
																		 std::move(second_leaf));
   } else if (op == "/") {
      return make_unique<DivideExpr>(std::move(first_leaf),
																   std::move(second_leaf));
  } else {
    return nullptr;
  }
}
