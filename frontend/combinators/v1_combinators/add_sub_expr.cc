#include "frontend/combinators/v1_combinators/add_sub_expr.h"

#define super NullParser

using namespace cs160::frontend;
using namespace std;

ParseStatus AddSubExprParser::parse(std::string inputProgram) {
	if (inputProgram.size() == 0) {
		return super::parse(inputProgram);
	}
  // ae
  MulDivExprParser lhs;
  ParseStatus result = lhs.parse(inputProgram);
	if (result.status) {
		// op
	  AddSubOpParser op;
		ParseStatus asParseStatus = op.parse(result.remainingCharacters);
		while (asParseStatus.status) {
	    MulDivExprParser rhs;
	    ParseStatus rhsParseStatus = rhs.parse(asParseStatus.remainingCharacters);
	    result.ast = std::move(make_node((asParseStatus.parsedCharacters),
														 std::move(result.ast),
														 std::move(rhsParseStatus.ast)));
	    asParseStatus = op.parse(rhsParseStatus.remainingCharacters);
	  }
		result.remainingCharacters = asParseStatus.remainingCharacters;
	}
	return result;	// Returning Success/Failure on MulDivExpr
}

// Creating the AST Node
std::unique_ptr<const AstNode> AddSubExprParser::make_node(std::string op,
  std::unique_ptr<const AstNode> first_leaf,
  std::unique_ptr<const AstNode> second_leaf) {
		if (op == "+") {
            return make_unique<AddExpr>(std::move(first_leaf), std::move(second_leaf));
		} else if (op == "-") {
        return make_unique<SubtractExpr>(std::move(first_leaf),
																	   std::move(second_leaf));
    } else {
			return nullptr;
		}
}